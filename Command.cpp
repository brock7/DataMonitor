// Command.cpp: implementation of the CCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Command.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CmdMapEntry CCommand::_CmdMapEntry[] = {0, 0};

CCommand::CCommand()
{
	m_szCmd[0x0] = '\0';
	m_nParamCount = 0;
}

CCommand::~CCommand()
{

}

CmdMapEntry* CCommand::GetCommandMap()
{
	return NULL;
}

int CCommand::OutputInfor(LPCTSTR pszFormat, ...)
{
	return 0;
}

BOOL CCommand::Parse(LPCTSTR lpstr)
{
	TCHAR* pbuf = m_szCmd;
	const TCHAR* c = lpstr;
	BOOL bOverFlag = TRUE;
	m_nParamCount = 0;

	while (*c)
	{
		if (*c == _T('\x020'))
		{
			*pbuf = _T('\0');
			bOverFlag = TRUE;
		}
		else
		{
			if (bOverFlag)
			{
				m_arParam[m_nParamCount] = pbuf;
				m_nParamCount ++;
				bOverFlag = FALSE;
			}
			*pbuf = *c;
		}
		c++;
		pbuf++;
	}
	*pbuf = _T('\0');
	return TRUE;
}

LPCTSTR CCommand::GetParam(int nIndex)
{
	return m_arParam[nIndex];
}

LPCTSTR CCommand::GetCmd()
{
	if (m_nParamCount <= 0)
		return NULL;
	return m_arParam[0];
}

int CCommand::GetParamCount()
{
	return m_nParamCount;
}

BOOL CCommand::Run()
{
	CmdMapEntry* pMap = GetCommandMap();
	if (pMap == NULL)
		return FALSE;
	while(pMap->lpCmd != NULL && pMap->pf != NULL)
	{
		if (lstrcmpi(pMap->lpCmd , GetCmd()) == 0) {
			_XTrace(_T("'%s' Will run\n"), pMap->lpCmd);
			return (this->*pMap->pf)();
		}
		pMap ++;
	}

	return FALSE;
}
