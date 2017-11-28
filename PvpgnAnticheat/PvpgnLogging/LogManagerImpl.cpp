#include "LogManagerImpl.h"
#include "LoggerImpl.h"

using namespace SelfServices;
using namespace SelfServices_Abstract;

LogManagerImpl::LogManagerImpl(string fileName)
{
	if (!instance)
	{
		instance = new LoggerImpl();
	}

	instance->DestinationFile(fileName);
}

Logger* LogManagerImpl::GetLogger(BaseObject baseObject)
{
	instance->PreLog(baseObject);
	
	return instance;
}