#pragma once

#include "SDK/SDK.h"
#include "CPawn.h"
#include <vector>

class CPawnManager
{
private:
	std::vector<CPawn*> m_scripts;

public:
	CPawnManager();
	void Load(const char * szName);
	CPawn * Get(int script) { return m_scripts.at(script); }
	void Register(AMX_NATIVE_INFO * funcArray);

	std::vector<CPawn*> GetScripts() { return m_scripts; }

	long CallFunction(char * szFuncName, long lDefaultRetVal = 1, char * szFormat = NULL, ...);
};

extern CPawnManager * g_pPawnManager;