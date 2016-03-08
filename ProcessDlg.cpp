// ProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "ProcessDlg.h"
#include ".\processdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessDlg dialog


CProcessDlg::CProcessDlg(DataProv* dataPrev, CWnd* pParent /*=NULL*/)
	: CDialog(CProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_idhex = false;
	m_dwSel = 0;
	m_dataPrev = dataPrev;
	m_multisel = false;
}


void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessDlg)
	DDX_Control(pDX, IDC_LIST_PROCESS, m_listProcess);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessDlg)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	//}}AFX_MSG_MAP
//	ON_LBN_SELCHANGE(IDC_LIST_PROCESS, OnLbnSelchangeListProcess)
	ON_LBN_DBLCLK(IDC_LIST_PROCESS, OnLbnDblclkListProcess)
	ON_WM_VKEYTOITEM()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessDlg message handlers

void CProcessDlg::OnOK() 
{
	int nSel = m_listProcess.GetCurSel();
	if (nSel == -1)
		return;

	m_listProcess.GetText(nSel, m_strSel);

	if (m_idhex) {
		
		_stscanf(m_strSel.GetBuffer(), _T("%x"), &m_dwSel);

	} else
		m_dwSel = _ttol((LPCTSTR )m_strSel);
	
	CDialog::OnOK();
}

void CProcessDlg::OnRefresh() 
{
	m_listProcess.ResetContent();

	DWORD id;
	CString name;
	
	
	BOOL bContinue = m_dataPrev->GetFirstData(id, name);
	while (bContinue) {
		CString strItem;
		if (m_idhex)
			strItem.Format(_T("%08x %s"), id, (LPCTSTR )name);
		else
			strItem.Format(_T("%5d %s"), id, (LPCTSTR )name);
		m_listProcess.AddString(strItem);

		if (strItem.GetLength() * 10 > m_listProcess.GetHorizontalExtent()) {
			m_listProcess.SetHorizontalExtent(strItem.GetLength() * 10);
		}

		bContinue = m_dataPrev->GetNextData(id, name);
	}
}

BOOL CProcessDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(m_title);	
	if (m_multisel)
		ModifyStyle(0, LBS_EXTENDEDSEL);
	OnRefresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProcessDlg::OnLbnDblclkListProcess()
{
	CProcessDlg::OnOK();
}

void CProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);


}

void CProcessDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	/*
	int cx = pRect->right - pRect->left;
	int cy = pRect->bottom - pRect->top;

	CWnd* pWnd = GetDlgItem(IDC_REFRESH);
	CRect rc;
	pWnd->GetWindowRect(&rc);

	CRect listRc;
	m_listProcess.GetWindowRect(&listRc);
	const int sp = listRc.left - pRect->left;

	m_listProcess.SetWindowPos(NULL, 0, 0, cx - 2 * sp, cy - rc.Height() - 2 * sp, 
		SWP_NOMOVE | SWP_NOZORDER);

	pWnd->SetWindowPos(NULL, cx - 2 * (rc.Width() + 10) - sp, 0, 0, 
		cy - rc.Height() - sp, SWP_NOSIZE | SWP_NOZORDER);

	pWnd = GetDlgItem(IDCANCEL);
	pWnd->SetWindowPos(NULL, cx - 1 * (rc.Width() + 10) - sp, 0, 0, 
		cy - rc.Height() - sp, SWP_NOSIZE | SWP_NOZORDER);

	pWnd = GetDlgItem(IDOK);
	pWnd->SetWindowPos(NULL, cx - sp, 0, 0, 
		cy - rc.Height() - sp, SWP_NOSIZE | SWP_NOZORDER);
	*/
}

int CProcessDlg::OnVKeyToItem( UINT nKey, CListBox* pListBox, UINT nIndex )
{
	if (m_title != _T("Process"))
		return false;

	UINT count = pListBox->GetCount();

	UINT cur = nIndex + 1;
	if (cur >= count)
		return -2;

	CString keyStr;
	keyStr.Format(_T(" %c"), nKey);
	keyStr = keyStr.MakeUpper();

	CString txt;
	while (cur != nIndex) {
		pListBox->GetText(cur, txt);
		txt = txt.MakeUpper();
		if (txt.Find(keyStr) != -1) {
			pListBox->SetCurSel(cur);
			break;
		}

		cur = (cur + 1) % count;
	}

	return -2;
}