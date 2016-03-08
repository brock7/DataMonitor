#pragma once


// CFindPage 对话框

class CFindPage : public CDialog
{
	DECLARE_DYNAMIC(CFindPage)

public:
	CFindPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindPage();

// 对话框数据
	enum { IDD = IDD_FINDPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_pageNum;
};
