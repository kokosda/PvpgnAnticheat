#pragma once

#include "../BaseObjects/BaseObject.h"

using namespace BaseObjects;
using namespace std;

namespace Domain_AbstractLayers
{
	class HashingService : public BaseObject
	{
		public:
	
			virtual wstring GetHash(const unsigned char* dataBuff, unsigned long dataBuffLength) = 0;
	};
}