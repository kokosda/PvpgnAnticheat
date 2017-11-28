#pragma once

#include "../PvpgnDomain/SignatureVerifier.h"
#include "FileWorkerImpl.h"
#include "../PvpgnDomain/RulesBraker.h"
#include <TlHelp32.h>
#include "../PvpgnDomain/HashingService.h"
#include "../PvpgnDomain/FileWorker.h"
#include "../PvpgnDomain/ProcessWorker.h"

#include <Windows.h>

using namespace Domain_AbstractLayers;
using namespace Domain_Clients;

namespace Infrastructure
{
	class SignatureVerifierImpl : public SignatureVerifier
	{
		public:
			SignatureVerifierImpl();

			virtual ~SignatureVerifierImpl(void);

			TriggeredSignature VerifyFileSignature(const FileSignature &fileSignature);

			TriggeredSignature VerifyProcessSignature(const ProcessSignature &processSignature);

		private:
			TriggeredSignature VerifyAsPositiveFileSignature(FileSignature *fileSignature);

			TriggeredSignature VerifyAsNegativeFileSignature(FileSignature *fileSignature);

			TriggeredSignature VerifyAsPositiveProcessSignature(ProcessSignature *processSignature);

			TriggeredSignature VerifyAsNegativeProcessSignature(ProcessSignature *processSignature);

			const FileInfoOriginal MakeActualFileInfo(const FileSignature &fileSignatureSource, unsigned long computedSize, const wstring &computedHash);

			const ProcessInfoOriginal MakeActualProcessInfo(const ProcessSignature &procSignatureSource);

			unsigned long GetTimeStamp() const;

			FileWorker *fileWorker;

			HashingService *hashingService;

			ProcessWorker *processWorker;

			wstring verificationWarningPrefix;
			wstring verificationPassedPrefix;
	};
}