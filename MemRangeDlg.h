#pragma once


// CMemRangeDlg 对话框

class CMemRangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CMemRangeDlg)

public:
	CMemRangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMemRangeDlg();

// 对话框数据
	enum { IDD = IDD_MEMRANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeStartaddr();
	DWORD m_start;
	DWORD m_end;
	afx_msg void OnEnKillfocusStartaddr();
};
