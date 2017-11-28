#include "BrokenRulesDetectorImpl.h"
#include "../PvpgnSelfServices/ServiceContainer.h"
#include <Windows.h>

#ifdef PREV_OR_EQUAL_WIN_7
#include <WinBase.h>
#elif WIN8
#include <Processthreadsapi.h>
#endif

#include "../PvpgnSelfServices/LogManagerImpl.h"

using namespace Infrastructure;
using namespace SelfServices;

unsigned long _stdcall VerifySignature(void *data);

BrokenRulesDetectorImpl::BrokenRulesDetectorImpl()
{
	rulesBraker = (RulesBraker*)ServiceContainer::GetImpl(typeid(RulesBraker).name());

	analyzedSignaturesCount = 0;
	currentThreadId = 0;
	currentAnalysisThreadHandle = NULL;
}

BrokenRulesDetectorImpl::~BrokenRulesDetectorImpl()
{
}

void BrokenRulesDetectorImpl::BeginAnalysis()
{
	currentAnalysisThreadHandle = CreateThread(NULL, NULL, VerifySignature, rulesBraker, NULL, &currentThreadId);

	if (currentAnalysisThreadHandle != NULL)
	{
		LogManagerImpl::GetLogger(*this)->LogInfo("Begin analysis of signatures. Signatures to process: %ul", rulesBraker->TotalSignatures());
	}
	else
	{
		LogManagerImpl::GetLogger(*this)->LogError("Cannot create \"broken rules detector\" thread. Code: %ul", GetLastError());
		throw new exception("Cannot create \"broken rules detector\" thread");
	}
}

unsigned long BrokenRulesDetectorImpl::Progress() const
{
	return rulesBraker->ProcessedSignatures();
}

unsigned long BrokenRulesDetectorImpl::Count() const
{
	return rulesBraker->TotalSignatures();
}

void BrokenRulesDetectorImpl::EndAnalysis()
{
	LogManagerImpl::GetLogger(*this)->LogInfo("Going to terminate signature analysis thread.");

	HANDLE h = GetAnalysisThreadHandle();

	if (h != NULL && !IsFinished())
	{
		if (TerminateThread(h, 1) == TRUE)
		{
			LogManagerImpl::GetLogger(*this)->LogWarn("Work of signature analysis thread was forcebly interrupted.");
		}
		else
		{
			LogManagerImpl::GetLogger(*this)->LogError("Cannot terminate signature analysis thread. Code %ul", GetLastError());
		}
	}

	FreeAnalysisThreadHandle(h);
}

bool BrokenRulesDetectorImpl::HasViolations() const
{
	return GetNotEmptySignaturesList().size() > 0;
}

const vector<TriggeredSignature> BrokenRulesDetectorImpl::GetNotEmptySignaturesList() const
{
	return rulesBraker->NotEmptySignatures();
}

void* BrokenRulesDetectorImpl::GetAnalysisThreadHandle() const
{
	return currentAnalysisThreadHandle;
}

bool BrokenRulesDetectorImpl::IsFinished() const
{
	HANDLE h = GetAnalysisThreadHandle();

	if (h != NULL)
	{
		DWORD exitCode;
		GetExitCodeThread(h, &exitCode);

		return exitCode == 0 || exitCode == 1;
	}

	return true;
}

#pragma region private

void BrokenRulesDetectorImpl::FreeAnalysisThreadHandle(HANDLE h)
{
	if (h != NULL && h == currentAnalysisThreadHandle)
	{
		CloseHandle(h);
		currentAnalysisThreadHandle = NULL;
	}
}

#pragma endregion

#pragma region Global

unsigned long _stdcall VerifySignature(void *data)
{
	RulesBraker* rB = (RulesBraker*)data;
	SignatureVerifier *signatureVerifier = (SignatureVerifier*)ServiceContainer::GetImpl(typeid(SignatureVerifier).name());

	unsigned long fSCount;
	const FileSignature **fileS = rB->GetSignatures()->GetAllFileSignatures(fSCount);
	const FileSignature **fI = fileS;

	unsigned long pSCount;
	const ProcessSignature **procS = rB->GetSignatures()->GetAllProcessSignatures(pSCount);
	const ProcessSignature **pI = procS;

	unsigned long processedFilesSigs = 0;
	unsigned long processedProcsSigs = 0;

	TriggeredSignature currentTrSig;

	while(rB->ProcessedSignatures() != rB->TotalSignatures())
	{
		if (processedFilesSigs < fSCount)
		{
			TriggeredSignature s = signatureVerifier->VerifyFileSignature(**fI);
			rB->AddSignature(s);

			fI++;
			processedFilesSigs++;
		}

		if (processedProcsSigs < pSCount)
		{
			TriggeredSignature s = signatureVerifier->VerifyProcessSignature(**pI);
			rB->AddSignature(s);

			pI++;
			processedProcsSigs++;
		}
	}
	delete [] fileS;
	delete [] procS;

	ExitThread(0);
	return 0;
}

#pragma endregion