// Command.h: interface for the CCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_H__2E14804B_6FDE_412F_8676_4FB4647B0926__INCLUDED_)
#define AFX_COMMAND_H__2E14804B_6FDE_412F_8676_4FB4647B0926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCommand;
typedef BOOL (CCommand::* TProcessCmd)();

typedef struct _tagCmdMapEntry
{
	LPCTSTR lpCmd;
	TProcessCmd pf;
}CmdMapEntry;

#define DECLARE_COMMAND_MAP() \
	static CmdMapEntry _CmdMapEntry[]; \
	static const CmdMapEntry* _GetBaseCommandMap(); \
	virtual CmdMapEntry* GetCommandMap();
	

#define BEGIN_COMMAND_MAP(class, baseclass) \
	const CmdMapEntry* class::_GetBaseCommandMap() {return baseclass::_CmdMapEntry;} \
	CmdMapEntry* class::GetCommandMap() {return class::_CmdMapEntry;} \
	CmdMapEntry class::_CmdMapEntry[] = {

#define COMMAND_MAP_ENTRY(cmd , op) \
	{cmd , (TProcessCmd )op} ,

#define END_COMMAND_MAP() \
	{NULL , NULL}};


class CCommand
{
public:
	CCommand();
	virtual ~CCommand();
	
	BOOL Run();
	BOOL SetCommandMap(CmdMapEntry* pMap);
	virtual CmdMapEntry* GetCommandMap();

	int GetParamCount();
	LPCTSTR GetCmd();
	LPCTSTR GetParam(int nIndex);
	BOOL Parse(LPCTSTR lpstr);

protected:
	virtual int OutputInfor(LPCTSTR pszFormat, ...);

protected:

	char	m_szCmd[0xff];
	LPCTSTR m_arParam[0x16];
	int		m_nParamCount;
	static CmdMapEntry _CmdMapEntry[];
};

#endif // !defined(AFX_COMMAND_H__2E14804B_6FDE_412F_8676_4FB4647B0926__INCLUDED_)
