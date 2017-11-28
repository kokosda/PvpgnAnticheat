#pragma once

#include "Entity.h"

using namespace Domain;

namespace Domain_Clients
{
	class Client : public Entity
	{
		public:
			Client(wstring destinationLauncher, int id);

			~Client(void);

			wstring ToWstring();

			string Version();

			wstring DestinationLauncher();
	
			wstring DestinationLauncher() const;
		private:
			const string version;

			const wstring destinationLauncher;
	};
}