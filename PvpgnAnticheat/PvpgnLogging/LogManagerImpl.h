#pragma once

#include "LogManager.h"

using namespace SelfServices_Abstract;

namespace SelfServices
{
	class LogManagerImpl : public LogManager
	{
		public:
			LogManagerImpl(string logFileName);

			~LogManagerImpl();

			static Logger* GetLogger(BaseObject baseObject);

		private:
			static Logger* instance;
	};
}