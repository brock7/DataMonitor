// CmdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "CmdDlg.h"
#include ".\cmddlg.h"
#include "cppst.h"
#include "BinModifyDlg.h"

extern "C" {
#include "disasm/disasm.h"
}

//////////////////////////////////////////////////////////////////////////

#define CRLF				_T("\r\n")
#define IsValidPtr(Ptr)		( ULONG_PTR(Ptr) < 0x80000000 && ULONG_PTR(Ptr) > 0x10000)

//////////////////////////////////////////////////////////////////////////

BEGIN_COMMAND_MAP(CCmdDlg, CCommand)
	COMMAND_MAP_ENTRY(_T("CLS") , &CCmdDlg::Cmd_Cls)
	COMMAND_MAP_ENTRY(_T("RTTI") , &CCmdDlg::Cmd_RTTI)
	COMMAND_MAP_ENTRY(_T("DB") , &CCmdDlg::Cmd_DB)
	COMMAND_MAP_ENTRY(_T("DD") , &CCmdDlg::Cmd_DD)
	COMMAND_MAP_ENTRY(_T("DW") , &CCmdDlg::Cmd_DW)
	COMMAND_MAP_ENTRY(_T("U") , &CCmdDlg::Cmd_U)
	COMMAND_MAP_ENTRY(_T("E") , &CCmdDlg::Cmd_E)
END_COMMAND_MAP()

// CCmdDlg 对话框

IMPLEMENT_DYNAMIC(CCmdDlg, CDialog)
CCmdDlg::CCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdDlg::IDD, pParent)
	, m_strInput(_T(""))
{

	m_dumpAddr = m_disasmAddr = 0;
}

CCmdDlg::~CCmdDlg()
{
}

void CCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT, m_cbInput);
	DDX_CBString(pDX, IDC_INPUT, m_strInput);
	DDX_Control(pDX, IDC_OUTPUT, m_output);
}


BEGIN_MESSAGE_MAP(CCmdDlg, CDialog)
END_MESSAGE_MAP()


// CCmdDlg 消息处理程序

BOOL CCmdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_editlog.SetEditCtrl(m_output.GetSafeHwnd());
	// m_editlog.AddText("Try");
	// m_output.SetWindowText("afsfsa");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {

		if (pMsg->wParam == VK_RETURN) {
			
			OnReturn();
			return FALSE;

		} else if (pMsg->wParam == 'D') {

			if (GetKeyState(VK_CONTROL) & 0x8000) {
				AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_VIEW_CMDDLG, 0);
				return TRUE;
			}
		}		
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCmdDlg::OnReturn()
{
	if (GetFocus() == NULL || GetFocus()->GetParent() != &m_cbInput) {
		m_cbInput.SetFocus();
		return;
	}

	UpdateData();

	if (m_strInput.GetLength() == 0)
		m_strInput = m_strLastCmd;

	if (m_strInput.GetLength() == 0)
		return;

	PrintLine(_T("> %s"), (LPCTSTR )m_strInput);

	if (OnCmd(m_strInput)) {
		m_cbInput.InsertString(0, m_strInput);

		if (m_cbInput.GetCount() >= 100)
			m_cbInput.DeleteString(m_cbInput.GetCount() - 1);
	} else {

		Print(_T("invalid command") CRLF );
	}
	
	m_strLastCmd = m_strInput;

	m_strInput.Empty();
	UpdateData(FALSE);
}

BOOL CCmdDlg::OnCmd(CString& cmd)
{
	if (!Parse(cmd))
		return FALSE;

	return Run();
}

void CCmdDlg::Print(LPCTSTR fmt, ...)
{
	TCHAR str[2048];

	va_list varg;
	va_start(varg, fmt);
	_vsntprintf(str, _countof(str), fmt, varg);
	va_end(varg);

	m_editlog.AddText(str);
}

void CCmdDlg::PrintLine(LPCTSTR fmt, ...)
{
	TCHAR str[2048];

	va_list varg;
	va_start(varg, fmt);
	_vsntprintf(str, _countof(str), fmt, varg);
	va_end(varg);

	lstrcat(str, CRLF);

	m_editlog.AddText(str);
}

BOOL CCmdDlg::Cmd_Cls()
{
	m_output.SetWindowText(_T(""));
	return TRUE;
}

