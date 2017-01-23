#pragma once
#include "SerConfig.h"
#include "afxwin.h"

// CSrvPropDlg �Ի���

class CSrvPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CSrvPropDlg)

public:
	CSrvPropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSrvPropDlg();
	void ShowSrvInfo();
	int ShowSrvStartStatus(DWORD dwStartStatus);

// �Ի�������
	enum { IDD = IDD_SRVPROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	SRVITEM m_SrvItem;//�洢��ѡ�еķ���Ľ����Ϣ
	DWORD m_dwNewStart;
	virtual BOOL OnInitDialog();
	CComboBox m_ComStart;//�������͵���Ͽ�ؼ�
	void InitCtrlButton();//��ʼ��״̬���ư�ť
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnContinue();
	afx_msg void OnCbnSelchangeComboStartstatus();
	afx_msg void OnBnClickedAppled();
};
