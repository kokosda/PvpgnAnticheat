#include "BaseObject.h"

using namespace BaseObjects;

BaseObject::BaseObject(void) : stringStreamConst(std::stringstream()), wstringStreamConst(std::wstringstream())
{
	stringStream = std::stringstream();
	wstringStream = std::wstringstream();
}

BaseObject::~BaseObject(void)
{
	//
}

BaseObject::BaseObject(const BaseObject &bo)
{
}

std::string BaseObject::ToString()
{
	return typeid(*this).name();
}

std::wstring BaseObject::ToWstring()
{
	return ToWideString(typeid(*this).name());
}

std::wstring BaseObject::ToWideString(std::string string)
{
	return std::wstring(string.begin(), string.end());
}

std::wstring BaseObject::ToWideString(unsigned char *bytes, unsigned long length)
{
	if (std::strlen((char *)bytes) >= length)
	{
		return std::wstring(bytes, bytes + length);
	}

	throw std::exception("Length is out of range.");
}

std::string BaseObject::ToString(std::wstring wstring)
{
	return std::string(wstring.begin(), wstring.end());
}

std::string BaseObject::ToString(const std::wstring wstring) const
{
	return std::string(wstring.begin(), wstring.end());
}

std::string BaseObject::IntToString(int value)
{
	ClearStringStream();

	stringStream << value;

	return stringStream.str();
}

std::string BaseObject::LongToString(long value)
{
	ClearStringStream();

	stringStream << value;

	return stringStream.str();
}

std::string BaseObject::ULongToString(unsigned long value)
{
	ClearStringStream();

	stringStream << value;

	return stringStream.str();
}

void BaseObject::ClearStringStream(void)
{
	stringStream.str(std::string());
}

void BaseObject::ClearWstringStream(void)
{
	wstringStream.str(std::wstring());
}

std::stringstream& BaseObject::StringStream(void)
{
	ClearStringStream();

	return stringStream;
}

std::wstringstream& BaseObject::WstringStream(bool clear)
{
	if (clear)
	{
		ClearWstringStream();
	}

	return wstringStream;
}

std::wstring BaseObject::WstreamBuffer(void) const
{
	return wstringStream.str();
}

std::string BaseObject::GetType()
{
	return typeid(*this).name();
}

bool BaseObject::IsEmpty()
{
	return true;
}

std::wstring BaseObject::ToWLower(const std::wstring &str) const
{
	std::wstring loweredStr = str;
	std::transform(loweredStr.begin(), loweredStr.end(), loweredStr.begin(), towlower);

	return loweredStr;
}