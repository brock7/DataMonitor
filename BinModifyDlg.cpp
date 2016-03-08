// BinModifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "BinModifyDlg.h"
#include ".\binmodifydlg.h"


// CBinModifyDlg 对话框

IMPLEMENT_DYNAMIC(CBinModifyDlg, CDialog)
CBinModifyDlg::CBinModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinModifyDlg::IDD, pParent)
	, m_type(2)
	, m_hex(TRUE)
	, m_freeze(FALSE)
	, m_strWhat(_T(""))
{
}

CBinModifyDlg::~CBinModifyDlg()
{
}

void CBinModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_BYTE, m_type);
	DDX_Check(pDX, IDC_HEX, m_hex);
	DDX_Check(pDX, IDC_FREEZETHREADS, m_freeze);
	DDX_Text(pDX, IDC_WHAT, m_strWhat);
}


BEGIN_MESSAGE_MAP(CBinModifyDlg, CDialog)
	ON_BN_CLICKED(IDC_WORD, OnBnClickedWord)
	ON_BN_CLICKED(IDC_DWORD, OnBnClickedDword)
	ON_BN_CLICKED(IDC_ASCII, OnBnClickedAscii)
	ON_BN_CLICKED(IDC_UNICODE, OnBnClickedUnicode)
END_MESSAGE_MAP()


// CBinModifyDlg 消息处理程序

void CBinModifyDlg::OnBnClickedWord()
{
	m_type = 1;
}

void CBinModifyDlg::OnBnClickedDword()
{
	m_type = 2;
}

void CBinModifyDlg::OnBnClickedAscii()
{
	m_type = 3;
}

void CBinModifyDlg::OnBnClickedUnicode()
{
	m_type = 4;
}
