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
 #include <udjat/tools/quark.h>
 #include <cstring>
 #include <dmiget/table.h>
 #include <dmiget/value.h>

 Udjat::DMI::Agent::Agent(const char *u) : url(Quark(u).c_str()) {

	try {

		auto value = ::DMIget::Table().find(this->url);
		if(value) {
			this->Object::properties.label = Quark(value->name()).c_str();
			this->Object::properties.summary = Quark(value->description()).c_str();
		} else {
			warning() << "Query for '" << this->url << "' returned an empty response" << endl;
		}

	} catch(const std::exception &e) {

		error() << this->url << ": '" << e.what() << "'" << endl;

	}

 }

 Udjat::DMI::Agent::Agent(const pugi::xml_node &node) : Abstract::Agent(node), url(Quark(node.attribute("url").as_string()).c_str()) {
 }

 Udjat::DMI::Agent::~Agent() {
 }

 Udjat::Value & Udjat::DMI::Agent::get(Udjat::Value &value) const {
	value = this->to_string();
	return value;
 }

 std::string Udjat::DMI::Agent::to_string() const noexcept {

 	try {

		return ::DMIget::Table()[this->url];

 	} catch(const std::exception &e) {

 		cerr << "dmi\tError '" << e.what() << "' getting " << this->url << endl;

 	} catch(...) {

		cerr << "dmi\tUnexpected error getting " << this->url << endl;

 	}

 	return "";
 }

