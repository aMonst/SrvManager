// SrvPropDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SrvManager.h"
#include "SrvPropDlg.h"
#include "SerConfig.h"
#include <Winsvc.h>
#include "MainFrm.h"

// CSrvPropDlg 对话框

IMPLEMENT_DYNAMIC(CSrvPropDlg, CDialog)

CSrvPropDlg::CSrvPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrvPropDlg::IDD, pParent)
{
}

CSrvPropDlg::~CSrvPropDlg()
{
}

void CSrvPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STARTSTATUS, m_ComStart);
}


BEGIN_MESSAGE_MAP(CSrvPropDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_START, &CSrvPropDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CSrvPropDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CSrvPropDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_CONTINUE, &CSrvPropDlg::OnBnClickedBtnContinue)
	ON_CBN_SELCHANGE(IDC_COMBO_STARTSTATUS, &CSrvPropDlg::OnCbnSelchangeComboStartstatus)
	ON_BN_CLICKED(ID_APPLED, &CSrvPropDlg::OnBnClickedAppled)
END_MESSAGE_MAP()


// CSrvPropDlg 消息处理程序

BOOL CSrvPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ComStart.AddString(_T("手动启动"));
	m_ComStart.AddString(_T("自动启动"));
	m_ComStart.AddString(_T("禁止启动"));
	m_ComStart.AddString(_T("未知类型"));
	ShowSrvInfo();
	InitCtrlButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSrvPropDlg::ShowSrvInfo()
{
	SetDlgItemText(IDC_EDIT_SRVNAME, m_SrvItem.m_strSrvName);
	SetDlgItemText(IDC_EDIT_SRVDINAME, m_SrvItem.m_strSrvDispName);
	SetDlgItemText(IDC_EDIT_DISCRIPTION, m_SrvItem.m_strDescribInfo);
	SetDlgItemText(IDC_EDIT_PATH, m_SrvItem.m_strBinPath);
	SetDlgItemText(IDC_EDIT_DEPEND, m_SrvItem.m_strDependSev);

	//显示状态
	CSerConfig *pSrvConf = new CSerConfig;
	SetDlgItemText(IDC_EDIT_STATUS, pSrvConf->GetServiceCurrentStatus(m_SrvItem.m_dwSrvStatus));
	delete pSrvConf;

	m_ComStart.SetCurSel(ShowSrvStartStatus(m_SrvItem.m_dwSrvStartStatus));
}

int CSrvPropDlg::ShowSrvStartStatus(DWORD dwStartStatus)
{
	int iStartStatus;
	switch (dwStartStatus)
	{
	case SERVICE_AUTO_START:
		iStartStatus = 1;
		break;
	case SERVICE_DEMAND_START:
		iStartStatus = 0;
		break;
	case SERVICE_DISABLED:
		iStartStatus = 2;
		break;
	default:
		iStartStatus = 3;
		break;
	}

	return iStartStatus;
}

void CSrvPropDlg::InitCtrlButton()
{
	DWORD dwCurrentStatus = 0;
	long lAcceptCtrl = -1;
	CSerConfig *pSrvConf = new CSerConfig;
	lAcceptCtrl = pSrvConf->GetSrvCtrlAccept(m_SrvItem.m_strSrvName, &dwCurrentStatus);
	if (-1 == lAcceptCtrl)//获取可控信息失败，禁用所有控制选项
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
	}

	if(!(lAcceptCtrl & SERVICE_ACCEPT_PAUSE_CONTINUE))//当前服务不可进行暂停、继续操作时，禁用这个两个选项
	{
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	}

	if (!(lAcceptCtrl & SERVICE_ACCEPT_STOP))//当前服务不可进行停止操作时，禁用停止选项
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}

	if (dwCurrentStatus == SERVICE_RUNNING)//当前状态为已启动，禁用启动、继续选项
	{
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	}else if(dwCurrentStatus == SERVICE_STOPPED)//当前状态为以停止，禁用停止，继续，暂停选项
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	}else if (dwCurrentStatus == SERVICE_PAUSED)//当前状态为暂停，禁用启动、暂停选项
	{
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
	}else
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
	}

	delete pSrvConf;
}
void CSrvPropDlg::OnBnClickedBtnStart()
{
	// TODO: 启动服务
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_START))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("已启动"));
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(TRUE);
}

void CSrvPropDlg::OnBnClickedBtnStop()
{
	// TODO: 停止服务
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_STOP))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("已停止"));
	}

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
}

void CSrvPropDlg::OnBnClickedBtnPause()
{
	// TODO: 暂停服务
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_PAUSED))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("已暂停"));
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(TRUE);
}

void CSrvPropDlg::OnBnClickedBtnContinue()
{
	// TODO: 继续运行
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_START))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("已启动"));
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(TRUE);

	delete pSrvConfig;
}
void CSrvPropDlg::OnCbnSelchangeComboStartstatus()
{
	// TODO: 用户选定启动类型
	int nIndex = m_ComStart.GetCurSel();
	switch(nIndex)
	{
	case 0:
		m_dwNewStart = SERVICE_DEMAND_START;
		break;
	case 1:
		m_dwNewStart = SERVICE_AUTO_START;
		break;
	case 2:
		m_dwNewStart = SERVICE_DISABLED;
		break;
	default:
		break;
	}
}

void CSrvPropDlg::OnBnClickedAppled()
{
	// TODO: 应用按钮
	CSerConfig *pSrvConfig = new CSerConfig;

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	if (!pSrvConfig->ChangeServiceType(m_SrvItem.m_strSrvName, m_dwNewStart))
	{
		MessageBox(_T("更改服务为禁止启动失败"), _T("错误"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		if (SERVICE_DEMAND_START == m_dwNewStart)
		{
			pListCtrl->SetItemText(nIndex, 2, _T("手动"));
		}else if (SERVICE_AUTO_START == m_dwNewStart)
		{
			pListCtrl->SetItemText(nIndex, 2, _T("自动"));
		}else if (SERVICE_DISABLED)
		{
			pListCtrl->SetItemText(nIndex, 2, _T("禁止"));
		}
		
	}

	delete pSrvConfig;
}
