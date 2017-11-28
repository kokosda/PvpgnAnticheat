#pragma once

#include "Signature.h"
#include "FileInfoOriginal.h"

namespace Domain_Anticheat
{
	class FileSignature : public Signature
	{
		public:
			FileSignature();

			FileSignature(FileInfoOriginal fileInfoOriginal, int id, bool positive = true);

			FileSignature(const FileSignature &signature);

			virtual ~FileSignature(void);			

			enum SignatureEnumeration SignatureEnumeration();

			wstring Value();

			wstring Value() const;

			wstring ToWstring();

			bool TriggerSignatureByValue(const FileInfoOriginal &value) const;

			const FileInfoOriginal* GetFileInfo() const;

			bool IsEmpty() const;
		private:
			const FileInfoOriginal fileInfoOriginal;
	};
}