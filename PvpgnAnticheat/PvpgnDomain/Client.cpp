#include "Client.h"
using namespace Domain_Clients;

Client::Client(wstring destinationLauncher, int id) : 
				Entity(id), 
				version("0.1"), 
				destinationLauncher(destinationLauncher)
{	
}

Client::~Client(void)
{
}

wstring Client::ToWstring()
{
	return BaseObject::ToWstring();
}

string Client::Version() 
{
	return version;
}

wstring Client::DestinationLauncher() 
{
	return destinationLauncher;
}

wstring Client::DestinationLauncher() const
{
	return destinationLauncher;
}