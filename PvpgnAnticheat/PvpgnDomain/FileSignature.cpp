#include "FileSignature.h"

using namespace Domain_Anticheat;

FileSignature::FileSignature() : Signature(wstring(), wstring(), -1)
{
}

FileSignature::FileSignature(FileInfoOriginal fileInfoOriginal, int id, bool positive) : 
					Signature(fileInfoOriginal.Name, fileInfoOriginal.Hash, id, positive), 
						fileInfoOriginal(fileInfoOriginal)
{
	enumeration = File;
}

FileSignature::FileSignature(const FileSignature &signature) : 
					Signature(signature),
						fileInfoOriginal(signature.fileInfoOriginal)
{
	enumeration = File;
}


FileSignature::~FileSignature(void)
{
}

enum SignatureEnumeration FileSignature::SignatureEnumeration()
{
	return enumeration;
}

wstring FileSignature::Value()
{
	return fileInfoOriginal.Hash;
}

wstring FileSignature::Value() const
{
	return fileInfoOriginal.Hash;
}

wstring FileSignature::ToWstring()
{
	WstringStream() << "Name: " << Name() << " Value: " << Value() << " Id: " << id << " Positive: " << positive;

	return WstreamBuffer();
}

bool FileSignature::TriggerSignatureByValue(const FileInfoOriginal &value) const
{
	if (positive && value.IsEmpty())
	{
		return true;
	}
	if (!positive && !value.IsEmpty())
	{
		return true;
	}
	if (positive && fileInfoOriginal == value)
	{
		return false;
	}
	if (!positive && !fileInfoOriginal.PartiallyEquals(value))
	{
		return true;
	}

	return false;
}

const FileInfoOriginal* FileSignature::GetFileInfo() const
{
	return &fileInfoOriginal;
}

bool FileSignature::IsEmpty() const
{
	return fileInfoOriginal.IsEmpty() && Signature::IsEmpty();
}