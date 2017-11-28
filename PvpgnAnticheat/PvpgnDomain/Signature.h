#pragma once

#include "Entity.h"
#include "SignatureEnumeration.h"
#include <map>

using namespace Domain;

namespace Domain_Anticheat
{
	class Signature : public Entity
	{
		public:
			Signature(wstring name, int id, enum SignatureEnumeration enumeration = Simple, bool positive = true);

			Signature(wstring name, wstring value, int id, bool positive = true);

			Signature(const Signature &signature);

			virtual ~Signature(void);

			virtual wstring Name();

			virtual wstring Name() const;

			virtual wstring Value();

			virtual wstring Value() const;

			virtual enum SignatureEnumeration SignatureEnumeration();

			virtual bool TriggerSignatureByValue(const wstring &value) const;

			enum SignatureEnumeration Enumeration() const;

			wstring ToWstring();

			virtual bool IsEmpty() const;

			bool Positive() const;

		protected:
			enum SignatureEnumeration enumeration;

			const bool positive;

			wstring name;

			wstring value;
	};
}