#pragma once

#include "Logger.h"

namespace SelfServices_Abstract
{
	class LogManager : public BaseObject
	{
		public:
			Logger* GetLogger(BaseObject baseObject);
	};
}