#pragma once

#include "../BaseObjects/BaseObject.h"

using namespace BaseObjects;
using namespace std;

namespace SelfServices_Abstract
{
	class Logger : public BaseObject
	{
		public:
			virtual void LogInfo(char* format, ...) = 0;

			virtual void LogWarn(char* format, ...) = 0;

			virtual void LogError(char* format, ...) = 0;

			virtual void LogFatal(char* format, ...) = 0;

			virtual void DestinationFile(const wstring &newLogFileName) = 0;

			virtual void Enable(bool enabled) = 0;

			virtual void PreLog(BaseObject &baseObject) = 0;
	};
}