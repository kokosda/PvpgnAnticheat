#pragma once

#include "../BaseObjects/BaseObject.h"

using namespace std;
using namespace BaseObjects;

namespace Domain_AbstractLayers
{
	class ProcessWorker : public BaseObject
	{
		public:
			virtual void** MakeProcessesSnapshot(void) = 0;

			virtual void ClearProcessesSnapshot(void** &processesEntries) = 0;

			virtual void* FindProcessEntryInSnapshotByName(const wstring &procName) const = 0;

			virtual wstring GetProcessEntryExecutionPath(void* &processEntry, unsigned long retakeCount = 5) = 0;
	};
}