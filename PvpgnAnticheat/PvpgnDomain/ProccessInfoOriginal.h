#pragma once

#include <string>
#include "FileInfoOriginal.h"

using namespace std;

namespace Domain_Anticheat
{
	struct ProcessInfoOriginal
	{
		ProcessInfoOriginal() { }

		ProcessInfoOriginal(wstring name, wstring hash) : Name(name), Hash(hash) { }

		ProcessInfoOriginal(wstring name, wstring hash, const FileInfoOriginal &executionFileInfo) : Name(name), Hash(hash), ExecutionFileInfo(executionFileInfo)  { }

		ProcessInfoOriginal(const ProcessInfoOriginal &value) : Name(value.Name), Hash(value.Hash), ExecutionFileInfo(value.ExecutionFileInfo) { }

		ProcessInfoOriginal operator= (const ProcessInfoOriginal &right) const
		{
			return ProcessInfoOriginal(right);
		}

		bool operator== (const ProcessInfoOriginal &right) const
		{
			return Name.compare(right.Name) == 0 &&
					Hash.compare(right.Hash) == 0 &&
					ExecutionFileInfo == right.ExecutionFileInfo;
		}

		bool IsEmpty() const
		{
			return Name.size() == 0 &&
					Hash.size() == 0 &&
					ExecutionFileInfo.IsEmpty();
		}

		bool PartiallyEquals(const ProcessInfoOriginal &value) const
		{
			return Name.compare(value.Name) == 0 ||
					Hash.compare(value.Hash) == 0 ||
					ExecutionFileInfo.PartiallyEquals(value.ExecutionFileInfo);
		}

		const wstring Name;

		const wstring Hash;

		const FileInfoOriginal ExecutionFileInfo;
	};
}