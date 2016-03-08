#pragma once
#include "afxwin.h"
#include "EditLog/EditLog.h"
#include "Command.h"

// CCmdDlg �Ի���

class CCmdDlg : public CDialog, public CCommand
{
	DECLARE_DYNAMIC(CCmdDlg)

public:
	CCmdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCmdDlg();

// �Ի�������
	enum { IDD = IDD_CMD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void OnReturn();
	BOOL OnCmd(CString& cmd);
	void Print(LPCTSTR fmt, ...);
	void PrintLine(LPCTSTR fmt, ...);

	bool ModifyMem(ULONG_PTR addr, BYTE data[], size_t len);

	BOOL Cmd_Cls();
	BOOL Cmd_RTTI();
	BOOL Cmd_DB();
	BOOL Cmd_DW();
	BOOL Cmd_DD();
	BOOL Cmd_U();
	BOOL Cmd_E();

	//////////////////////////////////////////////////////////////////////////
	
	DECLARE_MESSAGE_MAP()
	DECLARE_COMMAND_MAP()

public:
	CComboBox m_cbInput;
	CString m_strInput;
	CEdit m_output;
	

protected:
	CEditLog	m_editlog;

	ULONG_PTR	m_dumpAddr;
	ULONG_PTR	m_disasmAddr;

	CString		m_strLastCmd;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//////////////////////////////////////////////////////////////////////////

inline ULONG_PTR xtol(LPCTSTR str)
{
	ULONG_PTR n;
	_stscanf(str, _T("%x"), &n);
	return n;
}
