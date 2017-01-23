
// SrvManagerView.h : CSrvManagerView 类的接口
//


#pragma once
#include "SerConfig.h"

class CSrvManagerView : public CListView
{
protected: // 仅从序列化创建
	CSrvManagerView();
	DECLARE_DYNCREATE(CSrvManagerView)

// 属性
public:
	CSrvManagerDoc* GetDocument() const;

// 操作
public:
	void ShowServiceInfo();
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CSrvManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // SrvManagerView.cpp 中的调试版本
inline CSrvManagerDoc* CSrvManagerView::GetDocument() const
   { return reinterpret_cast<CSrvManagerDoc*>(m_pDocument); }
#endif

