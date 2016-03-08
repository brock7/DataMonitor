#pragma once


// CBinFindDlg 对话框

class CBinFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CBinFindDlg)

public:
	CBinFindDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBinFindDlg();

// 对话框数据
	enum { IDD = IDD_BINFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_hex;
	int m_type;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedByte();
	afx_msg void OnBnClickedWord();
	afx_msg void OnBnClickedDword();
	afx_msg void OnBnClickedAscii();
	afx_msg void OnBnClickedUnicode();
	CString m_strWhat;
	BOOL m_freezeThreads;
	BOOL m_saveResult;
	BOOL m_skipStack;
	BOOL m_code;
};
