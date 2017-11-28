#pragma once

#include "../PvpgnDomain/FileWorker.h"

#include <Windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <sys/stat.h>
#include <memory>

using namespace std;
using namespace Domain_AbstractLayers;

#define MAX_ABSOLUTE_PATH 32767
#define MAX_FILE_SIZE 1024 * 1024 * 500 //500 mb limit per file

namespace Infrastructure
{
	class FileWorkerImpl : public FileWorker
	{
		public:
			FileWorkerImpl(void);

			virtual ~FileWorkerImpl(void);

			wstring ExecutionDirectory(bool get = true, wstring directory = L"");

			unsigned long GetSize(wstring fileName);

			unsigned long GetContent(wstring fileName, unsigned char* &result);

			void WriteLine(string fileName, string line);

			int ConcatPathes(const wstring &path1, const wstring &path2, wchar_t* &result);

			int GetDirectory(const wstring &path, wchar_t* &result);

		#pragma region private

		private:
			HANDLE Open(wstring fileName); 

			unsigned long GetSize(HANDLE file);

			bool Read(HANDLE file, unsigned char* &result, unsigned long fileSize);

			void Write(HANDLE file, string line);

			BOOL Close(HANDLE file);

			HANDLE currentFile;
						
			DWORD lastErrorCode;

		#pragma endregion
	};
}