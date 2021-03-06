
// SrvManagerView.cpp : CSrvManagerView 濬腔妗珋
//

#include "stdafx.h"
#include "SrvManager.h"

#include "SrvManagerDoc.h"
#include "SrvManagerView.h"
#include <Winsvc.h>
#include "SrvPropDlg.h"
#include "CreateSrvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSrvManagerView

IMPLEMENT_DYNCREATE(CSrvManagerView, CListView)

BEGIN_MESSAGE_MAP(CSrvManagerView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CSrvManagerView::OnNMDblclk)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_RCLICK, &CSrvManagerView::OnNMRClick)
	ON_COMMAND(ID_START, &CSrvManagerView::OnStart)
	ON_COMMAND(ID_STOP, &CSrvManagerView::OnStop)
	ON_COMMAND(ID_PAUSE, &CSrvManagerView::OnPause)
	ON_COMMAND(ID_CONTINUE, &CSrvManagerView::OnContinue)
	ON_COMMAND(ID_DEMAND_START, &CSrvManagerView::OnDemandStart)
	ON_COMMAND(ID_AUTO_START, &CSrvManagerView::OnAutoStart)
	ON_COMMAND(ID_DISABLED_START, &CSrvManagerView::OnDisabledStart)
	ON_COMMAND(ID_REFRESH, &CSrvManagerView::OnRefresh)
	ON_COMMAND(ID_SRVPROP, &CSrvManagerView::OnSrvprop)
	ON_COMMAND(ID_DELETESRV, &CSrvManagerView::OnDeletesrv)
	ON_COMMAND(ID_ADDSRV, &CSrvManagerView::OnAddsrv)
END_MESSAGE_MAP()



CSrvManagerView::CSrvManagerView()
{
	
}

CSrvManagerView::~CSrvManagerView()
{
}

BOOL CSrvManagerView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CListView::PreCreateWindow(cs);
}

void CSrvManagerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& MainList = GetListCtrl();
	//修改主界面的ListCrtl控件为报表样式、整行选中
	SetWindowLong(MainList.m_hWnd, GWL_STYLE, GetWindowLong(MainList.m_hWnd, GWL_STYLE) | LVS_REPORT);
	MainList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	MainList.InsertColumn(0, _T("服务名称"), LVCFMT_LEFT, 100);
	MainList.InsertColumn(1, _T("当前状态"), LVCFMT_LEFT, 90);
	MainList.InsertColumn(2, _T("启动类型"), LVCFMT_LEFT, 90);
	MainList.InsertColumn(3, _T("所在路径"), LVCFMT_LEFT, 230);
	MainList.InsertColumn(4, _T("服务描述"), LVCFMT_LEFT, 400);
	m_SrvConfig.EnumServices();
	ShowServiceInfo();
}




#ifdef _DEBUG
void CSrvManagerView::AssertValid() const
{
	CListView::AssertValid();
}

void CSrvManagerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSrvManagerDoc* CSrvManagerView::GetDocument() const 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSrvManagerDoc)));
	return (CSrvManagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CSrvManagerView 消息处理函数

//将获取到的服务信息显示到界面
void CSrvManagerView::ShowServiceInfo()
{
	CSerConfig::iterator it = m_SrvConfig.begin();
	CListCtrl &rListCtrl = GetListCtrl();
	for (int i = 0; it != m_SrvConfig.end(); i++)
	{
		rListCtrl.InsertItem(i, _T(""));
		rListCtrl.SetItemText(i, 0,it->m_strSrvDispName);
		rListCtrl.SetItemText(i, 1, m_SrvConfig.GetServiceCurrentStatus(it->m_dwSrvStatus));
		rListCtrl.SetItemText(i, 2, m_SrvConfig.GetServiceStartStatus(it->m_dwSrvStartStatus));
		rListCtrl.SetItemText(i, 3, it->m_strBinPath);
		rListCtrl.SetItemText(i, 4, it->m_strDescribInfo);
		++it;
	}
}

void CSrvManagerView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// 双击鼠标
	CSrvPropDlg SrvPropDlg;
	int nIndex = GetListCtrl().GetSelectionMark();
	SrvPropDlg.m_SrvItem = m_SrvConfig.GetSrvItemByIndex(nIndex);
	SrvPropDlg.DoModal();
	*pResult = 0;
}

