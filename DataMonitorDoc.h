// DataMonitorDoc.h :  CDataMonitorDoc ��Ľӿ�
//


#pragma once

class CDataMonitorDoc : public CDocument
{
protected: // �������л�����
	CDataMonitorDoc();
	DECLARE_DYNCREATE(CDataMonitorDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CDataMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


