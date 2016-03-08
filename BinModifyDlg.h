#pragma once


// CBinModifyDlg 对话框

class CBinModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CBinModifyDlg)

public:
	CBinModifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBinModifyDlg();

// 对话框数据
	enum { IDD = IDD_BINMODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_type;
	BOOL m_hex;
	BOOL m_freeze;
	afx_msg void OnBnClickedWord();
	afx_msg void OnBnClickedDword();
	afx_msg void OnBnClickedAscii();
	afx_msg void OnBnClickedUnicode();
	CString m_strWhat;
};
