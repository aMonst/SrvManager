
// SrvManagerDoc.h : CSrvManagerDoc ��Ľӿ�
//


#pragma once


class CSrvManagerDoc : public CDocument
{
protected: // �������л�����
	CSrvManagerDoc();
	DECLARE_DYNCREATE(CSrvManagerDoc)

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
	virtual ~CSrvManagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


