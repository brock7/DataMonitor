#pragma once


// CFindPage �Ի���

class CFindPage : public CDialog
{
	DECLARE_DYNAMIC(CFindPage)

public:
	CFindPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindPage();

// �Ի�������
	enum { IDD = IDD_FINDPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_pageNum;
};