void CSrvManagerView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CPoint ptMouse;
	GetCursorPos(&ptMouse);

	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(pNMItemActivate->iItem);
	DWORD dwCurrStatus = 0;
	long lAcceptCtrl = m_SrvConfig.GetSrvCtrlAccept(rSrvItem.m_strSrvName, &dwCurrStatus);

	CMenu Menu, *pMenu = NULL;
	Menu.LoadMenu(IDM_SRV_CTRL);
	pMenu = Menu.GetSubMenu(0);
	
	if (-1 == lAcceptCtrl)//获取可控信息失败，禁用所有控制选项
	{
		pMenu->EnableMenuItem(ID_STOP, MF_GRAYED);
		pMenu->EnableMenuItem(ID_CONTINUE, MF_GRAYED);
		pMenu->EnableMenuItem(ID_START, MF_GRAYED);
		pMenu->EnableMenuItem(ID_PAUSE, MF_GRAYED);
	}

	if(!(lAcceptCtrl & SERVICE_ACCEPT_PAUSE_CONTINUE))//当前服务不可进行暂停、继续操作时，禁用这个两个选项
	{
		pMenu->EnableMenuItem(ID_PAUSE, MF_GRAYED);
		pMenu->EnableMenuItem(ID_CONTINUE, MF_GRAYED);
	}

	if (!(lAcceptCtrl & SERVICE_ACCEPT_STOP))//当前服务不可进行停止操作时，禁用停止选项
	{
		pMenu->EnableMenuItem(ID_STOP, MF_GRAYED);
	}

	if (dwCurrStatus == SERVICE_RUNNING)//当前状态为已启动，禁用启动、继续选项
	{
		pMenu->EnableMenuItem(ID_START, MF_GRAYED);
		pMenu->EnableMenuItem(ID_CONTINUE, MF_GRAYED);
	}else if(dwCurrStatus == SERVICE_STOPPED)//当前状态为以停止，禁用停止，继续，暂停选项
	{
		pMenu->EnableMenuItem(ID_STOP, MF_GRAYED);
		pMenu->EnableMenuItem(ID_PAUSE, MF_GRAYED);
		pMenu->EnableMenuItem(ID_CONTINUE, MF_GRAYED);
	}else if (dwCurrStatus == SERVICE_PAUSED)//当前状态为暂停，禁用启动、暂停选项
	{
		pMenu->EnableMenuItem(ID_START, MF_GRAYED);
		pMenu->EnableMenuItem(ID_PAUSE, MF_GRAYED);
	}else
	{
		pMenu->EnableMenuItem(ID_STOP, MF_GRAYED);
		pMenu->EnableMenuItem(ID_CONTINUE, MF_GRAYED);
		pMenu->EnableMenuItem(ID_START, MF_GRAYED);
		pMenu->EnableMenuItem(ID_PAUSE, MF_GRAYED);
	}

	if (SERVICE_AUTO_START == rSrvItem.m_dwSrvStartStatus)
	{
		pMenu->EnableMenuItem(ID_AUTO_START, MF_GRAYED);
	}
	else if (SERVICE_DEMAND_START == rSrvItem.m_dwSrvStartStatus)
	{
		pMenu->EnableMenuItem(ID_DEMAND_START, MF_GRAYED);
	}else if (SERVICE_DISABLED == rSrvItem.m_dwSrvStartStatus)
	{
		pMenu->EnableMenuItem(ID_DISABLED_START, MF_GRAYED);
	}

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN, ptMouse.x, ptMouse.y, this);

	*pResult = 0;
}

void CSrvManagerView::OnStart()
{
	// TODO:启动服务
	int nSrvIndex = GetListCtrl().GetSelectionMark();
	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nSrvIndex);
	
	if (!m_SrvConfig.CtrlServiceStatus(rSrvItem.m_strSrvName, SERVICE_START))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		GetListCtrl().SetItemText(nSrvIndex, 1, _T("已启动"));
	}
}

void CSrvManagerView::OnStop()
{
	// TODO: 停止服务
	int nSrvIndex = GetListCtrl().GetSelectionMark();
	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nSrvIndex);

	if (!m_SrvConfig.CtrlServiceStatus(rSrvItem.m_strSrvName, SERVICE_STOP))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		GetListCtrl().SetItemText(nSrvIndex, 1, _T("已停止"));
	}
}

