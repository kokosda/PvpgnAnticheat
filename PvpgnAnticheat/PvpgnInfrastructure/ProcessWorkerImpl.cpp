#include "ProcessWorkerImpl.h"
#include "../PvpgnSelfServices/LogManagerImpl.h"

using namespace Infrastructure;
using namespace SelfServices;

ProcessWorkerImpl::ProcessWorkerImpl()
{
	procHandle = NULL;
	processesSnapshot = NULL;
	actualProcessesSnapshotLength = 0;

	InitializeProcessesWorkflow();
}

ProcessWorkerImpl::~ProcessWorkerImpl()
{
	void** pS = (void**)processesSnapshot;
	ReleaseProcessHandle();
	ClearProcessesSnapshot(pS);
}

void** ProcessWorkerImpl::MakeProcessesSnapshot(void)
{
	LPPROCESSENTRY32 *procSnapshot = new LPPROCESSENTRY32[1024];
	LPPROCESSENTRY32 *procIndexer = procSnapshot;

	(*procIndexer) = new PROCESSENTRY32();
	(*procIndexer)->dwSize = sizeof(PROCESSENTRY32);
		
	if (Process32First(procHandle, *procIndexer) != TRUE)
	{
		LogManagerImpl::GetLogger(*this)->LogError("Cannot obtain first process entry. Code: %s", GetLastError());
		throw new exception("Cannot obtain first process entry");
	}

	actualProcessesSnapshotLength++;

	do
	{
		procIndexer++;
		(*procIndexer) = new PROCESSENTRY32();
		(*procIndexer)->dwSize = sizeof(PROCESSENTRY32);

		actualProcessesSnapshotLength++;
	}
	while (Process32Next(procHandle, *procIndexer) == TRUE);

	return (void**)procSnapshot;
}

void ProcessWorkerImpl::ClearProcessesSnapshot(void** &processesEntries)
{
	LPPROCESSENTRY32 *indexer = (LPPROCESSENTRY32*)processesEntries;
	unsigned long till = actualProcessesSnapshotLength;

	while(till > 0)
	{
		delete (*indexer);

		indexer++;
		till--;
	}

	if (processesEntries)
	{
		delete [] processesEntries;
		processesEntries = NULL;
	}

	actualProcessesSnapshotLength = 0;
}

void* ProcessWorkerImpl::FindProcessEntryInSnapshotByName(const wstring &procName) const
{
	LPPROCESSENTRY32 *indexer = processesSnapshot;
	unsigned long till = actualProcessesSnapshotLength;
	wstring loweredProcName = ToWLower(procName);
	
	while(till > 0)
	{
		if (ToWLower((*indexer)->szExeFile).compare(loweredProcName) == 0)
		{
			return *indexer;
		}

		indexer++;
		till--;
	}

	return NULL;
}

wstring ProcessWorkerImpl::GetProcessEntryExecutionPath(void* &processEntry, unsigned long retakeCount)
{
	wchar_t *buff = new wchar_t[MAX_PATH];
	unsigned long buffSize = MAX_PATH;
	LPPROCESSENTRY32 procEntry = (LPPROCESSENTRY32)processEntry;
	HANDLE prHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procEntry->th32ProcessID);

	if (prHandle && retakeCount >= 0)
	{
		DWORD fileLength = GetModuleFileNameEx(prHandle, NULL, buff, buffSize);
		if (fileLength > 0)
		{
			wstring res(buff);
			delete [] buff;
			return res;
		}

		LogManagerImpl::GetLogger(*this)->LogError("Cannot obtain execution file info for process entry %s. Process Id: %s. Code: %s", 
														procEntry->szExeFile, procEntry->th32ProcessID, GetLastError());
		
		delete [] buff;
		throw new exception("Cannot obtain execution file info for process entry");
	}
	else if (retakeCount < 0)
	{
		delete [] buff;
		return wstring();
	}
	else if (!prHandle)
	{
		wstring tmpName(procEntry->szExeFile);
		RetakeProcessesSnapshot();
		procEntry = (LPPROCESSENTRY32)FindProcessEntryInSnapshotByName(tmpName);
		
		delete [] buff;
		return GetProcessEntryExecutionPath(processEntry, --retakeCount);
	}

	delete [] buff;
	return wstring();
}

#pragma region private

void ProcessWorkerImpl::ReleaseProcessHandle(void)
{
	if (procHandle)
	{
		CloseHandle(procHandle);
		procHandle = NULL;
	}
}

void ProcessWorkerImpl::InitializeProcessesWorkflow(void)
{
	void** pS = (void**)processesSnapshot;
	ReleaseProcessHandle();
	ClearProcessesSnapshot(pS);

	procHandle = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		
	if (procHandle == INVALID_HANDLE_VALUE)
	{
		LogManagerImpl::GetLogger(*this)->LogFatal("Cannot take processes snapshot. Code: %s", GetLastError());
		throw new exception("Cannot take processes snapshot.");
	}

	processesSnapshot = (LPPROCESSENTRY32*)MakeProcessesSnapshot();
}

void ProcessWorkerImpl::RetakeProcessesSnapshot(void)
{
	InitializeProcessesWorkflow();
}

#pragma endregion