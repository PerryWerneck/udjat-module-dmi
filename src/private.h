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

#pragma once

#include <udjat/defs.h>
#include <udjat/module.h>
#include <udjat/agent.h>
#include <udjat/factory.h>
#include <stdexcept>

using namespace std;
using namespace Udjat;

namespace Udjat {

	namespace DMI {

		/// @brief SMBIOS read agent.
		class Factory : public Udjat::Factory {
		private:

		public:
			Factory();
			virtual ~Factory();

			bool parse(Abstract::Agent &parent, const pugi::xml_node &node) const override;
			std::shared_ptr<Abstract::Agent> factory(const char *id) const;

		};

		/// @brief
		class UDJAT_API Agent : public Udjat::Abstract::Agent {
		private:
			uint8_t id[3];

		public:
			Agent(const char *id);
			Agent(const pugi::xml_node &node);
			virtual ~Agent();

			Udjat::Value & get(Udjat::Value &value) override;

			std::string to_string() const override;

		};

		/// @brief DMI module controller.
		class Controller : public Udjat::Module {
		private:
			static Controller * instance;

			// Agent factory.
			Factory smbios;

		public:
			Controller();
			virtual ~Controller();

			static Controller & getInstance();

		};



	}

}
