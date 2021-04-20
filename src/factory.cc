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

 #include "config.h"
 #include "private.h"

 Udjat::DMI::Factory::Factory() : Udjat::Factory(Quark::getFromStatic("dmi")) {

	cout << "DMI\tAgent factory is active" << endl;

	static const Udjat::ModuleInfo info = {

		PACKAGE_NAME,			// The module name.
		"DMI Agent factory", 	// The module description.
		PACKAGE_VERSION, 		// The module version.
		PACKAGE_BUGREPORT, 		// The bugreport address.
		PACKAGE_URL, 			// The package URL.
		nullptr
	};

	this->info = &info;

 }

 Udjat::DMI::Factory::~Factory() {
	cout << "DMI\tAgent factory is inactive" << endl;
 }


 void Udjat::DMI::Factory::parse(Abstract::Agent &parent, const pugi::xml_node &node) const {
#ifdef DEBUG
	cout << "DMI\tParsing DMI agent factory" << endl;
#endif // DEBUG

	parent.insert(make_shared<Agent>(node));

 }

