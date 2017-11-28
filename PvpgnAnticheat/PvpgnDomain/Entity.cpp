#include "Entity.h"

using namespace Domain;

Entity::Entity(int id)
{
	(*this).id = id;
}


Entity::~Entity(void)
{
}

/*virtual*/ std::string Entity::ToString()
{
	return IntToString(id);
}

bool Entity::IsEmpty() const
{
	return id <= 0;
}

int Entity::Id() const
{
	return id;
}