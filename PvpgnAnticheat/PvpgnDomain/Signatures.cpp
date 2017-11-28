#include "Signatures.h"
#include "Signature.h"

using namespace Domain_Anticheat;
using namespace std;

Signatures::Signatures(int id) : Entity(id)
{
}

Signatures::~Signatures(void)
{
}

bool Signatures::AddFileSignature(FileSignature fileSignature)
{
	try
	{
		fileSignatures.insert(make_pair(fileSignature.Name(), fileSignature));

		return true;
	}
	catch (exception ex)
	{
		return false;
	}
}

bool Signatures::AddProcSignature(ProcessSignature procSignature)
{
	try
	{
		procSignatures.insert(make_pair(procSignature.Name(), procSignature));

		return true;
	}
	catch (exception ex)
	{
		return false;
	}
}

const FileSignature* Signatures::GetFileSignature(wstring name)
{
	map<wstring, FileSignature>::iterator it = fileSignatures.find(name);

	if (it != fileSignatures.end())
	{
		return &it->second;
	}

	return NULL;
}

ProcessSignature* Signatures::GetProcSignature(wstring name)
{
	map<wstring, ProcessSignature>::iterator it = procSignatures.find(name);

	if (it != procSignatures.end())
	{
		return &it->second;
	}

	return NULL;
}

wstring Signatures::ToWstring()
{
	ClearWstringStream();

	for(map<wstring, FileSignature>::iterator it = fileSignatures.begin(); it != fileSignatures.end(); it++)
	{
		WstringStream(false) << it->second.ToWstring() << endl;
	}

	for(map<wstring, ProcessSignature>::iterator it = procSignatures.begin(); it != procSignatures.end(); it++)
	{
		WstringStream(false) << it->second.ToWstring() << endl;
	}

	return WstreamBuffer();
}

const FileSignature** Signatures::GetAllFileSignatures(unsigned long &count) const
{
	const FileSignature **fileS = new const FileSignature*[fileSignatures.size()];
	const FileSignature **i = fileS;
	count = 0;

	for(map<wstring, FileSignature>::const_iterator it = fileSignatures.begin(); it != fileSignatures.end(); it++, i++)
	{
		*i = &it->second;
		count++;
	}

	return fileS;
}

const ProcessSignature** Signatures::GetAllProcessSignatures(unsigned long &count) const
{
	const ProcessSignature **procS = new const ProcessSignature*[procSignatures.size()];
	const ProcessSignature **i = procS;
	count = 0;

	for(map<wstring, ProcessSignature>::const_iterator it = procSignatures.begin(); it != procSignatures.end(); it++, i++)
	{
		*i = &it->second;
		count++;
	}

	return procS;
}

unsigned long Signatures::Size() const
{
	return sizeof(FileSignature) * fileSignatures.size() + sizeof(ProcessSignature) * procSignatures.size();
}

unsigned long Signatures::Count() const
{
	return fileSignatures.size() + procSignatures.size();
}