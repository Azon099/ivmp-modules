#include "CPawnManager.h"
#include <list>
#include <string>

#ifdef _LINUX
#include <stdarg.h>
#endif

using std::list;

CPawnManager::CPawnManager()
{

}

void CPawnManager::Load(const char *szName)
{
	std::string path = "pawnscripts/";
	path += szName;
	CPawn * pawn = new CPawn(path.c_str());

	if(pawn)
	{
		pawn->Execute();
		m_scripts.push_back(pawn);
	}
}

void CPawnManager::Register(AMX_NATIVE_INFO * funcArray)
{
	for(int i = 0; i < m_scripts.size(); i++)
	{
		CPawn * pawn = m_scripts.at(i);
		pawn->Register(funcArray);
	}
}

long CPawnManager::CallFunction(char * szFuncName, long lDefaultRetVal, char * szFormat, ...)
{
	for(int i = 0; i < m_scripts.size(); i++)
	{
		AMX m_amx = m_scripts.at(i)->GetAMX();
		int m_iError, m_iIndex;
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
	}
	return lDefaultRetVal;
}
