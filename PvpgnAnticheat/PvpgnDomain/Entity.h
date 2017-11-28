#pragma once

#include "../BaseObjects/BaseObject.h"

using namespace BaseObjects;
using namespace std;

namespace Domain
{
	class Entity : public BaseObject
	{
		public:
			Entity(int id);

			~Entity(void);

			virtual string ToString();

			virtual bool IsEmpty() const;

			int Id() const;

		protected:
			int id;
	};
}