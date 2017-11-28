#include "FileWorkerImpl.h"
#include "../PvpgnSelfServices/LogManagerImpl.h"

using namespace Infrastructure;
using namespace SelfServices;


FileWorkerImpl::FileWorkerImpl(void)
{
}


FileWorkerImpl::~FileWorkerImpl(void)
{
}

#pragma region FileWorker

wstring FileWorkerImpl::ExecutionDirectory(bool get, wstring directory)
{
	if (get)
	{
		WCHAR buffer[MAX_ABSOLUTE_PATH];
		DWORD resultLength = GetCurrentDirectory(MAX_ABSOLUTE_PATH, buffer);

		wstring result(buffer, resultLength);
		
		return result;
	}
	else if (!get && directory.size() > 0)
	{
		if (directory.size() <= MAX_PATH)
		{
			if (SetCurrentDirectory(directory.c_str()) != TRUE)
			{
				LogManagerImpl::GetLogger(*this)->LogFatal("Cannot set current execution directory. Code %u", GetLastError());

				throw new exception("Cannot set current execution directory. See log for details");
			}
		}
		else
		{
			throw new exception("Max directory length value exceeded");
		}
	}

	return ExecutionDirectory();
}

unsigned long FileWorkerImpl::GetSize(wstring fileName)
{
	HANDLE file = Open(fileName);
	
	if (file != NULL)
	{
		unsigned long size = GetSize(file);

		Close(currentFile);
	
		return size;
	}

	return 0;
}

unsigned long FileWorkerImpl::GetContent(wstring fileName, unsigned char* &result)
{
	unsigned long size = GetSize(fileName);

	if (size != 0)
	{
		result = new unsigned char[size];
	
		HANDLE file = Open(fileName);

		if (file != NULL)
		{
			BOOL readResult = Read(file, result, size);

			Close(file);

			return size;
		}
	}
	else
	{
		result = NULL;
	}

	return 0;
}

void FileWorkerImpl::WriteLine(string fileName, string line)
{
	throw new exception("Not implemented!");
}

int FileWorkerImpl::ConcatPathes(const wstring &path1, const wstring &path2, wchar_t* &result)
{
	if (path1.size() + path2.size() <= MAX_PATH)
	{
		if (PathCombine(result, path1.c_str(), path2.c_str()) != NULL)
		{			
			return wcslen(result);
		}

		return 0;
	}
	else
	{
		WstringStream() << "Cannot concatenate 2 pathes [" << path1 << "; " << path2 << "] due to max path length exceeded";

		throw new exception(ToString(WstreamBuffer()).c_str());
	}
}

int FileWorkerImpl::GetDirectory(const wstring &path, wchar_t* &result)
{	
	wchar_t *fileRes;

	DWORD length = GetFullPathName(path.c_str(), path.size() + 2, result, &fileRes);
	wcsncpy_s(result, wcslen(result) + 1, result, wcslen(result) - wcslen(fileRes));
	
	if (length > 0)
	{
		return wcslen(result);
	}

	LogManagerImpl::GetLogger(*this)->LogError("Cannot get directory from path: %s. Error code: %u", path, GetLastError());

	throw new exception("Cannot get directory for specified path");
}

#pragma endregion

#pragma region private

HANDLE FileWorkerImpl::Open(wstring fileName)
{
	if (fileName.length() <= MAX_PATH)
	{
		currentFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		
		lastErrorCode = GetLastError();

		if (currentFile != INVALID_HANDLE_VALUE && lastErrorCode != ERROR_FILE_NOT_FOUND && lastErrorCode != ERROR_SHARING_VIOLATION)
		{
			return currentFile;
		}
		else
		{
			LogManagerImpl::GetLogger(*this)->LogError("Unable to open file %s", fileName.c_str());
		}
	}

	return NULL;
}

bool FileWorkerImpl::Read(HANDLE file, unsigned char* &result, unsigned long fileSize)
{
	if (fileSize <= MAX_FILE_SIZE)
	{
		DWORD bytesRead;
		BOOL readFileSuccess = false;
		
		readFileSuccess = ReadFile(file, result, fileSize, &bytesRead, NULL);

		return (fileSize == bytesRead) && (readFileSuccess != NULL);
	}

	return false;
}

BOOL FileWorkerImpl::Close(HANDLE file)
{
	return CloseHandle(file);
}

unsigned long FileWorkerImpl::GetSize(HANDLE file)
{
	LARGE_INTEGER bigSize;

	BOOL result = GetFileSizeEx(file, &bigSize);

	if (result)
	{
		return bigSize.LowPart;
	}

	return 0;
}

#pragma endregion