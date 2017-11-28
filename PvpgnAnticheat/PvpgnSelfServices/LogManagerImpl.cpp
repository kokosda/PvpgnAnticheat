#include "LogManagerImpl.h"
#include "LoggerImpl.h"

using namespace SelfServices;

LogManagerImpl::LogManagerImpl(wstring *logFileName)
{
	if (!loggerInstance)
	{
		loggerInstance = new LoggerImpl();
	}

	loggerInstance->DestinationFile(*logFileName);
}

LogManagerImpl::~LogManagerImpl()
{
	delete loggerInstance;
}

Logger* LogManagerImpl::GetLogger()
{
	return loggerInstance;
}

Logger* LogManagerImpl::GetLogger(BaseObject &baseObject)
{
	loggerInstance->PreLog(baseObject);
	
	return loggerInstance;
}

void LogManagerImpl::Init(wstring logFileName)
{
	if (!instance)
	{
		instance = new LogManagerImpl(&logFileName);
	}
}