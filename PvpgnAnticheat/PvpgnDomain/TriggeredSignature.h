#pragma once

#include "Signature.h"

using namespace Domain;

namespace Domain_Anticheat
{
	class TriggeredSignature : public Signature
	{
		public:
			TriggeredSignature() : Signature(wstring(), wstring(), 0) { }
			TriggeredSignature(Signature signature) : Signature(signature), timeStamp(0) { }
			TriggeredSignature(Signature signature, unsigned long timeStamp) : Signature(signature), timeStamp(timeStamp) { }

			TriggeredSignature(const TriggeredSignature &value) : Signature(value)
			{ 
				timeStamp = value.timeStamp;
			}
						
			virtual ~TriggeredSignature(void) { }

			unsigned long TimeStamp(void) const { return timeStamp; }

			bool IsEmpty() const { return timeStamp == 0 && Signature::IsEmpty(); }

			TriggeredSignature operator= (const TriggeredSignature &right) const
			{
				TriggeredSignature r(right);
				return r;
			}

		protected:
			unsigned long timeStamp;
	};
}