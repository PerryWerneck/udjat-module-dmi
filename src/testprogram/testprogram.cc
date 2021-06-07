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

 #include "../private.h"
 #include <udjat.h>
 #include <udjat/module.h>
 #include <udjat/tools/logger.h>
 #include <udjat/worker.h>
 #include <pugixml.hpp>

 using namespace std;
 using namespace Udjat;

//---[ Implement ]------------------------------------------------------------------------------------------

static void test_agent_parser() {

	Abstract::Agent::init("test.xml");

	Udjat::run();

	Abstract::Agent::deinit();

}

int main(int argc, char **argv) {

	// Logger::redirect();

	auto module = udjat_module_init();

	{
		cout << "------------------------------" << endl;

		// cout << DMI::Agent("1.0.0").to_string() << endl;
		cout << DMI::Agent("1.0.1").to_string() << endl;
		cout << DMI::Agent("1.0.2").to_string() << endl;

		cout << "------------------------------" << endl;

	}

	/*
	{
		Response response;
		Request request("/sample");
		Worker::work("agent",request,response);
		cout << response.toStyledString() << endl;
	}
	*/

	/*
	{
		Json::Value response(Json::objectValue);
		root_agent->get(response,true,true);
		cout << response.toStyledString() << endl;
	}
	*/

	delete module;

	return 0;
}
