// BinFindDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "BinFindDlg.h"
#include ".\binfinddlg.h"


// CBinFindDlg 对话框

IMPLEMENT_DYNAMIC(CBinFindDlg, CDialog)
CBinFindDlg::CBinFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinFindDlg::IDD, pParent)
	, m_hex(FALSE)
	, m_type(2)
	, m_strWhat(_T(""))
	, m_freezeThreads(FALSE)
	, m_saveResult(TRUE)
	, m_skipStack(TRUE)
	, m_code(FALSE)
{
}

CBinFindDlg::~CBinFindDlg()
{
}

void CBinFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_HEX, m_hex);
	DDX_Radio(pDX, IDC_BYTE, m_type);
	DDX_Text(pDX, IDC_WHAT, m_strWhat);
	DDX_Check(pDX, IDC_FREEZETHREADS, m_freezeThreads);
	DDX_Check(pDX, IDC_SAVERESULT, m_saveResult);
	DDX_Check(pDX, IDC_SKIPSTACK, m_skipStack);
	DDX_Check(pDX, IDC_CODE, m_code);
}


BEGIN_MESSAGE_MAP(CBinFindDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BYTE, OnBnClickedByte)
	ON_BN_CLICKED(IDC_WORD, OnBnClickedWord)
	ON_BN_CLICKED(IDC_DWORD, OnBnClickedDword)
	ON_BN_CLICKED(IDC_ASCII, OnBnClickedAscii)
	ON_BN_CLICKED(IDC_UNICODE, OnBnClickedUnicode)
END_MESSAGE_MAP()


// CBinFindDlg 消息处理程序

void CBinFindDlg::OnBnClickedOk()
{	
	OnOK();
}

void CBinFindDlg::OnBnClickedByte()
{
	m_type = 0;
}

void CBinFindDlg::OnBnClickedWord()
{
	m_type = 1;
}

void CBinFindDlg::OnBnClickedDword()
{
	m_type = 2;
}

void CBinFindDlg::OnBnClickedAscii()
{
	m_type = 3;
}

void CBinFindDlg::OnBnClickedUnicode()
{
	m_type = 3;
}
