#pragma once

#include "../PvpgnDomain/ProcessWorker.h"
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

using namespace Domain_AbstractLayers;

namespace Infrastructure
{
	class ProcessWorkerImpl : public ProcessWorker
	{
		public:
			ProcessWorkerImpl();

			virtual ~ProcessWorkerImpl();

			void** MakeProcessesSnapshot(void);

			void ClearProcessesSnapshot(void** &processesEntries);

			void* FindProcessEntryInSnapshotByName(const wstring &procName) const;

			wstring GetProcessEntryExecutionPath(void* &processEntry, unsigned long retakeCount = 5);

		private:
			void ReleaseProcessHandle(void);

			void InitializeProcessesWorkflow(void);

			void RetakeProcessesSnapshot(void);

			unsigned long actualProcessesSnapshotLength;

			HANDLE procHandle;

			LPPROCESSENTRY32* processesSnapshot;
	};
}