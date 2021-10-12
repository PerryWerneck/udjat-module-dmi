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
 #include <dmiget/table.h>

 Udjat::DMI::Protocol::Protocol() : Udjat::URL::Protocol("dmi","",&moduleinfo) {

 }

 std::shared_ptr<Udjat::URL::Response> Udjat::DMI::Protocol::call(const URL &url, const Udjat::URL::Method method, const char *mimetype, const char *payload) {

	class Response : public Udjat::URL::Response {
	private:
		std::string value;

	public:
		Response(const char *str) : value(str) {
			response.payload = value.c_str();
			response.length = value.size();
		}

	};

	if(method != Udjat::URL::Method::Get) {
		throw runtime_error("Invalid or unsupported request method");
	}

	string key{"dmi:/"};
	key += url.getFileName();

	return make_shared<Response>(::DMI::Table()[key.c_str()].c_str());
 }

