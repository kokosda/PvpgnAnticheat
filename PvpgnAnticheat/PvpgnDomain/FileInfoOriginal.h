#pragma once

#include <string>
using namespace std;

namespace Domain_Anticheat
{
	struct FileInfoOriginal
	{
		FileInfoOriginal() : Name(wstring()), Hash(wstring()), OriginalSize(0), FullRelativePath(wstring()) { }

		FileInfoOriginal(wstring name, wstring hash) : Name(name), Hash(hash) { OriginalSize = 0; FullRelativePath = wstring(); }

		FileInfoOriginal(wstring name, wstring hash, unsigned long size) : Name(name), Hash(hash), OriginalSize(size) { FullRelativePath = wstring(); }

		FileInfoOriginal(wstring name, wstring hash, unsigned long size, wstring relativePath) : Name(name), Hash(hash), OriginalSize(size), FullRelativePath(relativePath) { }

		FileInfoOriginal(const FileInfoOriginal &fileInfoOriginal) : 
					Name(fileInfoOriginal.Name), 
					Hash(fileInfoOriginal.Hash), 
					FullRelativePath(fileInfoOriginal.FullRelativePath), 
					OriginalSize(fileInfoOriginal.OriginalSize) { }

		bool operator== (const FileInfoOriginal &right) const
		{ 
			return Name.compare(right.Name) == 0 &&
					Hash.compare(right.Hash) == 0 &&
					OriginalSize == right.OriginalSize &&
					FullRelativePath.compare(right.FullRelativePath) == 0;
		}

		FileInfoOriginal operator= (const FileInfoOriginal &right) const
		{
			return FileInfoOriginal(right);
		}

		bool IsEmpty() const
						{ 
							return Name.size() == 0 &&
								   Hash.size() == 0 &&
								   OriginalSize == 0 &&
								   FullRelativePath.size() == 0;
						}

		bool PartiallyEquals(const FileInfoOriginal &value) const
		{
			return Name.compare(value.Name) == 0 || 
					Hash.compare(value.Hash) == 0;
		}		

		unsigned long OriginalSize;

		wstring FullRelativePath;

		const wstring Name;

		const wstring Hash;
	};
}