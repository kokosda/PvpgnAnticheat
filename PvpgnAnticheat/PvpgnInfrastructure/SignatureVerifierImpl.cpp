#include "SignatureVerifierImpl.h"
#include "../PvpgnSelfServices/LogManagerImpl.h"
#include <Psapi.h>
#include "../PvpgnSelfServices/ServiceContainer.h"
#include "../PvpgnDomain/NonTriggeredSignature.h"

using namespace Infrastructure;
using namespace SelfServices;

SignatureVerifierImpl::SignatureVerifierImpl()
{
	fileWorker = (FileWorker*)ServiceContainer::GetImpl(typeid(FileWorker).name());
	processWorker = (ProcessWorker*)ServiceContainer::GetImpl(typeid(ProcessWorker).name());
	hashingService = (HashingService*)ServiceContainer::GetImpl(typeid(HashingService).name());

	verificationWarningPrefix = L"SIGNATURE VERIFICATION FAILED";
	verificationPassedPrefix = L"SIGNATURE VERIFICATION PASSED";
}

SignatureVerifierImpl::~SignatureVerifierImpl(void)
{
}

TriggeredSignature SignatureVerifierImpl::VerifyFileSignature(const FileSignature &fS)
{
	FileSignature fileSignature(fS);
	LogManagerImpl::GetLogger(*this)->LogInfo("Verifying signature %s", fileSignature.ToWstring().c_str());

	if (fileSignature.Positive())
	{
		TriggeredSignature s(VerifyAsPositiveFileSignature(&fileSignature));
		return s;
	}
	else
	{
		return VerifyAsNegativeFileSignature(&fileSignature);
	}
}

TriggeredSignature SignatureVerifierImpl::VerifyProcessSignature(const ProcessSignature &pS)
{
	ProcessSignature procSignature(pS);
	LogManagerImpl::GetLogger(*this)->LogInfo("Verifying signature %s", procSignature.ToWstring().c_str());
	ProcessInfoOriginal actualProcInfo = MakeActualProcessInfo(procSignature);

	if (procSignature.TriggerSignatureByValue(actualProcInfo))
	{
		LogManagerImpl::GetLogger(*this)->LogWarn("%s: suspicious process %s detected", verificationWarningPrefix.c_str(), procSignature.Name().c_str());
		return TriggeredSignature(procSignature, GetTimeStamp());
	}

	LogManagerImpl::GetLogger(*this)->LogInfo("%s: process %s was not found", verificationPassedPrefix.c_str(), procSignature.Name().c_str());
	return NonTriggeredSignature(procSignature);
}

#pragma region private

TriggeredSignature SignatureVerifierImpl::VerifyAsPositiveFileSignature(FileSignature *fileSignature)
{
	LogManagerImpl::GetLogger(*this)->LogInfo("Verifying as positive file signature.");

	wchar_t *concatResult = new wchar_t[MAX_PATH];
	int concatResultSize = fileWorker->ConcatPathes(fileSignature->GetFileInfo()->Name, fileSignature->GetFileInfo()->FullRelativePath, concatResult);
	wstring targetPath(concatResult);

	delete [] concatResult;

	LogManagerImpl::GetLogger(*this)->LogInfo("Going to read file %s", targetPath.c_str());

	unsigned char* fileBytes;
	unsigned long fileLength = fileWorker->GetContent(targetPath, fileBytes);
	const unsigned char* imutableFileBytes = fileBytes;

	bool fileExists = fileBytes != NULL;

	if (fileExists)
	{
		wstring computedHash;
		LogManagerImpl::GetLogger(*this)->LogInfo("File %s read successfully. Starting of hashing it.", targetPath.c_str());

		computedHash = hashingService->GetHash(imutableFileBytes, fileLength);
		delete [] fileBytes;

		LogManagerImpl::GetLogger(*this)->LogInfo("File %s hashed: %s", targetPath.c_str(), computedHash.c_str());

		FileInfoOriginal actualFileInfo = MakeActualFileInfo(*fileSignature, fileLength, computedHash);
				
		if (fileSignature->TriggerSignatureByValue(actualFileInfo))
		{
			LogManagerImpl::GetLogger(*this)->LogInfo("%s: signature %s did not pass verification.", verificationWarningPrefix.c_str(), fileSignature->ToWstring().c_str());
			return TriggeredSignature(*fileSignature, GetTimeStamp());
		}

		LogManagerImpl::GetLogger(*this)->LogInfo("%s: signature %s passed verification successfully", verificationPassedPrefix.c_str(), fileSignature->ToWstring().c_str());
		return NonTriggeredSignature(*fileSignature);
	}
	else
	{
		LogManagerImpl::GetLogger(*this)->LogInfo("%s: required file %s was not found in target directory.", verificationWarningPrefix.c_str(), targetPath.c_str());
		return TriggeredSignature(*fileSignature, GetTimeStamp());
	}
}

