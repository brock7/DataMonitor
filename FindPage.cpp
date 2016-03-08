// FindPage.cpp : 实现文件
//

#include "stdafx.h"
#include "DataMonitor.h"
#include "FindPage.h"


// CFindPage 对话框

IMPLEMENT_DYNAMIC(CFindPage, CDialog)
CFindPage::CFindPage(CWnd* pParent /*=NULL*/)
	: CDialog(CFindPage::IDD, pParent)
{
}

CFindPage::~CFindPage()
{
}

void CFindPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PAGENUM, m_pageNum);
}


BEGIN_MESSAGE_MAP(CFindPage, CDialog)
END_MESSAGE_MAP()


// CFindPage 消息处理程序
