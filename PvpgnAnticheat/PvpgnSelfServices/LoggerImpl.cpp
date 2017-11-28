#include "LoggerImpl.h"
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

using namespace SelfServices;

LoggerImpl::LoggerImpl(void)
{
	timeFormat = L"'['HH:mm:ss.";
	dateFormat = L"yyyy/MM/dd' '";
	dateFormatStrLen = wcslen(dateFormat);
	timeFormatStrlen = wcslen(timeFormat);
	dateStringBuffer = new wchar_t[dateFormatStrLen];
	timeStringBuffer = new wchar_t[timeFormatStrlen];

	currentFile = NULL;
}

/*virtual*/ LoggerImpl::~LoggerImpl(void)
{
	if (currentFile && currentFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(currentFile);
	}

	delete [] dateStringBuffer;
	delete [] timeStringBuffer;
}

void LoggerImpl::LogInfo(char* format, ...)
{
	currentLogString.append(" INFO: ");

	va_list argptr;
	va_start(argptr, format);

	BaseLog(format, argptr);

	va_end(argptr);
}

void LoggerImpl::LogWarn(char* format, ...)
{
	currentLogString.append(" WARN: ");

	va_list argptr;
	va_start(argptr, format);

	BaseLog(format, argptr);

	va_end(argptr);
}

void LoggerImpl::LogError(char* format, ...)
{
	currentLogString.append(" ERROR: ");

	va_list argptr;
	va_start(argptr, format);

	BaseLog(format, argptr);

	va_end(argptr);
}

void LoggerImpl::LogFatal(char* format, ...)
{
	currentLogString.append(" FATAL: ");

	va_list argptr;
	va_start(argptr, format);

	BaseLog(format, argptr);

	va_end(argptr);
}

void LoggerImpl::DestinationFile(const wstring &newLogFileName)
{
	currentDestiationFileName = newLogFileName;

	if (currentFile != NULL)
	{
		CloseHandle(currentFile);

		currentFile = NULL;
	}

	wstringstream result;
	result << L"PvpgnAnticheat: Destination logger file set to " << newLogFileName.c_str() << endl;
	OutputDebugString(result.str().c_str());
}

void LoggerImpl::Enable(bool enabled)
{
	this->enabled = enabled;

	if (!enabled && currentFile)
	{
		CloseHandle(currentFile);
	}
}

void LoggerImpl::PreLog(BaseObject &baseObject)
{
	if (enabled)
	{
		currentLogString = string();
		currentLogString.append(CurrentDateTime());
		currentLogString.append(" ");
		currentLogString.append(baseObject.GetType());
	}
}

#pragma region private

string LoggerImpl::CurrentDateTime()
{
	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);

	GetDateFormat(NULL, NULL, &currentTime, dateFormat, dateStringBuffer, dateFormatStrLen);
	GetTimeFormat(NULL, TIME_FORCE24HOURFORMAT, &currentTime, timeFormat, timeStringBuffer, timeFormatStrlen);
	
	wstringstream result;
	result << dateStringBuffer << timeStringBuffer << currentTime.wMilliseconds << L"]";

	return ToString(result.str());
}

void LoggerImpl::BaseLog(char* &format, va_list argPtr)
{
	if (enabled)
	{
		wchar_t* reformatedResult;
		wstring res = ToWideString(format);
		ReadMultipleParams(res.c_str(), argPtr, reformatedResult);

		currentLogString.append(ToString(reformatedResult));
		currentLogString.append("\n");

		delete [] reformatedResult;

		WriteLine(currentLogString);
	}
}

void LoggerImpl::ReadMultipleParams(const wchar_t* format, va_list argPtr, wchar_t* &result)
{
	int size = 256;
    string str;

	while (true) 
	{
        str.resize(size);
		wchar_t *buff = new wchar_t [str.size() + 1];

        int n = vswprintf_s(buff, str.size(), format, argPtr);
		
        if (n > -1 && n < size) 
		{
            str.resize(n);
			result = buff;
			break;
        }
        if (n > -1)
		{
            size = n + 1;
		}
        else
		{
            size *= 2;
		}

		delete [] buff;
    }
}

void LoggerImpl::WriteLine(string &line)
{
	if (currentDestiationFileName.length() <= MAX_PATH)
	{
		bool created = true;

		if (!currentFile || currentFile == INVALID_HANDLE_VALUE)
		{
			currentFile = CreateFile(currentDestiationFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			DWORD errorCode = GetLastError();

			if (currentFile == INVALID_HANDLE_VALUE || errorCode == ERROR_FILE_NOT_FOUND || errorCode == ERROR_SHARING_VIOLATION)
			{
				created = false;

				OutputDebugString(L"PvpgnAnticheat: Cannot open log file for writing.\n");
			}

			if (currentFile == INVALID_HANDLE_VALUE)
			{
				return;
			}
		}

		if (created)
		{
			SetFilePointer(currentFile, 0, NULL, FILE_END);
			SetEndOfFile(currentFile);

			DWORD bytesWritten;
			BOOL result = WriteFile(currentFile, currentLogString.c_str(), currentLogString.length(), &bytesWritten, NULL);

			if (!result)
			{
				OutputDebugString(L"PvpgnAnticheat: Cannot write string to destination log-file!\n");

				return;
			}

			if (bytesWritten != currentLogString.length())
			{
				OutputDebugString(L"PvpgnAnticheat: Source log string was unproperly wrote do destination log-file!\n");

				return;
			}
		}
	}
}

#pragma endregion