// DataMonitorDoc.cpp :  CDataMonitorDoc 类的实现
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


// CDataMonitorDoc 构造/析构

CDataMonitorDoc::CDataMonitorDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDataMonitorDoc::~CDataMonitorDoc()
{
}

BOOL CDataMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDataMonitorDoc 序列化

void CDataMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CDataMonitorDoc 诊断

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


// CDataMonitorDoc 命令
