#include "LoggerImpl.h"
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

using namespace SelfServices;

LoggerImpl::LoggerImpl(void)
{
	dateFormat = "'['HH':'mm':'ss']";
}

/*virtual*/ LoggerImpl::~LoggerImpl(void)
{
	if (currentFile)
	{
		CloseHandle(currentFile);
	}
}

void LoggerImpl::LogInfo(string format, ...)
{
	currentLogString.append("INFO:");

	va_list argsList;

	BaseLog(format, argsList);
}

void LoggerImpl::LogWarn(string format, ...)
{
	currentLogString.append("WARN:");

	va_list argsList;

	BaseLog(format, argsList);
}

void LoggerImpl::LogError(string format, ...)
{
	currentLogString.append("ERROR:");

	va_list argsList;

	BaseLog(format, argsList);
}

void LoggerImpl::LogFatal(string format, ...)
{
	currentLogString.append("FATAL:");

	va_list argsList;

	BaseLog(format, argsList);
}

void LoggerImpl::DestinationFile(string newLogFileName)
{
	currentDestiationFileName = newLogFileName;

	if (currentFile)
	{
		CloseHandle(currentFile);

		currentFile = NULL;
	}
}

void LoggerImpl::Enable(bool enabled)
{
	this->enabled = enabled;

	if (!enabled && currentFile)
	{
		CloseHandle(currentFile);
	}
}

void LoggerImpl::PreLog(BaseObject baseObject)
{
	if (enabled)
	{
		currentLogString = string();
		currentLogString.append(CurrentDateTime());
		currentLogString.append(" ");
		currentLogString.append(baseObject.GetType());
		currentLogString.append(" ");
	}
}

#pragma region private

string LoggerImpl::CurrentDateTime()
{
	wchar_t timerString;

	GetDateFormat(LOCALE_NEUTRAL, TIME_FORCE24HOURFORMAT, NULL, ToWideString(dateFormat).c_str(), &timerString, NULL);

	return ToString(&timerString);
}

void LoggerImpl::BaseLog(string format, va_list argsList)
{
	string result = ReadMultipleParams(format, argsList);

	AppendLineEnd(result);

	WriteLine(currentDestiationFileName, result);
}

string LoggerImpl::ReadMultipleParams(string format, va_list ap)
{
	int size = 100;
    string str;

	while (true) 
	{
        str.resize(size);
        va_start(ap, format);
        int n = vsnprintf((char *)str.c_str(), size, format.c_str(), ap);
        va_end(ap);

        if (n > -1 && n < size) 
		{
            str.resize(n);
            return str;
        }
        if (n > -1)
		{
            size = n + 1;
		}
        else
		{
            size *= 2;
		}
    }

	return str;
}

void LoggerImpl::AppendLineEnd(string line)
{
	currentLogString.append(" ");
	currentLogString.append(line);
	currentLogString.append("\n\r");
}

void LoggerImpl::WriteLine(string fileName, string line)
{
	if (fileName.length() <= MAX_PATH)
	{
		if (!currentFile)
		{
			BaseObject baseObject;
			currentFile = CreateFile(baseObject.ToWideString(fileName).c_str(), GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		
		DWORD errorCode = GetLastError();

		if (currentFile != INVALID_HANDLE_VALUE && errorCode != ERROR_FILE_NOT_FOUND && errorCode != ERROR_SHARING_VIOLATION)
		{
			DWORD bytesWritten;
			WriteFile(currentFile, line.c_str(), line.length(), &bytesWritten, NULL);
		}
	}
}

#pragma endregion