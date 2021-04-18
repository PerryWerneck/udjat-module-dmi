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

int main(int argc, char **argv) {

	Logger::redirect();

	auto module = udjat_module_init();

	auto root_agent = Abstract::Agent::set_root(make_shared<Abstract::Agent>("root","System","Application"));

	{
		pugi::xml_document doc;
		doc.load_file("test.xml");
		root_agent->load(doc);
	}

	Udjat::start();

	{
		Response response;
		Worker::work("agent",Request("/sample"),response);
		cout << response.toStyledString() << endl;
	}

	/*
	{
		Json::Value response(Json::objectValue);
		root_agent->get(response,true,true);
		cout << response.toStyledString() << endl;
	}
	*/

	Udjat::stop();

	delete module;
	return 0;
}
