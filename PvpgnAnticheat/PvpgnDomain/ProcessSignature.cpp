#include "ProcessSignature.h"

using namespace Domain_Anticheat;

ProcessSignature::ProcessSignature(ProcessInfoOriginal processInfo, int id, bool positive) : 
					Signature(processInfo.Name, processInfo.Hash, id, positive),
						processInfo(processInfo)
{
	enumeration = Process;
}

ProcessSignature::ProcessSignature(const ProcessSignature &signature) : 
					Signature(signature),
						processInfo(signature.processInfo)
{
	enumeration = Process;
}

ProcessSignature::~ProcessSignature(void)
{
}


SignatureEnumeration ProcessSignature::SignatureEnumeration()
{
	return enumeration;
}

bool ProcessSignature::TriggerSignatureByValue(const ProcessInfoOriginal &value) const
{
	if (positive && value.IsEmpty())
	{
		return true;
	}
	if (!positive && !value.IsEmpty())
	{
		return true;
	}
	if (positive && processInfo == value)
	{
		return false;
	}
	if (!positive && !processInfo.PartiallyEquals(value))
	{
		return true;
	}

	return false;
}

bool ProcessSignature::IsEmpty() const
{
	return processInfo.IsEmpty() &&
			Signature::IsEmpty();
}

const ProcessInfoOriginal* ProcessSignature::GetProcessInfo() const
{
	const ProcessInfoOriginal* procInfo = &processInfo;

	return procInfo;
}

wstring ProcessSignature::Value()
{
	return processInfo.ExecutionFileInfo.Hash;
}

wstring ProcessSignature::Value() const
{
	return processInfo.ExecutionFileInfo.Hash;
}

wstring ProcessSignature::ToWstring()
{
	WstringStream() << "Name: " << Name() << " Value: " << Value() << " Enumeration: " << Enumeration() << " Id: " << Id() << " Positive: " << Positive();

	return WstreamBuffer();
}