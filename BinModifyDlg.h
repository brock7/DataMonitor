#pragma once


// CBinModifyDlg �Ի���

class CBinModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CBinModifyDlg)

public:
	CBinModifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBinModifyDlg();

// �Ի�������
	enum { IDD = IDD_BINMODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
