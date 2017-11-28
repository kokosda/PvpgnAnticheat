#include "HashingServiceImpl.h"
#include "../PvpgnSelfServices/LogManagerImpl.h"

using namespace Infrastructure;
using namespace SelfServices;

HashingServiceImpl::HashingServiceImpl()
{
	cryptoContext = NULL;

	if (CryptAcquireContext(&cryptoContext, NULL, MS_ENH_DSS_DH_PROV, PROV_DSS_DH, CRYPT_VERIFYCONTEXT) != TRUE)
	{
		LogManagerImpl::GetLogger(*this)->LogFatal("Cannot initialize crypto contenxt. Code: %s", ULongToString(GetLastError()).c_str());

		throw new exception("Cannot initialize crypto context.");
	}
}

HashingServiceImpl::~HashingServiceImpl()
{
	if (cryptoContext)
	{
		if (CryptReleaseContext(cryptoContext, NULL) != TRUE)
		{
			LogManagerImpl::GetLogger(*this)->LogWarn("Cannot release crypto contenxt. Code: %s", GetLastError());
		}

		cryptoContext = NULL;
	}
}

wstring HashingServiceImpl::GetHash(const unsigned char* dataBuff, unsigned long dataBuffLength)
{
	HCRYPTHASH hash;

	if (CryptCreateHash(cryptoContext, CALG_SHA, NULL, NULL, &hash) != TRUE)
	{
		LogManagerImpl::GetLogger(*this)->LogFatal("Cannot create hash object. Code: %s", GetLastError());
		throw new exception("Cannot create hash object.");
	}

	if (CryptHashData(hash, dataBuff, dataBuffLength, NULL) != TRUE)
	{
		ReleaseHash(hash);

		LogManagerImpl::GetLogger(*this)->LogFatal("Cannot produce hash data. Code: %s", GetLastError());
		throw new exception("Cannot produce hash data. See log for details.");
	}

	unsigned char *buff = new unsigned char[1024];
	DWORD buffLen;

	if (CryptGetHashParam(hash, HP_HASHVAL, buff, &buffLen, 0) != TRUE)
	{
		ReleaseHash(hash);
		delete [] buff;

		LogManagerImpl::GetLogger(*this)->LogFatal("Cannot retrieve hash data. Code: %s", GetLastError());
		throw new exception("Cannot retrieve hash data for file. See log for details.");
	}

	wstring res = ToWideString(buff, buffLen);

	ReleaseHash(hash);
	delete [] buff;

	return res;
}

void HashingServiceImpl::ReleaseHash(HCRYPTHASH &hash)
{
	if (CryptDestroyHash(hash) != TRUE)
	{
		LogManagerImpl::GetLogger(*this)->LogError("Cannot destroy hash. Code: %s", GetLastError());
	}

	hash = NULL;
}