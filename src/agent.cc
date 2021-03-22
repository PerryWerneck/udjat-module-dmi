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

 #include "private.h"
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <cstring>

 static const char * dmipath = "/sys/firmware/dmi/";

 Udjat::DMI::Agent::Agent(const pugi::xml_node &node) {

#ifdef DEBUG
	cout << "DMI\tCreating agent " << node.attribute("name").as_string() << endl;
#endif // DEBUG

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



#ifdef DEBUG
	cout << "DMI\tID is " << to_string((int) this->id[0]) << "-" << to_string((int) this->id[1]) << "-" << to_string((int) this->id[2]) << endl;
#endif // DEBUG


 }

 Udjat::DMI::Agent::~Agent() {

 }

 void Udjat::DMI::Agent::get(const char *name, Json::Value &value){

	char buffer[4096];

 	try {

		string path{dmipath};

		path += "entries/";
		path += to_string((unsigned int) id[0]);
		path += "-";
		path += to_string((unsigned int) id[1]);
		path += "/raw";

		int fd = open(path.c_str(),O_RDONLY);
		if(fd < 0) {
			string msg{"Error opening "};
			msg += path;
			throw system_error(errno,system_category(),msg);
		}

		memset(buffer,0,4096);
		auto length = read(fd,buffer,4095);
		if(length < 0) {
			string msg{"Error reading "};
			msg += path;
			auto err = errno;
			close(fd);
			throw system_error(err,system_category(),msg);
		}

		cout << "************* " << length << endl;

		close(fd);

 	} catch(const exception &e) {

		failed(e,"Cant access DMI data");
		throw;
 	}

 }

