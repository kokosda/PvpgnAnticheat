#pragma once

#include "../PvpgnDomain/BrokenRulesDetector.h"
#include "../PvpgnDomain/SignatureVerifier.h"
#include <Windows.h>

using namespace Domain_AbstractLayers;

namespace Infrastructure
{
	class BrokenRulesDetectorImpl : public BrokenRulesDetector
	{
		public:
			BrokenRulesDetectorImpl();

			virtual ~BrokenRulesDetectorImpl();

			void BeginAnalysis();

			unsigned long Progress() const;

			unsigned long Count() const;

			void EndAnalysis();

			bool HasViolations() const;

			const vector<TriggeredSignature> GetNotEmptySignaturesList() const;


			bool IsFinished() const;

		protected:
			void* GetAnalysisThreadHandle() const;

		private:
			void FreeAnalysisThreadHandle(HANDLE h);

			RulesBraker* rulesBraker;
			unsigned long analyzedSignaturesCount;
			vector<TriggeredSignature> triggeredSignatures;
			unsigned long currentThreadId;
			HANDLE currentAnalysisThreadHandle;
	};
}