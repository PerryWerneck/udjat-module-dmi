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

#include <config.h>
#include <udjat/defs.h>
#include <udjat/agent.h>
#include <udjat/agent/abstract.h>
#include <udjat/tools/xml.h>

using namespace std;
using namespace Udjat;

namespace Udjat {

	namespace DMI {

		/// @brief SMBios Agent.
		class UDJAT_API Agent : public Udjat::Abstract::Agent {
		private:
			const char *url = nullptr;

		public:

			/// @brief SMBios Agent factory
			class UDJAT_API Factory : Abstract::Agent::Factory {
			public:
				Factory(const char *name = "smbios");
				virtual ~Factory();

				/// @brief Create an agent from XML node.
				/// @param node XML definition for the new agent.
				virtual std::shared_ptr<Abstract::Agent> AgentFactory(const XML::Node &node) const override;

			};

			Agent(const char *url);
			Agent(const XML::Node &node);
			virtual ~Agent();

			Udjat::Value & get(Udjat::Value &value) const override;

			std::string to_string() const noexcept override;

		};

	}

}
