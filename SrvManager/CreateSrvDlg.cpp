// CreateSrvDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SrvManager.h"
#include "CreateSrvDlg.h"


// CCreateSrvDlg �Ի���

IMPLEMENT_DYNAMIC(CCreateSrvDlg, CDialog)

CCreateSrvDlg::CCreateSrvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateSrvDlg::IDD, pParent)
{
	m_strSrvName = _T("");
	m_strDisName = _T("");
	m_strSrvDesc = _T("");
	m_strBinaryPathName = _T("");
}

CCreateSrvDlg::~CCreateSrvDlg()
{
}

void CCreateSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCreateSrvDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SELECTPATH, &CCreateSrvDlg::OnBnClickedBtnSelectpath)
	ON_BN_CLICKED(IDOK, &CCreateSrvDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateSrvDlg ��Ϣ�������

BOOL CCreateSrvDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox *pComBo = (CComboBox*)GetDlgItem(IDC_COMBO_STARTTYPE);
	pComBo->AddString(_T("�Զ�"));
	pComBo->AddString(_T("�ֶ�"));
	pComBo->AddString(_T("��ֹ����"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCreateSrvDlg::OnBnClickedBtnSelectpath()
{
	// TODO: �������·��
	TCHAR szBuf[4096] = {0};
	BROWSEINFO bi = {0};
	bi.hwndOwner = this->m_hWnd;
	bi.lpszTitle = _T("��ѡ�����·��");
	bi.pszDisplayName = szBuf;
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	
	LPITEMIDLIST lpidl = NULL;
	InitCommonControls();
	lpidl = SHBrowseForFolder(&bi);
	if (NULL != lpidl)
	{
		SHGetPathFromIDList(lpidl, szBuf);
		GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(szBuf);
	}
}

void CCreateSrvDlg::OnBnClickedOk()
{
	// TODO: ȷ����ť�������û�����
	GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strBinaryPathName);
	GetDlgItem(IDC_EDIT_SRVNAME)->GetWindowText(m_strSrvName);
	GetDlgItem(IDC_EDIT_DISNAME)->GetWindowText(m_strDisName);
	GetDlgItem(IDC_EDIT_DESC)->GetWindowText(m_strSrvDesc);
	if (_T("") == m_strDisName)
	{
		m_strDisName = m_strSrvName;
	}

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_STARTTYPE);
	int nSelect = pCombo->GetCurSel();
	switch(nSelect)
	{
	case 0:
		m_dwStartType = SERVICE_AUTO_START;
		break;
	case 1:
		m_dwStartType = SERVICE_DEMAND_START;
		break;
	case 2:
		m_dwStartType = SERVICE_DISABLED;
		break;
	default:
		break;
	}
	OnOK();
}
