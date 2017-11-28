#pragma once

#include "FileSignature.h"
#include "ProcessSignature.h"
#include "TriggeredSignature.h"

using namespace Domain_Anticheat;

namespace Domain_AbstractLayers
{
	class SignatureVerifier : public BaseObject
	{
		public:
			virtual TriggeredSignature VerifyFileSignature(const FileSignature &fileSignature) = 0;

			virtual TriggeredSignature VerifyProcessSignature(const ProcessSignature &processSignature) = 0;
	};
}