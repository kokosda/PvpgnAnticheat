#pragma once

#include <sstream>
#include <algorithm>

namespace BaseObjects
{
	class BaseObject
	{
		public:
			BaseObject(void);

			~BaseObject(void);

			BaseObject(const BaseObject &bo);

			virtual std::string ToString();

			virtual std::wstring ToWstring();

			std::wstring ToWideString(std::string string);

			std::wstring ToWideString(unsigned char *bytes, unsigned long length);

			std::string ToString(std::wstring wstring);

			std::string ToString(const std::wstring wstring) const;

			std::string IntToString(int value);

			std::string LongToString(long value);

			std::string ULongToString(unsigned long value);

			void ClearStringStream(void);

			void ClearWstringStream(void);

			std::stringstream& StringStream(void);

			std::wstringstream& WstringStream(bool clear = true);

			std::wstring WstreamBuffer(void) const;

			std::string GetType();

			virtual bool IsEmpty();

			std::wstring ToWLower(const std::wstring &str) const;
			
		protected:
			std::stringstream stringStream;

			const std::stringstream stringStreamConst;

			std::wstringstream wstringStream;

			const std::wstringstream wstringStreamConst;
	};
}