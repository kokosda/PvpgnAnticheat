#include "ServiceContainer.h"

using namespace SelfServices;

map<string, void*> ServiceContainer::container;

void* ServiceContainer::GetImpl(string className)
{
	return container[className];
}

void ServiceContainer::AddImpl(string className, void* implObj)
{
	map<string, void*>::iterator it = container.find(className);

	if (it == container.end())
	{
		container[className] = implObj;
	}
}