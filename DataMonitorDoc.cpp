// DataMonitorDoc.cpp :  CDataMonitorDoc ���ʵ��
//

#include "stdafx.h"
#include "DataMonitor.h"

#include "DataMonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDataMonitorDoc

IMPLEMENT_DYNCREATE(CDataMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CDataMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CDataMonitorDoc ����/����

CDataMonitorDoc::CDataMonitorDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CDataMonitorDoc::~CDataMonitorDoc()
{
}

BOOL CDataMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDataMonitorDoc ���л�

void CDataMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CDataMonitorDoc ���

#ifdef _DEBUG
void CDataMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDataMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDataMonitorDoc ����
