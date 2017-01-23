#pragma once
#include "SerConfig.h"
#include "afxwin.h"

// CSrvPropDlg 对话框

class CSrvPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CSrvPropDlg)

public:
	CSrvPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSrvPropDlg();
	void ShowSrvInfo();
	int ShowSrvStartStatus(DWORD dwStartStatus);

// 对话框数据
	enum { IDD = IDD_SRVPROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	SRVITEM m_SrvItem;//存储被选中的服务的结点信息
	DWORD m_dwNewStart;
	virtual BOOL OnInitDialog();
	CComboBox m_ComStart;//启动类型的组合框控件
	void InitCtrlButton();//初始化状态控制按钮
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnContinue();
	afx_msg void OnCbnSelchangeComboStartstatus();
	afx_msg void OnBnClickedAppled();
};
