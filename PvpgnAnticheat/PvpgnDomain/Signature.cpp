#include "Signature.h"

using namespace Domain_Anticheat;

Signature::Signature(wstring name, int id, enum SignatureEnumeration enumeration, bool positive) : 
			Entity(id), 
				positive(positive), enumeration(enumeration), name(name)
{
	value = wstring();
}

Signature::Signature(wstring name, wstring value, int id, bool positive) : Entity(id), positive(positive)
{
	enumeration = Simple;

	this->name = name;
	this->value = value;
}

Signature::Signature(const Signature &signature) : Entity(signature.id), positive(signature.positive)
{
	this->name = signature.name;
	this->value = signature.value;

	this->enumeration = signature.enumeration;
}

Signature::~Signature(void)
{
}

wstring Signature::Name() 
{
	return name;
}

wstring Signature::Name() const
{
	return name;
}

wstring Signature::Value()
{
	return value;
}

wstring Signature::Value() const
{
	return value;
}

enum SignatureEnumeration Signature::SignatureEnumeration()
{
	return enumeration;
}

bool Signature::TriggerSignatureByValue(const wstring &value) const
{
	if (positive && Value().compare(value) == 0)
	{
		return false;
	}
	else if (!positive && value.size() == 0)
	{
		return false;
	}
	
	return true;
}

enum SignatureEnumeration Signature::Enumeration() const
{
	return enumeration;
}

wstring Signature::ToWstring()
{
	WstringStream() << "Name: " << Signature::Name() << " Value: " << Signature::Value();

	return WstreamBuffer();
}

bool Signature::IsEmpty() const
{
	return value.size() == 0 || Entity::IsEmpty();
}

bool Signature::Positive() const
{
	return positive;
}