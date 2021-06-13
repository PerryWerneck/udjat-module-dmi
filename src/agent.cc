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

 Udjat::DMI::Agent::Agent(const char *id) : Abstract::Agent("dmi") {

	// Reset
	this->id[0] = this->id[1] = this->id[2] = 0;

	string indexes[3];

	for(size_t ix=0; ix<3 && *id; ix++) {

		const char *next = strchr(id,'.');
		if(!next) {
			indexes[ix] = id;
			break;
		}

		indexes[ix] = string(id,next-id);
		id = next+1;
	}

#ifdef DEBUG
	cout	<< "I[0]= '" << indexes[0] << "'" << endl
			<< "I[1]= '" << indexes[1] << "'" << endl
			<< "I[2]= '" << indexes[2] << "'" << endl;
#endif // DEBUG

	// Check for the first element.
	if(indexes[0].empty()) {
		return;
	}

	this->id[0] = ::Type::indexByName(indexes[0].c_str());
	this->id[1] = (uint8_t) stoi(indexes[1]);
	this->id[2] = (uint8_t) stoi(indexes[2]);

 }

 Udjat::DMI::Agent::Agent(const pugi::xml_node &node) : Agent(node.attribute("id").as_string()) {

	load(node);

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

 Udjat::Value & Udjat::DMI::Agent::get(Udjat::Value &value){
	value = this->to_string();
	return value;
 }

 std::string Udjat::DMI::Agent::to_string() const {

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

		error("Error '{}' getting DMI header length",e.what());
		return "";

 	}

 	// Read DMI Strings
 	try {

		read_file(path+"raw",buffer);
		const char *text = &buffer[offset];

		size_t ix = (size_t) id[2];
		while(ix-- > 0) {

#ifdef DEBUG
			cout << "ix=" << ix << " text='" << text << "'" << endl;
#endif // DEBUG

			if(!*text) {
				cerr << getName() << "\tCan't find required string" << endl;
				return "";
			}

			text += strlen(text)+1;

		}

		return text;

 	} catch(const exception &e) {

		error("Error '{}' reading raw DMI info",e.what());
		return "";
 	}

 }

