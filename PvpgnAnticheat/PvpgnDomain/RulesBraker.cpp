#include "RulesBraker.h"

using namespace Domain_Clients;

RulesBraker::RulesBraker(const Client *client, Signatures *signatures) : 
					signatures(signatures), client(client)
{
}

RulesBraker::~RulesBraker()
{
}

void RulesBraker::AddSignature(TriggeredSignature triggeredSignature)
{
	if (ProcessedSignatures() < TotalSignatures())
	{
		triggeredSignatures.push_back(triggeredSignature);
	}
}

Signatures* RulesBraker::GetSignatures()
{
	return signatures;
}

Signatures* RulesBraker::GetSignatures() const
{
	return signatures;
}

const Client* RulesBraker::GetClient() const
{
	return client;
}

unsigned long RulesBraker::ProcessedSignatures() const
{
	return triggeredSignatures.size();
}

unsigned long RulesBraker::TotalSignatures() const
{
	return signatures->Count();
}

vector<TriggeredSignature> RulesBraker::NotEmptySignatures() const
{
	vector<TriggeredSignature> res;

	for(vector<TriggeredSignature>::const_iterator it = triggeredSignatures.begin(); it != triggeredSignatures.end(); it++)
	{
		if (!it->IsEmpty())
		{
			res.push_back(*it);
		}
	}

	return res;
}