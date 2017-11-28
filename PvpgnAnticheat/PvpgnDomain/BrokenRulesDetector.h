#pragma once

#include "RulesBraker.h"

using namespace Domain_Anticheat;
using namespace Domain_Clients;

namespace Domain_AbstractLayers
{
	class BrokenRulesDetector : public BaseObject
	{
		public:
			virtual void BeginAnalysis() = 0;

			virtual unsigned long Progress() const = 0;

			virtual unsigned long Count() const = 0;

			virtual void EndAnalysis() = 0;

			virtual bool HasViolations() const = 0;

			virtual const vector<TriggeredSignature> GetNotEmptySignaturesList() const = 0;

			virtual bool IsFinished() const = 0;

		protected:
			virtual void* GetAnalysisThreadHandle() const = 0;
	};
}
