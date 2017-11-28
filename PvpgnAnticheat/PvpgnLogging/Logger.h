#pragma once

#include "../BaseObjects/BaseObject.h"
#include <string>

using namespace BaseObjects;
using namespace std;

namespace SelfServices_Abstract
{
	class Logger : public BaseObject
	{
		public:
			virtual void LogInfo(string format, ...) = 0;

			virtual void LogWarn(string format, ...) = 0;

			virtual void LogError(string format, ...) = 0;

			virtual void LogFatal(string format, ...) = 0;

			virtual void DestinationFile(string newLogFileName) = 0;

			virtual void Enable(bool enabled) = 0;

			virtual void PreLog(BaseObject baseObject) = 0;
	};
}