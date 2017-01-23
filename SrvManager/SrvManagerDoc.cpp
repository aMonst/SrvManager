
// SrvManagerDoc.cpp : CSrvManagerDoc 类的实现
//

#include "stdafx.h"
#include "SrvManager.h"

#include "SrvManagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSrvManagerDoc

IMPLEMENT_DYNCREATE(CSrvManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSrvManagerDoc, CDocument)
END_MESSAGE_MAP()


// CSrvManagerDoc 构造/析构

CSrvManagerDoc::CSrvManagerDoc()
{
	// TODO: 在此添加一次性构造代码

}

CSrvManagerDoc::~CSrvManagerDoc()
{
}

BOOL CSrvManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSrvManagerDoc 序列化

void CSrvManagerDoc::Serialize(CArchive& ar)
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


// CSrvManagerDoc 诊断

#ifdef _DEBUG
void CSrvManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSrvManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSrvManagerDoc 命令