TriggeredSignature SignatureVerifierImpl::VerifyAsNegativeFileSignature(FileSignature *fileSignature)
{
	LogManagerImpl::GetLogger(*this)->LogInfo("Verifying as negative file signature.");

	wchar_t *concatResult = new wchar_t[MAX_PATH];
	int concatResultSize = fileWorker->ConcatPathes(fileSignature->GetFileInfo()->Name, fileSignature->GetFileInfo()->FullRelativePath, concatResult);
	wstring targetPath(concatResult);

	delete [] concatResult;

	LogManagerImpl::GetLogger(*this)->LogInfo("Going to read file %s", targetPath.c_str());

	unsigned char* fileBytes;
	unsigned long fileLength = fileWorker->GetContent(targetPath, fileBytes);
	
	if (!fileBytes)
	{
		LogManagerImpl::GetLogger(*this)->LogInfo("%s: suspicious file %s was not found", verificationPassedPrefix.c_str(), targetPath.c_str());
		return NonTriggeredSignature(*fileSignature);
	}

	LogManagerImpl::GetLogger(*this)->LogWarn("%s: suspicious file %s detected in game directory.", verificationWarningPrefix.c_str(), targetPath.c_str());
	return TriggeredSignature(*fileSignature, GetTimeStamp());
}

const FileInfoOriginal SignatureVerifierImpl::MakeActualFileInfo(const FileSignature &fileSignatureSource, unsigned long computedSize, const wstring &computedHash)
{
	const FileInfoOriginal* sourceFio = fileSignatureSource.GetFileInfo();

	return FileInfoOriginal(sourceFio->Name, computedHash, computedSize, sourceFio->FullRelativePath);
}

const ProcessInfoOriginal SignatureVerifierImpl::MakeActualProcessInfo(const ProcessSignature &procSignature)
{
	void* procEntry = processWorker->FindProcessEntryInSnapshotByName(procSignature.Name());

	if (procEntry)
	{
		LogManagerImpl::GetLogger(*this)->LogInfo("Process %s found", procSignature.Name().c_str());

		wstring procPath = processWorker->GetProcessEntryExecutionPath(procEntry, 1);
		const ProcessInfoOriginal *sourcePio = procSignature.GetProcessInfo();
		
		unsigned char* fileBytes;
		unsigned long fileLength = fileWorker->GetContent(procPath, fileBytes);

		wstring exFileHash = hashingService->GetHash(fileBytes, fileLength);

		delete [] fileBytes;

		ProcessInfoOriginal res(sourcePio->Name, wstring(), FileInfoOriginal(sourcePio->ExecutionFileInfo.Name, exFileHash, fileLength));

		return res;
	}

	LogManagerImpl::GetLogger(*this)->LogInfo("Process %s was not found", procSignature.Name().c_str());
	return ProcessInfoOriginal();
}

unsigned long SignatureVerifierImpl::GetTimeStamp() const
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	return localTime.wYear + localTime.wMonth + localTime.wDay + localTime.wHour + localTime.wMinute + localTime.wSecond + localTime.wMilliseconds;
}

#pragma endregion