
// SrvManagerDoc.cpp : CSrvManagerDoc ���ʵ��
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


// CSrvManagerDoc ����/����

CSrvManagerDoc::CSrvManagerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CSrvManagerDoc::~CSrvManagerDoc()
{
}

BOOL CSrvManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSrvManagerDoc ���л�

void CSrvManagerDoc::Serialize(CArchive& ar)
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


// CSrvManagerDoc ���

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


// CSrvManagerDoc ����
