// SrvPropDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SrvManager.h"
#include "SrvPropDlg.h"
#include "SerConfig.h"
#include <Winsvc.h>
#include "MainFrm.h"

// CSrvPropDlg �Ի���

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


// CSrvPropDlg ��Ϣ�������

BOOL CSrvPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ComStart.AddString(_T("�ֶ�����"));
	m_ComStart.AddString(_T("�Զ�����"));
	m_ComStart.AddString(_T("��ֹ����"));
	m_ComStart.AddString(_T("δ֪����"));
	ShowSrvInfo();
	InitCtrlButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSrvPropDlg::ShowSrvInfo()
{
	SetDlgItemText(IDC_EDIT_SRVNAME, m_SrvItem.m_strSrvName);
	SetDlgItemText(IDC_EDIT_SRVDINAME, m_SrvItem.m_strSrvDispName);
	SetDlgItemText(IDC_EDIT_DISCRIPTION, m_SrvItem.m_strDescribInfo);
	SetDlgItemText(IDC_EDIT_PATH, m_SrvItem.m_strBinPath);
	SetDlgItemText(IDC_EDIT_DEPEND, m_SrvItem.m_strDependSev);

	//��ʾ״̬
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
	if (-1 == lAcceptCtrl)//��ȡ�ɿ���Ϣʧ�ܣ��������п���ѡ��
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
	}

	if(!(lAcceptCtrl & SERVICE_ACCEPT_PAUSE_CONTINUE))//��ǰ���񲻿ɽ�����ͣ����������ʱ�������������ѡ��
	{
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	}

	if (!(lAcceptCtrl & SERVICE_ACCEPT_STOP))//��ǰ���񲻿ɽ���ֹͣ����ʱ������ֹͣѡ��
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	}

	if (dwCurrentStatus == SERVICE_RUNNING)//��ǰ״̬Ϊ����������������������ѡ��
	{
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	}else if(dwCurrentStatus == SERVICE_STOPPED)//��ǰ״̬Ϊ��ֹͣ������ֹͣ����������ͣѡ��
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	}else if (dwCurrentStatus == SERVICE_PAUSED)//��ǰ״̬Ϊ��ͣ��������������ͣѡ��
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
	// TODO: ��������
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_START))
	{
		MessageBox(_T("��������ʧ�ܣ�"), _T("����"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("������"));
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(TRUE);
}

void CSrvPropDlg::OnBnClickedBtnStop()
{
	// TODO: ֹͣ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_STOP))
	{
		MessageBox(_T("��������ʧ�ܣ�"), _T("����"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("��ֹͣ"));
	}

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
}

void CSrvPropDlg::OnBnClickedBtnPause()
{
	// TODO: ��ͣ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_PAUSED))
	{
		MessageBox(_T("��������ʧ�ܣ�"), _T("����"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("����ͣ"));
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(TRUE);
}

void CSrvPropDlg::OnBnClickedBtnContinue()
{
	// TODO: ��������
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	CSerConfig *pSrvConfig = new CSerConfig;
	if (!pSrvConfig->CtrlServiceStatus(m_SrvItem.m_strSrvName, SERVICE_START))
	{
		MessageBox(_T("��������ʧ�ܣ�"), _T("����"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		pListCtrl->SetItemText(nIndex, 1, _T("������"));
	}

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(TRUE);

	delete pSrvConfig;
}
void CSrvPropDlg::OnCbnSelchangeComboStartstatus()
{
	// TODO: �û�ѡ����������
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
	// TODO: Ӧ�ð�ť
	CSerConfig *pSrvConfig = new CSerConfig;

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl *pListCtrl = (CListCtrl *)pFrame->GetActiveView();
	if (!pSrvConfig->ChangeServiceType(m_SrvItem.m_strSrvName, m_dwNewStart))
	{
		MessageBox(_T("���ķ���Ϊ��ֹ����ʧ��"), _T("����"), MB_OK);
	}else
	{
		int nIndex = pListCtrl->GetSelectionMark();
		if (SERVICE_DEMAND_START == m_dwNewStart)
		{
			pListCtrl->SetItemText(nIndex, 2, _T("�ֶ�"));
		}else if (SERVICE_AUTO_START == m_dwNewStart)
		{
			pListCtrl->SetItemText(nIndex, 2, _T("�Զ�"));
		}else if (SERVICE_DISABLED)
		{
			pListCtrl->SetItemText(nIndex, 2, _T("��ֹ"));
		}
		
	}

	delete pSrvConfig;
}
