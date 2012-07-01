#pragma once

#include "SDK/SDK.h"

#undef UNICODE
#undef _UNICODE
#undef _UNICODE_
#include "pawn/amx/amx.h"
#include "pawn/amx/amxaux.h"

class CPawn
{
private:
	char m_szName[128];
	AMX m_amx;
	int	m_iIndex;
	void * m_pProg;

public:
	CPawn(const char * szName);
	~CPawn();

	bool Execute();

	void Register(AMX_NATIVE_INFO * funcArray);

	long CallFunction(char * szFuncName, long lDefaultRetVal = 1, char * szFormat = NULL, ...);

	char * GetName() { return m_szName; }

	AMX GetAMX() { return m_amx; }
};

inline bool CheckError(int iError)
{
	if(iError != AMX_ERR_NONE)
	{
		LogPrintf("Run time error %d: %s", iError, aux_StrError(iError));
		return false;
	}
	return true;
}

typedef struct _Param
{
	int type;
	bool bdata;
	int idata;
	float fdata;
	char* szdata;
} Param;
