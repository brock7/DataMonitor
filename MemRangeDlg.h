#pragma once


// CMemRangeDlg �Ի���

class CMemRangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CMemRangeDlg)

public:
	CMemRangeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMemRangeDlg();

// �Ի�������
	enum { IDD = IDD_MEMRANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeStartaddr();
	DWORD m_start;
	DWORD m_end;
	afx_msg void OnEnKillfocusStartaddr();
};
