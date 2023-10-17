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
 #include <udjat/tools/protocol.h>
 #include <udjat/tools/url.h>
 #include <smbios/value.h>
 #include <udjat/tools/file.h>

 namespace Udjat {

	 DMI::Protocol::Protocol() : Udjat::Protocol("dmi",moduleinfo) {

	 }

	 String DMI::Protocol::call(const URL &url, const HTTP::Method UDJAT_UNUSED(method), const char UDJAT_UNUSED(*payload)) const {
		return Udjat::String{ SMBios::Value::find(url.c_str())->to_string() };
	 }

	 std::shared_ptr<Protocol::Worker> DMI::Protocol::WorkerFactory() const {

		class Worker : public Protocol::Worker {
		public:

			Worker() : Protocol::Worker{} {
			}

			String get(const std::function<bool(double, double)> &) override {
				return String{ SMBios::Value::find(url().c_str())->to_string() };
			}

			int test(const std::function<bool(double, double)> &) noexcept override {

				URL url{this->url()};

				try {

					String value{SMBios::Value::find(url.c_str())->to_string()};
					String pattern;

					if(value.empty()) {
						return 404;
					}

					pattern = url["value"];
					if(!pattern.empty()) {

						// Single value, test it.
						if(File::Path::match(value.c_str(),pattern.c_str())) {
							return 200;
						}

						return 404;

					}

					pattern = url["values"];
					if(!pattern.empty()) {

						// Multiple values, test it.
						return pattern.for_each(",",[&value](const String &str){

							if(File::Path::match(value.c_str(),str.c_str())) {
								return true;
							}

							return false;

						}) ? 200 : 404;

					}


					warning() << "Missing '?value=' or '?values=' on test URL" << endl;
					return EINVAL;

				} catch(const std::exception &e) {

					error() << url << ": " << e.what();
					return 500;

				} catch(...) {

					error() << url << ": Unexpected error" << endl;
					return 500;

				}

				return 200;

			}


		};

		return make_shared<Worker>();

	 }

 }


