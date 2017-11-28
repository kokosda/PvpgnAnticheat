#pragma once

#include "Logger.h"

using namespace SelfServices_Abstract;

namespace SelfServices
{
	class LoggerImpl : public Logger
	{
		public:
			LoggerImpl(void);

			virtual ~LoggerImpl(void);

			void LogInfo(char* format, ...);

			void LogWarn(char* format, ...);

			void LogError(char* format, ...);

			void LogFatal(char* format, ...);

			void DestinationFile(const wstring &newLogFileName);

			void Enable(bool enabled);

			void PreLog(BaseObject &baseObject);

		#pragma region private

		private:
			wstring currentDestiationFileName;
			string currentLogString;

			const wchar_t* dateFormat;
			const wchar_t* timeFormat;
			wchar_t* dateStringBuffer;
			wchar_t* timeStringBuffer;
			int dateFormatStrLen;
			int timeFormatStrlen;

			bool enabled;

			void* currentFile;

			void ReadMultipleParams(const wchar_t* format, va_list argPtr, wchar_t * &result);

			void BaseLog(char* &format, va_list argPtr);

			void WriteLine(string &line);

			string CurrentDateTime();

		#pragma endregion
	};
}