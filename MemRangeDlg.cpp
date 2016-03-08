// MemRangeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "MemRangeDlg.h"
#include ".\memrangedlg.h"


// CMemRangeDlg 对话框

IMPLEMENT_DYNAMIC(CMemRangeDlg, CDialog)
CMemRangeDlg::CMemRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemRangeDlg::IDD, pParent)
{
	m_start = m_end = 0;
}

CMemRangeDlg::~CMemRangeDlg()
{
}

void AFX_CDECL _Afx_DDX_TextWithFormat2(CDataExchange* pDX, int nIDC,
										LPCTSTR lpszFormat, UINT nIDPrompt, ...)
										// only supports windows output formats - no floating point
{
	va_list pData;
	va_start(pData, nIDPrompt);

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	ASSERT( hWndCtrl != NULL );

	TCHAR szT[64];
	if (pDX->m_bSaveAndValidate)
	{
		void* pResult;

		pResult = va_arg( pData, void* );
		// the following works for %d, %u, %ld, %lu
		::GetWindowText(hWndCtrl, szT, _countof(szT));
		if (_stscanf(szT, lpszFormat, pResult) != 1)
		{
			AfxMessageBox(nIDPrompt);
			pDX->Fail();        // throws exception
		}
	}
	else
	{
		_vstprintf(szT, lpszFormat, pData);
		// does not support floating point numbers - see dlgfloat.cpp
		SetWindowText(hWndCtrl, szT);
	}

	va_end(pData);
}

void AFXAPI DDX_HexText(CDataExchange* pDX, int nIDC, DWORD& value)
{
	if (pDX->m_bSaveAndValidate)
		_Afx_DDX_TextWithFormat2(pDX, nIDC, _T("%x"), AFX_IDP_PARSE_UINT, &value);
	else
		_Afx_DDX_TextWithFormat2(pDX, nIDC, _T("%x"), AFX_IDP_PARSE_UINT, value);
}

void CMemRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_HexText(pDX, IDC_STARTADDR, m_start);
	DDX_HexText(pDX, IDC_ENDADDR, m_end);
}


BEGIN_MESSAGE_MAP(CMemRangeDlg, CDialog)
	ON_EN_CHANGE(IDC_STARTADDR, OnEnChangeStartaddr)
	ON_EN_KILLFOCUS(IDC_STARTADDR, OnEnKillfocusStartaddr)
END_MESSAGE_MAP()


// CMemRangeDlg 消息处理程序

void CMemRangeDlg::OnEnChangeStartaddr()
{

}

void CMemRangeDlg::OnEnKillfocusStartaddr()
{
	UpdateData();
	m_end = m_start;
	UpdateData(FALSE);
}
