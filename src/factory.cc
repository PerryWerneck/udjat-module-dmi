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
 #include <udjat/moduleinfo.h>

 namespace Udjat {

	static const ModuleInfo moduleinfo{"DMI Agent factory"};

	DMI::Factory::Factory() : Udjat::Factory("dmi",moduleinfo) {
	}

	DMI::Factory::~Factory() {
	}

	std::shared_ptr<Abstract::Agent> DMI::Factory::AgentFactory(const Abstract::Object UDJAT_UNUSED(&parent), const pugi::xml_node &node) const {
		return make_shared<DMI::Agent>(node);
	}

	std::shared_ptr<Abstract::Agent> DMI::Factory::factory(const char *id) const {
		return make_shared<DMI::Agent>(id);
	}


 }
