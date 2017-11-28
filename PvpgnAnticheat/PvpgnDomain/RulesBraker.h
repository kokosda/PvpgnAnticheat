#pragma once

#include "Entity.h"
#include "Client.h"
#include "TriggeredSignature.h"
#include "Signatures.h"

#include <vector>

using namespace Domain;
using namespace Domain_Anticheat;

namespace Domain_Clients
{
	class RulesBraker : public BaseObject
	{
		public:
			RulesBraker(const Client *client, Signatures *signatures);

			virtual ~RulesBraker();

			const vector<TriggeredSignature>& TriggeredSignatures() { return triggeredSignatures; }

			void AddSignature(TriggeredSignature triggeredSignature);

			Signatures* GetSignatures();

			Signatures* GetSignatures() const;

			const Client* GetClient() const;

			unsigned long ProcessedSignatures() const;

			unsigned long TotalSignatures() const;

			vector<TriggeredSignature> NotEmptySignatures() const;
		private:
			vector<TriggeredSignature> triggeredSignatures;

			const Client *client;

			Signatures *signatures;
	};
}