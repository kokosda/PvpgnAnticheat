#pragma once

#include "TriggeredSignature.h"

using namespace Domain_Anticheat;

namespace Domain_Anticheat
{
	class NonTriggeredSignature : public TriggeredSignature
	{
		public:
			NonTriggeredSignature(const TriggeredSignature &triggeredSignature)
			{
				id = triggeredSignature.Id();
				name = triggeredSignature.Name();
				value = wstring();
				timeStamp = triggeredSignature.TimeStamp();
				enumeration = triggeredSignature.Enumeration();
			}

			NonTriggeredSignature(const Signature &signature)
			{
				id = signature.Id();
				name = signature.Name();
				value = wstring();
				timeStamp = 0;
				enumeration = signature.Enumeration();
			}
	};
}