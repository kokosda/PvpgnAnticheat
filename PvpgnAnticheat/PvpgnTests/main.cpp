#include "../PvpgnDomain/Client.h"
#include "../PvpgnDomain/Signatures.h"
#include "../PvpgnDomain/FileSignature.h"
#include "../PvpgnDomain/ProcessSignature.h"
#include "../PvpgnDomain/TriggeredSignature.h"
#include "../PvpgnInfrastructure/FileWorkerImpl.h"
#include "../PvpgnSelfServices/LogManagerImpl.h"
#include "../PvpgnSelfServices/ServiceContainer.h"
#include "../PvpgnDomain/RulesBraker.h"
#include "../PvpgnInfrastructure/SignatureVerifierImpl.h"
#include "../PvpgnInfrastructure/HashingServiceImpl.h"
#include "../PvpgnInfrastructure/ProcessWorkerImpl.h"
#include "../PvpgnInfrastructure/BrokenRulesDetectorImpl.h"

#include <iostream>

using namespace Domain_Clients;
using namespace Domain_Anticheat;
using namespace Domain;
using namespace Infrastructure;
using namespace SelfServices;
using namespace std;

LogManagerImpl* LogManagerImpl::instance;
Logger* LogManagerImpl::loggerInstance;

void SignatureVerificatorTest(const Client &client, Signatures *signatures, const FileSignature &signatureToVerify);

const wstring logFileName = L"anticheat.log";

int main(int argc, char** argv)
{	
	LogManagerImpl::Init(logFileName);
	BaseObject bObj;
	LogManagerImpl::GetLogger(bObj)->Enable(true);

	BaseObject baseObject;
	Entity entity(0);
	Client client(L"\\\\?\\R:\\Games\\Fallout 3\\FalloutLauncher.exe", 1);
		
	Signatures *signatures = new Signatures(2);
	FileInfoOriginal fileInfoOriginal(L"atimgpud.dll", L"", 165304);

	FileSignature fileSignature(fileInfoOriginal, 1, false);
	FileSignature negativeFileSignature(FileInfoOriginal(L"GDFFallout3.dll", L"{t-¦,dHV¯VTý\rX", 409600), 7, true);
	
	ProcessInfoOriginal processInfoOriginal(L"MapHack_for_iccup.exe", L"", FileInfoOriginal(L"MapHack_for_iccup.exe", L"µ6ÇÐéé9:Û'è÷âð§Ç¬", 1315545));

	ProcessSignature processSignature(processInfoOriginal, 2, false);

	signatures->AddFileSignature(fileSignature);
	signatures->AddProcSignature(processSignature);
	signatures->AddFileSignature(negativeFileSignature);

	const FileSignature* signature = signatures->GetFileSignature(L"Diesel.bin");

	TriggeredSignature triggeredSignature(fileSignature, 3948934);
	
	wstring goodFileName = L"GameOfThrones_III.txt";
	wstring badFileName = L"GameOfThrones_IV.txt";

	FileWorker *fileWorker = new FileWorkerImpl();
	ServiceContainer::AddImpl(typeid(FileWorker).name(), fileWorker);

	unsigned char* fileContent = new unsigned char[0];
	unsigned long hashSize = fileWorker->GetContent(goodFileName, fileContent);

	unsigned char* badFileContent = new unsigned char[0];
	fileWorker->GetContent(badFileName, badFileContent);

	cout << "Anticheat test launched..." << endl;

	cout << "Base Object: " << baseObject.ToString() << endl;

	cout << "Entity: " << entity.ToString() << endl;

	wcout << "Client: " << client.ToWstring() << endl;

	wcout << "Signatures: " << endl << signatures->ToWstring() << endl;

	wcout << "Signatures once again: " << endl << signatures->ToWstring() << endl;

	cout << "FileWorker: " << fileWorker->ToString() << endl;

	wcout << "Execution dir: " << fileWorker->ExecutionDirectory() << endl;

	wcout << "Good File " << goodFileName << " size: " << fileWorker->GetSize(goodFileName) << endl;

	if (hashSize > 0)
	{
		cout << "Good File " << BaseObject().ToString(goodFileName) << " hash: " << fileContent << endl;
	}
	else
	{
		cout << "Good File " << BaseObject().ToString(goodFileName) << " hash: " << "error while reading the file or file is empty" << endl;
	}

	wcout << "Bad File " << badFileName << " size: " << fileWorker->GetSize(badFileName) << endl;

	wcout << "Bad File " << badFileName << " hash: " << (badFileContent ? (const char*)badFileContent : " ") << endl;

	cout << endl;

	SignatureVerificatorTest(client, signatures, negativeFileSignature);

	cout << "Signature verification completed" << endl;

	BrokenRulesDetector *brd = new BrokenRulesDetectorImpl();
	RulesBraker *rb = (RulesBraker*)ServiceContainer::GetImpl(typeid(RulesBraker).name());

	cout << "Starting broken rules detector" << endl;

	brd->BeginAnalysis();
	while (!brd->IsFinished())
	{
		cout << "Analyzing progress. Proccessed signatures: " << brd->Progress() << endl;
	}
	brd->EndAnalysis();
	vector<TriggeredSignature> tr = brd->GetNotEmptySignaturesList();

	cout << "Client has signature violations: " << brd->HasViolations() << endl;
	
	system("pause");

	return 0;
}

void SignatureVerificatorTest(const Client &client, Signatures *signatures, const FileSignature &signatureToVerify)
{
	Client copyClient(client);
	FileWorker *fileWorker = (FileWorker*)ServiceContainer::GetImpl(typeid(FileWorker).name());

	const RulesBraker *rulesBraker = new RulesBraker(&client, signatures);
	void *rb = (void*)rulesBraker;
	ServiceContainer::AddImpl(typeid(RulesBraker).name(), rb);

	wstring initialExecDir = fileWorker->ExecutionDirectory();

	wchar_t *absPath = new wchar_t[MAX_PATH];
	fileWorker->ConcatPathes(initialExecDir, logFileName, absPath);

	LogManagerImpl::GetLogger()->DestinationFile(absPath);

	delete [] absPath;

	wchar_t *clientDir = new wchar_t[MAX_PATH];
	fileWorker->GetDirectory(client.DestinationLauncher(), clientDir);

	wstring res = fileWorker->ExecutionDirectory(false, clientDir);

	delete [] clientDir;
	
	HashingService *hashingService = new HashingServiceImpl();
	ServiceContainer::AddImpl(typeid(HashingService).name(), hashingService);

	ProcessWorker *processWorker = new ProcessWorkerImpl();
	ServiceContainer::AddImpl(typeid(ProcessWorker).name(), processWorker);

	SignatureVerifier *signatureVerifier = new SignatureVerifierImpl();
	ServiceContainer::AddImpl(typeid(SignatureVerifier).name(), signatureVerifier);

 	TriggeredSignature resultOfNegativeFileSignatureVerification = signatureVerifier->VerifyFileSignature(signatureToVerify);

	unsigned long prCount;
	const ProcessSignature** procSignature = signatures->GetAllProcessSignatures(prCount);

	TriggeredSignature ddd = signatureVerifier->VerifyProcessSignature(**procSignature);

	delete [] procSignature;
}