BOOL CCmdDlg::Cmd_RTTI()
{
	if (GetParamCount() != 2) {

		PrintLine(_T("invalid command"));
		return TRUE;
	}

	ULONG_PTR addr = xtol(GetParam(1));
	if (!IsValidPtr(addr)) {

		PrintLine(_T("invalid address: %p"), addr );
		return TRUE;
	}

	ULONG_PTR vtablePtr = 0;
	g_dbgEng->ReadMemory((PVOID )addr, (PBYTE )&vtablePtr, sizeof(vtablePtr));	

	ULONG_PTR rttiPtr = vtablePtr - sizeof(ULONG_PTR);
	ULONG_PTR rtti;
	g_dbgEng->ReadMemory((PVOID )rttiPtr, (PBYTE )&rtti, sizeof(rtti));

	ULONG_PTR typeDescPtrPtr = rtti + offsetof(RTTICompleteObjectLocator, pTypeDescriptor);
	ULONG_PTR typeDescPtr;

	g_dbgEng->ReadMemory((PVOID )typeDescPtrPtr, (PBYTE )&typeDescPtr, sizeof(typeDescPtr));

	TypeDescriptor typeDesc;
	memset(&typeDesc, 0 , sizeof(typeDesc));

	g_dbgEng->ReadMemory((PVOID )typeDescPtr, (PBYTE )&typeDesc, sizeof(typeDesc));

	PrintLine("class name: %s", typeDesc.name);

	ULONG_PTR vtable[32];
	memset(vtable, 0 , sizeof(vtable));
	g_dbgEng->ReadMemory((PVOID )vtablePtr, (PBYTE )vtable, sizeof(vtable));

	PrintLine("vtable: %p", vtablePtr);
	for (int i = 0; i < 32; i ++) {
		if (!IsValidPtr(vtable[i]))
			break;
		PrintLine("\t%d\t%p", i, vtable[i]);
	}
	
	return TRUE;
}

BOOL CCmdDlg::Cmd_DB()
{
	ULONG_PTR addr;
	if (GetParamCount() >= 2) {

		ULONG_PTR addr = xtol(GetParam(1));
		if (!IsValidPtr(addr)) {

			PrintLine(_T("invalid address: %p"), addr );
			return TRUE;
		}
	} else
		addr = m_dumpAddr;

	BYTE buf[1024];
	size_t len = 0x10;

	if (GetParamCount() > 2) {
		len = atol(GetParam(2));
		if (len == 0)
			len = 1;

		if (len > sizeof(buf))
			len = sizeof(buf);
	}
        
	size_t readlen = g_dbgEng->ReadMemory((PVOID )addr, buf, len);
	len = readlen;

	size_t n = 0;
	while (n < len) {

		Print("%p\t", addr + n);

		for (int i = 0; i < 0x10; i ++) {
			if (n + i < len)
				Print("%02X ", buf[n + i]);
			else
				Print("   ");
		}

		Print("|");

		for (int i = 0; i < 0x10 && n + i < len; i ++) {
			BYTE c = buf[n + i];

#define IsVisableChar(C)		( (C) < 0x20 )

			Print("%c", IsVisableChar(c) ? '.' : c);
		}
        
		Print(CRLF);

		n += 0x10;
	}

	m_dumpAddr = addr + len;

	return TRUE;
}

BOOL CCmdDlg::Cmd_DW()
{
	ULONG_PTR addr;
	if (GetParamCount() >= 2) {

		ULONG_PTR addr = xtol(GetParam(1));
		if (!IsValidPtr(addr)) {

			PrintLine(_T("invalid address: %p"), addr );
			return TRUE;
		}
	} else
		addr = m_dumpAddr;

	BYTE buf[1024];
	size_t len = 0x10 / sizeof(WORD);

	if (GetParamCount() > 2) {
		len = atol(GetParam(2));
		if (len == 0)
			len = 1;

		if (len > sizeof(buf) / sizeof(WORD))
			len = sizeof(buf) / sizeof(WORD);
	}

	size_t readlen = g_dbgEng->ReadMemory((PVOID )addr, buf, len * sizeof(WORD));
	len = readlen / sizeof(WORD);

	size_t n = 0;
	while (n < len) {

		Print("%p\t", addr + n * sizeof(WORD));
		for (int i = 0; i < 0x10 && n < len; i ++, n ++ ) {
			Print("%04X ", *(WORD* )&buf[n * sizeof(WORD)]);
		}

		Print(CRLF);
	}

	m_dumpAddr = addr + len * sizeof(WORD);

	return TRUE;
}

