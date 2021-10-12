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

 #include <udjat.h>
 #include <udjat/module.h>
 #include <unistd.h>
 #include <udjat/tools/systemservice.h>
 #include <udjat/url.h>

 using namespace std;
 using namespace Udjat;

 int main(int argc, char **argv) {

	setlocale( LC_ALL, "" );

	Logger::redirect(nullptr,true);

	auto module = udjat_module_init();
	auto agent = Abstract::Agent::init("./test.xml");

	try {

		Module::load("udjat-module-information");

	} catch(const std::exception &e) {
		cerr << "Error '" << e.what() << "' loading information module" << endl;
	}

	try {

		Module::load("http");

		cout << "http://localhost:8989/api/1.0/info/modules.xml" << endl;
		cout << "http://localhost:8989/api/1.0/info/protocols.xml" << endl;

		for(auto child : *agent) {
			cout << "http://localhost:8989/api/1.0/agent/" << child->getName() << ".xml" << endl;
		}

	} catch(const std::exception &e) {
		cerr << e.what() << endl;
	}

	try {

		cout << "dmi:///bios/version= '" << Udjat::URL("dmi:///bios/version").get()->c_str() << "'" << endl;

	} catch(const std::exception &e) {
		cerr << e.what() << endl;
	}

	cout << "Waiting for requests" << endl;

	Udjat::SystemService().run();

	Abstract::Agent::deinit();

	cout << "Removing module" << endl;
	delete module;
	Module::unload();

	return 0;
}
