#include "Event.h"

using namespace UI_Controls_Handlers;

Event::Event(void* args, Control* sender) : args(args), sender(sender)
{
}

Handler* Event::GetHandler(Handler* h)
{
	this->h = h;
}

Handler* Event::operator+=(Handler* h)
{
	this->h = h;
}