BOOL CCmdDlg::Cmd_DD()
{
	ULONG_PTR addr;
	if (GetParamCount() >= 2) {

		ULONG_PTR addr = xtol(GetParam(1));
		if (!IsValidPtr(addr)) {

			PrintLine(_T("invalid address: %p"), addr );
			return TRUE;
		}
	} else
		addr = m_dumpAddr;

	BYTE buf[1024];
	size_t len = 0x10 / sizeof(DWORD);

	if (GetParamCount() > 2) {
		len = atol(GetParam(2));
		if (len == 0)
			len = 1;

		if (len > sizeof(buf) / sizeof(DWORD))
			len = sizeof(buf) / sizeof(DWORD);
	}

	size_t readlen = g_dbgEng->ReadMemory((PVOID )addr, buf, len * sizeof(DWORD));
	len = readlen / sizeof(DWORD);

	size_t n = 0;
	while (n < len) {

		Print("%p\t", addr + n * sizeof(DWORD));

		for (int i = 0; i < 0x10 && n < len; i ++, n ++) {
			Print("%08X ", *(DWORD* )&buf[n * sizeof(DWORD)]);
		}

		Print(CRLF);
	}

	m_dumpAddr = addr + len * sizeof(DWORD);

	return TRUE;
}

BOOL CCmdDlg::Cmd_U()
{
	ULONG_PTR addr;
	if (GetParamCount() >= 2) {

		addr = xtol(GetParam(1));
		if (!IsValidPtr(addr)) {

			PrintLine(_T("invalid address: %p"), addr );
			return TRUE;
		}
	} else
		addr = m_disasmAddr;

	BYTE buf[1024];
	size_t len = 0x10;

	if (GetParamCount() > 2) {
		len = atol(GetParam(2));
		if (len == 0)
			len = 1;

		if (len > sizeof(buf))
			len = sizeof(buf);
	}

	size_t readlen = g_dbgEng->ReadMemory((PVOID )addr, buf, len);
	len = readlen;

	t_disasm da;
	memset(&da, 0, sizeof(da));

	// ulong l = Disasm(code, len, (ulong )addr, &da, DISASM_SIZE);
	ULONG pos = 0;
	ulong l;

	m_disasmAddr = addr;

	while (pos <= len) {

		l = Disasm((char* )&buf[pos], len - pos, (ulong )addr + pos, &da, DISASM_CODE);
		if (l == 0)
			break;

		m_disasmAddr += l;
	    
		PrintLine(" %p | %3i  %-24s  %-24s\n", addr + pos, l, da.dump, da.result);

		pos += l;
	}
	
	return TRUE;
}

bool CCmdDlg::ModifyMem(ULONG_PTR addr, BYTE data[], size_t len)
{
	return g_dbgEng->WriteMemory(PVOID(addr), data, len) == len;
}

BOOL CCmdDlg::Cmd_E()
{
	if (GetParamCount() != 2) {

		PrintLine(_T("invalid command"));
		return TRUE;
	}

	ULONG_PTR addr = xtol(GetParam(1));
	if (!IsValidPtr(addr)) {

		PrintLine(_T("invalid address: %p"), addr );
		return TRUE;
	}

	CBinModifyDlg dlg;
	if (dlg.DoModal() != IDOK)
		return TRUE;

	DWORD n;
	WCHAR* wstr;
	size_t len;

	bool result = false;

	USES_CONVERSION;

	switch(dlg.m_type) {
		case 0: // BYTE
		case 1: // WORD
		case 2:	// DWORD
			if (dlg.m_hex)
				_stscanf((LPCTSTR )dlg.m_strWhat, "%x", &n);
			else
				_stscanf((LPCTSTR )dlg.m_strWhat, "%d", &n);

			result = ModifyMem(addr, (BYTE* )&n, dlg.m_type == 0 ? 1 : dlg.m_type * 2);
			break;

		case 3: // ASCII
			result = ModifyMem(addr, (BYTE* )(LPCSTR )dlg.m_strWhat, dlg.m_strWhat.GetLength());
			break;

		case 4: // UNICODE
			wstr = T2W((LPCSTR )dlg.m_strWhat);
			len = wcslen(wstr) * 2;

			result = ModifyMem(addr, (BYTE* )wstr, len);
			break;
	}

	if (!result)
		MessageBox(_T("modification failed."), NULL);

	return TRUE;
}
