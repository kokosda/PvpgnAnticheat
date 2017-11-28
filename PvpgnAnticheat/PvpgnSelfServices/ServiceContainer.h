#pragma once

#include <map>
#include <typeinfo>
#include "../BaseObjects/BaseObject.h"

using namespace BaseObjects;
using namespace std;

namespace SelfServices
{
	class ServiceContainer : BaseObject
	{
		public:
			static void* GetImpl(string className);
			
			static void AddImpl(string className, void* implObj);
			
		private:
			static map<string, void*> container;
	};
}