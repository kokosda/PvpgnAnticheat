#pragma once

#include "Entity.h"
#include "FileSignature.h"
#include "ProcessSignature.h"

#include <map>

using namespace std;
using namespace Domain;

namespace Domain_Anticheat
{
	class Signatures : public Entity
	{
		public:
			Signatures(int id);

			virtual ~Signatures(void);

			bool AddFileSignature(FileSignature fileSignature);

			bool AddProcSignature(ProcessSignature procSignature);

			const FileSignature* GetFileSignature(wstring name);

			ProcessSignature* GetProcSignature(wstring name);

			wstring ToWstring();

			const FileSignature** GetAllFileSignatures(unsigned long &count) const;

			const ProcessSignature** GetAllProcessSignatures(unsigned long &count) const;

			unsigned long Size() const;

			unsigned long Count() const;
		private:
			map<wstring, FileSignature> fileSignatures;

			map<wstring, ProcessSignature> procSignatures;
	};
}