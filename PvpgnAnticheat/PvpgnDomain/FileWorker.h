#pragma once

#include <string>
#include "../BaseObjects/BaseObject.h"

using namespace std;
using namespace BaseObjects;

namespace Domain_AbstractLayers
{
	class FileWorker : public BaseObject
	{
		public:
	
			virtual wstring ExecutionDirectory(bool get = true, wstring directory = L"") = 0;

			virtual unsigned long GetSize(wstring fileName) = 0;

			virtual unsigned long GetContent(wstring fileName, unsigned char* &result) = 0;

			virtual void WriteLine(string fileName, string line) = 0;

			virtual int ConcatPathes(const wstring &path1, const wstring &path2, wchar_t* &result) = 0;

			virtual int GetDirectory(const wstring &path, wchar_t* &result) = 0;
	};
}