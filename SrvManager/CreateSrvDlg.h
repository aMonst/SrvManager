#pragma once


// CCreateSrvDlg 对话框

class CCreateSrvDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateSrvDlg)

public:
	CCreateSrvDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateSrvDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CREATESRV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSrvName;
	CString m_strDisName;
	DWORD m_dwStartType;
	CString m_strSrvDesc;
	CString m_strBinaryPathName;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSelectpath();
	afx_msg void OnBnClickedOk();
};
