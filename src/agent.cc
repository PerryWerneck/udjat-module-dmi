/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2021 Perry Werneck <perry.werneck@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 // References:
 //
 // https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-firmware-dmi-entries
 // https://github.com/mirror/dmidecode/blob/master/dmidecode.c
 // https://gitlab.yottabyte.com/open-source/mcelog/-/blob/master/dmi.c

 #include "private.h"
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <cstring>

 static const char * dmipath = "/sys/firmware/dmi/";

 struct Type {

	const char *name;

 	static size_t indexByName(const char *name);

 };


 static const Type types[] = {

	Type{"BIOS"},
	Type{"System"},
	Type{"Base Board"},
	Type{"Chassis"},
	Type{"Processor"},
	Type{"Memory Controller"},
	Type{"Memory Module"},
	Type{"Cache"},
	Type{"Port Connector"},
	Type{"System Slots"},
	Type{"On Board Devices"},
	Type{"OEM Strings"},
	Type{"System Configuration Options"},
	Type{"BIOS Language"},
	Type{"Group Associations"},
	Type{"System Event Log"},
	Type{"Physical Memory Array"},
	Type{"Memory Device"},
	Type{"32-bit Memory Error"},
	Type{"Memory Array Mapped Address"},
	Type{"Memory Device Mapped Address"},
	Type{"Built-in Pointing Device"},
	Type{"Portable Battery"},
	Type{"System Reset"},
	Type{"Hardware Security"},
	Type{"System Power Controls"},
	Type{"Voltage Probe"},
	Type{"Cooling Device"},
	Type{"Temperature Probe"},
	Type{"Electrical Current Probe"},
	Type{"Out-of-band Remote Access"},
	Type{"Boot Integrity Services"},
	Type{"System Boot"},
	Type{"64-bit Memory Error"},
	Type{"Management Device"},
	Type{"Management Device Component"},
	Type{"Management Device Threshold Data"},
	Type{"Memory Channel"},
	Type{"IPMI Device"},
	Type{"Power Supply"},
	Type{"Additional Information"},
	Type{"Onboard Device"},
	Type{"Management Controller Host Interface"},
	Type{"TPM Device"},
 };


 #define type_length (sizeof(types)/sizeof(type[0]))



 static bool isNumber(const char *str) {
 	for(const char *ptr = str;*ptr;ptr++) {
		if(*ptr < '0' || *ptr > '9') {
			return false;
		}
 	}
	return true;
 }

 size_t Type::indexByName(const char *type) {

	size_t length = strlen(type);

 	if(isNumber(type))
		return atoi(type);

	for(size_t index = 0; index < type_length;index++ ) {
 		if(strncasecmp(types[index].name,type,length) == 0)
			return (uint8_t) index;
	}

	string msg{"Unexpected type name: "};
	msg += type;

	throw runtime_error(msg);

 }

 Udjat::DMI::Agent::Agent(const pugi::xml_node &node) {

#ifdef DEBUG
	cout << "DMI\tCreating agent " << node.attribute("name").as_string() << endl;
#endif // DEBUG

	// Reset
	id[0] = id[1] = id[2] = 0;

	// Check if the entry has an ID in the format type-instance-field
	{
		const char * id = node.attribute("id").as_string();
		if(id && *id) {

			// Split id.
			const char *first = id;
			for(size_t index = 0; index < 3; index++) {
				const char *last = strchr(first,'-');
				if(last) {
					this->id[index] = atoi(string(first,last-first).c_str());
				} else {
					this->id[index] = atoi(first);
					break;
				}

				first = last+1;
			}

		}

	}

	// Check for type attribute
	{
		auto type = node.attribute("type");
		if(type) {
			id[0] = ::Type::indexByName(type.as_string());
		}
	}

	// Check for instance attribute
	{
		auto instance = node.attribute("instance");
		if(instance) {
			id[1] = instance.as_uint(id[1]);
		}
	}

	// Get index
	{
		auto index = node.attribute("string-index");
		if(index) {
			id[2] = index.as_uint(id[2]);
		}
	}

	load(node);

#ifdef DEBUG
	cout << "DMI\tID is " 	<< std::to_string((int) this->id[0])
							<< "-" << std::to_string((int) this->id[1]) << "-"
							<< std::to_string((int) this->id[2]) << endl;
#endif // DEBUG


 }

 Udjat::DMI::Agent::~Agent() {

 }

 static size_t read_file(const string &filename, char buffer[4096]) {

	int fd = open(filename.c_str(),O_RDONLY);
	if(fd < 0) {
		string msg{"Error opening "};
		msg += filename;
		throw system_error(errno,system_category(),msg);
	}

	memset(buffer,0,4096);
	auto length = read(fd,buffer,4095);
	if(length < 0) {
		string msg{"Error reading "};
		msg += filename;
		auto err = errno;
		close(fd);
		throw system_error(err,system_category(),msg);
	}

	close(fd);

	return (size_t) length;

 }

 void Udjat::DMI::Agent::get(const char *name, Json::Value &value){

	char buffer[4096];
	size_t offset = 0;

 	// Build base path
	string path{dmipath};

	path += "entries/";
	path += std::to_string((unsigned int) id[0]);
	path += "-";
	path += std::to_string((unsigned int) id[1]);
	path += "/";

	// Get header length
	try {

		read_file(path+"length",buffer);
		offset = atoi(buffer);

 	} catch(const exception &e) {

		failed("Error getting DMI header length",e);
		throw;
 	}


 	// Read DMI Strings
 	try {

		read_file(path+"raw",buffer);
		const char *text = &buffer[offset];

		size_t ix = (size_t) id[2];
		while(ix-- > 0) {

			cout << "ix=" << ix << " text='" << text << "'" << endl;

			if(!*text) {
				throw runtime_error("Can't find required string");
			}

			text += strlen(text)+1;

		}

		value[name] = text;

 	} catch(const exception &e) {

		failed("Cant read raw DMI info",e);
		throw;
 	}

 }

