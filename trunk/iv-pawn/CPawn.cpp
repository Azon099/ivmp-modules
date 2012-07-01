#include "CPawn.h"
#include <list>

#ifdef _LINUX
#include <stdarg.h>
#endif

using namespace std;

extern "C" int amx_ConsoleInit(AMX* amx);
extern "C" int amx_ConsoleCleanup(AMX* amx);
extern "C" int amx_TimeInit(AMX* amx);
extern "C" int amx_TimeCleanup(AMX* amx);
extern "C" int amx_CoreInit(AMX* amx);
extern "C" int amx_CoreCleanup(AMX* amx);
extern "C" int amx_FixedInit(AMX* amx);
extern "C" int amx_FixedCleanup(AMX* amx);
extern "C" int amx_FloatInit(AMX* amx);
extern "C" int amx_FloatCleanup(AMX* amx);
extern "C" int amx_StringInit(AMX* amx);
extern "C" int amx_StringCleanup(AMX* amx);

CPawn::CPawn(const char * szName)
{
	strcpy(m_szName, szName);
	LogPrintf("access: %s", m_szName);
	int m_iError = aux_LoadProgram(&m_amx, m_szName, m_pProg);
	if(!CheckError(m_iError))
	{
		return;
	}

	amx_ConsoleInit(&m_amx);
	amx_TimeInit(&m_amx);
	amx_CoreInit(&m_amx);
	amx_FixedInit(&m_amx);
	amx_FloatInit(&m_amx);
	amx_StringInit(&m_amx);

	CallFunction("OnInit");
}

CPawn::~CPawn()
{
	amx_ConsoleCleanup(&m_amx);
	amx_TimeCleanup(&m_amx);
	amx_CoreCleanup(&m_amx);
	amx_FixedCleanup(&m_amx);
	amx_FloatCleanup(&m_amx);
	amx_StringCleanup(&m_amx);
	aux_FreeProgram(&m_amx);
}

bool CPawn::Execute()
{
	cell iRet;
	int m_iError = amx_Exec(&m_amx, &iRet, AMX_EXEC_MAIN);
	return CheckError(m_iError);
}


void CPawn::Register(AMX_NATIVE_INFO * funcArray)
{
	amx_Register(&m_amx, funcArray, -1);
}

long CPawn::CallFunction(char * szFuncName, long lDefaultRetVal, char * szFormat, ...)
{
	int m_iError;
	m_iError = amx_FindPublic(&m_amx, szFuncName, &m_iIndex);
	if(m_iError == AMX_ERR_NONE)
	{
		const char* p = szFormat;
		if(p)
		{
			va_list ap;
			list <Param> pparams;
			va_start(ap, szFormat);
			while(*p)
			{
				switch(*p)
				{
				case 'b':
					{
						// TODO
					}
					break;
				case 'i':
					{
						int i = va_arg(ap, int);
						Param param;
						param.type = 'i';
						param.idata = i;
						pparams.push_front(param);
						break;
					}
				case 'f':
					{
						float f = va_arg(ap, float);
						Param param;
						param.type = 'f';
						param.fdata = f;
						pparams.push_front(param);
						break;
					}
					break;
				case 's':
					{
						char* sz = va_arg(ap, char*);
						Param param;
						param.type = 's';
						param.szdata = sz;
						pparams.push_front(param);
						break;
					}
				}
				++p;
			}
			va_end(ap);
			list <Param>::iterator iter;
			cell amx_addr, *phys_addr;
			for(iter = pparams.begin(); iter != pparams.end(); iter++)
			{
				switch((*iter).type)
				{
				case 'b':
					{
						// TODO
					}
					break;
				case 'i':
					{
						m_iError = amx_Push(&m_amx, (*iter).idata);
						CheckError(m_iError);
						break;
					}
				case 'f':
					{
						m_iError = amx_Push(&m_amx, amx_ftoc((*iter).fdata));
						CheckError(m_iError);
						break;
					}
					break;
				case 's':
					{
						m_iError = amx_PushString(&m_amx, &amx_addr, &phys_addr, (*iter).szdata, 0, 0);
						CheckError(m_iError);
						break;
					}
				}
			}
		}
		cell iRetVal = lDefaultRetVal;
		m_iError = amx_Exec(&m_amx, &iRetVal, m_iIndex);
		CheckError(m_iError);
		return (long)iRetVal;
	}
	return lDefaultRetVal;
}
