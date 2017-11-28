#pragma once

#include "Logger.h"

using namespace SelfServices_Abstract;

namespace SelfServices
{
	class LogManagerImpl : public BaseObject
	{
		public:
			static void Init(wstring logFileName);

			~LogManagerImpl(void);

			static Logger* GetLogger();

			static Logger* GetLogger(BaseObject &baseObject);

		private:			
			LogManagerImpl(wstring *logFileName);

			static Logger* loggerInstance;
			
			static LogManagerImpl* instance;
	};
}