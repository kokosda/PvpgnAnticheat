#pragma once

#include "Signature.h"
#include "ProccessInfoOriginal.h"

namespace Domain_Anticheat
{
	class ProcessSignature : public Signature
	{
		public:
			ProcessSignature(ProcessInfoOriginal processInfo, int id, bool positive = true);

			ProcessSignature(const ProcessSignature &signature);

			virtual ~ProcessSignature(void);			

			enum SignatureEnumeration SignatureEnumeration();

			bool IsEmpty() const;

			bool TriggerSignatureByValue(const ProcessInfoOriginal &value) const;

			const ProcessInfoOriginal* GetProcessInfo() const;

			wstring Value();

			wstring Value() const;

			wstring ToWstring();

		private:
			const ProcessInfoOriginal processInfo;
	};
}
