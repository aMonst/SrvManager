
// SrvManagerView.h : CSrvManagerView ��Ľӿ�
//


#pragma once
#include "SerConfig.h"

class CSrvManagerView : public CListView
{
protected: // �������л�����
	CSrvManagerView();
	DECLARE_DYNCREATE(CSrvManagerView)

// ����
public:
	CSrvManagerDoc* GetDocument() const;

// ����
public:
	void ShowServiceInfo();
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CSrvManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

private:
	CSerConfig m_SrvConfig;
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnPause();
	afx_msg void OnContinue();
	afx_msg void OnDemandStart();
	afx_msg void OnAutoStart();
	afx_msg void OnDisabledStart();
	afx_msg void OnRefresh();
	afx_msg void OnSrvprop();
	afx_msg void OnDeletesrv();
	afx_msg void OnAddsrv();
};

#ifndef _DEBUG  // SrvManagerView.cpp �еĵ��԰汾
inline CSrvManagerDoc* CSrvManagerView::GetDocument() const
   { return reinterpret_cast<CSrvManagerDoc*>(m_pDocument); }
#endif

