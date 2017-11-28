#pragma once

#include "../PvpgnDomain/HashingService.h"
#include <Windows.h>

using namespace Domain_AbstractLayers;
using namespace std;

namespace Infrastructure
{
	class HashingServiceImpl : public HashingService
	{
		public:
			HashingServiceImpl();

			virtual ~HashingServiceImpl();

			wstring GetHash(const unsigned char *dataBuff, unsigned long dataBuffLength);

		private:
			void ReleaseHash(HCRYPTHASH &hash);

			HCRYPTPROV cryptoContext;
	};
}