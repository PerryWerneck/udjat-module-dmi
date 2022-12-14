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

 Udjat::DMI::Controller * Udjat::DMI::Controller::instance = nullptr;

 const Udjat::ModuleInfo Udjat::DMI::moduleinfo{"DMI Access Module"};

 Udjat::DMI::Controller::Controller() : Udjat::Module("DMI",moduleinfo) {
	instance = this;
 }

 Udjat::DMI::Controller::~Controller() {
	cout << "DMI\tModule unloaded" << endl;
	instance = nullptr;
 }

 Udjat::DMI::Controller & Udjat::DMI::Controller::getInstance() {
	if(!instance) {
		throw runtime_error("DMI Controller is not available");
	}
	return *instance;
 }

