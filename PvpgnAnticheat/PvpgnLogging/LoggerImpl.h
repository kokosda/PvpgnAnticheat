#pragma once

#include <string>
#include "../BaseObjects/BaseObject.h"
#include "Logger.h"

using namespace std;
using namespace BaseObjects;
using namespace SelfServices_Abstract;

namespace SelfServices
{
	class LoggerImpl : public Logger
	{
		public:
			LoggerImpl(void);

			virtual ~LoggerImpl(void);

			void LogInfo(string format, ...);

			void LogWarn(string format, ...);

			void LogError(string format, ...);

			void LogFatal(string format, ...);

			void DestinationFile(string newLogFileName);

			void Enable(bool enabled);

			void PreLog(BaseObject baseObject);

		#pragma region private

		private:
			string ReadMultipleParams(string format, va_list params);

			void AppendLineEnd(string line);

			string CurrentDateTime();

			void WriteLine(string fileName, string line);

			void BaseLog(string line, va_list argsList);

			string currentDestiationFileName;

			string currentLogString;

			/*const*/ string dateFormat;

			bool enabled;

			void* currentFile;

		#pragma endregion
	};
}