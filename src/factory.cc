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

 static const char * dmipath = "/sys/firmware/dmi/";

 Udjat::DMI::Factory::Factory() : Abstract::Agent::Factory(Quark("dmi")) {

	cout << "DMI\tAgent factory is active" << endl;


 }

 Udjat::DMI::Factory::~Factory() {

	cout << "DMI\tAgent factory is inactive" << endl;

 }

 void Udjat::DMI::Factory::parse(Abstract::Agent &parent, const pugi::xml_node &node) const {



 }