void CSrvManagerView::OnPause()
{
	// TODO: 暂停服务
	int nSrvIndex = GetListCtrl().GetSelectionMark();
	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nSrvIndex);

	if (!m_SrvConfig.CtrlServiceStatus(rSrvItem.m_strSrvName, SERVICE_PAUSED))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		GetListCtrl().SetItemText(nSrvIndex, 1, _T("已暂停"));
	}
}

void CSrvManagerView::OnContinue()
{
	// TODO: 继续运行服务
	int nSrvIndex = GetListCtrl().GetSelectionMark();
	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nSrvIndex);

	if (!m_SrvConfig.CtrlServiceStatus(rSrvItem.m_strSrvName, SERVICE_START))
	{
		MessageBox(_T("启动服务失败！"), _T("错误"), MB_OK);
	}else
	{
		GetListCtrl().SetItemText(nSrvIndex, 1, _T("已启动"));
	}
}

void CSrvManagerView::OnDemandStart()
{
	// TODO: 手动启动服务
	int nSrvIndex = GetListCtrl().GetSelectionMark();
	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nSrvIndex);

	if (!m_SrvConfig.ChangeServiceType(rSrvItem.m_strSrvName, SERVICE_DEMAND_START))
	{
		MessageBox(_T("更改服务为手动启动失败"), _T("错误"), MB_OK);
	}else
	{
		GetListCtrl().SetItemText(nSrvIndex, 2, _T("手动"));
	}
}

void CSrvManagerView::OnAutoStart()
{
	// TODO: 自动启动服务
	int nSrvIndex = GetListCtrl().GetSelectionMark();
	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nSrvIndex);

	if (!m_SrvConfig.ChangeServiceType(rSrvItem.m_strSrvName, SERVICE_AUTO_START))
	{
		MessageBox(_T("更改服务为自动启动失败"), _T("错误"), MB_OK);
	}else
	{
		GetListCtrl().SetItemText(nSrvIndex, 2, _T("自动"));
	}
}

void CSrvManagerView::OnDisabledStart()
{
	// TODO: 禁止启动服务
	int nSrvIndex = GetListCtrl().GetSelectionMark();
	SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nSrvIndex);

	if (!m_SrvConfig.ChangeServiceType(rSrvItem.m_strSrvName, SERVICE_DISABLED))
	{
		MessageBox(_T("更改服务为禁止启动失败"), _T("错误"), MB_OK);
	}else
	{
		GetListCtrl().SetItemText(nSrvIndex, 2, _T("禁止"));
	}
}

void CSrvManagerView::OnRefresh()
{
	// TODO: 刷新菜事件处理
	//先清除链表和显示信息，在重新获取信息并显示
	m_SrvConfig.Clear();
	GetListCtrl().DeleteAllItems();
	m_SrvConfig.EnumServices();
	ShowServiceInfo();
}

void CSrvManagerView::OnSrvprop()
{
	// TODO: 弹出属性对话框
	CSrvPropDlg SrvPropDlg;
	int nIndex = GetListCtrl().GetSelectionMark();
	SrvPropDlg.m_SrvItem = m_SrvConfig.GetSrvItemByIndex(nIndex);
	SrvPropDlg.DoModal();
}

void CSrvManagerView::OnDeletesrv()
{
	// TODO: 删除服务
	int iSelect = MessageBox(_T("删除的服务可能会影响您正常使用系统，是否继续"), _T("警告"), MB_YESNO | MB_ICONWARNING);
	if (IDYES == iSelect)
	{
		int nIndex = GetListCtrl().GetSelectionMark();
		SRVITEM &rSrvItem = m_SrvConfig.GetSrvItemByIndex(nIndex);
		BOOL bRet = m_SrvConfig.DeleteService(rSrvItem);
		if (!bRet)
		{
			MessageBox(_T("删除服务失败"));
			return ;
		}

		GetListCtrl().DeleteItem(nIndex);
	}
}

void CSrvManagerView::OnAddsrv()
{
	// TODO: 添加服务
	CCreateSrvDlg CreateSrvDlg;
	if (IDOK == CreateSrvDlg.DoModal())
	{
		BOOL bRet = m_SrvConfig.CreateServices(CreateSrvDlg.m_strSrvName, CreateSrvDlg.m_strDisName, CreateSrvDlg.m_dwStartType, CreateSrvDlg.m_strBinaryPathName, CreateSrvDlg.m_strSrvDesc);
		if (!bRet)
		{
			MessageBox(_T("创建服务失败"));
			return;
		}

		OnRefresh();
	}
	
}
