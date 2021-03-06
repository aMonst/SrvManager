#include "StdAfx.h"
#include "SerConfig.h"
#include <Winsvc.h>

CSerConfig::CSerConfig(void)
{
	m_pHead = NULL;
}

CSerConfig::~CSerConfig(void)
{
}

BOOL CSerConfig::EnumServices()
{
	//获取服务信息列表
	SC_HANDLE  hSChandle = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSChandle)
	{
		return FALSE;
	}

	LPENUM_SERVICE_STATUS lpServices = NULL;
	DWORD cbBytesNeeded = 0;
	DWORD ServicesReturned = 0;
	DWORD ResumeHandle = 0;
	DWORD cbBufSize = 0;
	BOOL bRet = ::EnumServicesStatus(hSChandle, SERVICE_WIN32, SERVICE_STATE_ALL, lpServices, 0, &cbBytesNeeded, &ServicesReturned, &ResumeHandle);
	if (!bRet && ERROR_MORE_DATA == GetLastError())
	{
		cbBufSize = cbBytesNeeded;
		lpServices = (LPENUM_SERVICE_STATUS)new char[cbBufSize];
		::ZeroMemory(lpServices, cbBufSize);
		bRet = ::EnumServicesStatus(hSChandle, SERVICE_WIN32, SERVICE_STATE_ALL, lpServices, cbBufSize, &cbBytesNeeded, &ServicesReturned, &ResumeHandle);
		if (!bRet)
		{
			::CloseServiceHandle(hSChandle);
			return FALSE;
		}
	}

	SRVITEM *pTemp = NULL;
	SRVITEM *pLast = NULL;
	for (int nIndex = 0; nIndex < ServicesReturned; nIndex++)
	{
		if (NULL == m_pHead)
		{
			m_pHead = new SRVITEM;
			pTemp = m_pHead;
			pLast = m_pHead;
		}else
		{
			pTemp = new SRVITEM;
			pLast->m_pNext = pTemp;
			pLast = pTemp;
		}

		pLast->m_strSrvName = lpServices[nIndex].lpServiceName;//获取服务的真实名称
		pLast->m_strSrvDispName = lpServices[nIndex].lpDisplayName;//获取服务的显示名称
		pLast->m_dwSrvStatus = lpServices[nIndex].ServiceStatus.dwCurrentState;//获取服务的当前状态
		GetSrvBinPathAndStartStatus(pLast->m_strSrvName, *pLast);//获取服务的路径和启动类型
		GetServiceDescribtion(pLast->m_strSrvName, *pLast);//获取服务的描述信息
	}

	delete[] lpServices;
	return TRUE;
}

CString CSerConfig::GetServiceCurrentStatus(DWORD dwCurrentStatus)
{
	//获取服务的启动信息
	CString strStatus = _T(""); 
	switch (dwCurrentStatus)
	{
	case SERVICE_RUNNING:
		strStatus = _T("已启动");
		break;
	case SERVICE_STOPPED:
		strStatus = _T("已停止");
		break;
	case SERVICE_PAUSED:
		strStatus = _T("已暂停");
		break;
	case SERVICE_START_PENDING:
		strStatus = _T("正在启动");
		break;
	case SERVICE_STOP_PENDING:
		strStatus = _T("正在停止");
		break;
	case SERVICE_CONTINUE_PENDING:
		strStatus = _T("正在继续");
		break;
	case SERVICE_PAUSE_PENDING:
		strStatus = _T("正在暂停");
	default:
		break;
	}

	return strStatus;
}

BOOL CSerConfig::GetSrvBinPathAndStartStatus(CString strSrvName, SRVITEM& SrvItem)
{
	//获取服务的文件路径和启动类型
	BOOL bRet = FALSE;
	SC_HANDLE hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager)
	{
		return bRet;
	}

	SC_HANDLE hSCService = OpenService(hSCManager, strSrvName, SERVICE_ALL_ACCESS);
	if (NULL == hSCService)
	{
		CloseServiceHandle(hSCManager);
		return bRet;
	}

	LPQUERY_SERVICE_CONFIG lpServiceConfig = NULL;
	DWORD cbBytesNeeded = 0;
	bRet = ::QueryServiceConfig(hSCService, lpServiceConfig, 0, &cbBytesNeeded);
	if (!bRet && ERROR_INSUFFICIENT_BUFFER == GetLastError())
	{
		lpServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[cbBytesNeeded];
		ZeroMemory(lpServiceConfig, cbBytesNeeded);
		DWORD dwBufCount = cbBytesNeeded;
		bRet = ::QueryServiceConfig(hSCService, lpServiceConfig, dwBufCount, &cbBytesNeeded);

		if (!bRet)
		{
			CloseServiceHandle(hSCService);
			CloseServiceHandle(hSCManager);
			return bRet;
		}
	
		SrvItem.m_dwSrvStartStatus = lpServiceConfig->dwStartType;
		SrvItem.m_strBinPath = lpServiceConfig->lpBinaryPathName;
		SrvItem.m_strDependSev = lpServiceConfig->lpDependencies;
		delete[] lpServiceConfig;
	}

	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCManager);
	return bRet;
}

CString CSerConfig::GetServiceStartStatus(DWORD dwStartStatus)
{	
	//填充启动类型字符串
	CString strStartType = _T("");
	switch (dwStartStatus)
	{
	case SERVICE_AUTO_START:
		strStartType = _T("自动");
		break;
	case SERVICE_DEMAND_START:
		strStartType = _T("手动");
		break;
	case SERVICE_DISABLED:
		strStartType = _T("禁止");
		break;
	default:
		strStartType = _T("未知");
		break;
	}

	return strStartType;
}

BOOL CSerConfig::GetServiceDescribtion(CString strSrvName, SRVITEM& SrvItem)
{
	//获取描述信息
	BOOL bRet = FALSE;
	SC_HANDLE h_SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == h_SCManager)
	{
		return bRet;
	}

	SC_HANDLE h_SCService = OpenService(h_SCManager, strSrvName, SERVICE_ALL_ACCESS);
	if (NULL == h_SCService)
	{
		CloseServiceHandle(h_SCManager);
		return bRet;
	}
	LPSERVICE_DESCRIPTION lpBuf = NULL;
	DWORD BufSize = 0;
	DWORD cbByteNeeded = 0;
	bRet = ::QueryServiceConfig2(h_SCService, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)lpBuf, BufSize, &cbByteNeeded);
	if (!bRet && ERROR_INSUFFICIENT_BUFFER == GetLastError())
	{
		BufSize = cbByteNeeded;
		lpBuf = (LPSERVICE_DESCRIPTION)new char[BufSize + 1];
		bRet = ::QueryServiceConfig2(h_SCService, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)lpBuf, BufSize, &cbByteNeeded);
		if (!bRet)
		{
			CloseServiceHandle(h_SCService);
			CloseServiceHandle(h_SCManager);
			return bRet;
		}

		bRet = TRUE;
		SrvItem.m_strDescribInfo = lpBuf->lpDescription;

		CloseServiceHandle(h_SCService);
		CloseServiceHandle(h_SCManager);
		return bRet;
	}

	CloseServiceHandle(h_SCService);
	CloseServiceHandle(h_SCManager);
	return bRet;
}

SRVITEM &CSerConfig::GetSrvItemByIndex(int nIndex)
{
	iterator it = begin();
	for (int i = 0; it != end(); i++)
	{
		if (i == nIndex)
		{
			break;
		}

		++it;
	}

	return *it.pItem;
}

long CSerConfig::GetSrvCtrlAccept(CString strSrvName, DWORD *pCurrentStatus)
{
	long lAcceptCtrl = -1;

	SC_HANDLE h_SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == h_SCManager)
	{
		return -1;
	}

	SC_HANDLE h_SCServ = OpenService(h_SCManager, strSrvName, SERVICE_ALL_ACCESS);
	if (NULL == h_SCServ)
	{
		CloseServiceHandle(h_SCManager);
		return -1;
	}

	SERVICE_STATUS sts = {0};
	if (!QueryServiceStatus(h_SCServ, &sts))
	{
		CloseServiceHandle(h_SCServ);
		CloseServiceHandle(h_SCManager);
		return -1;
	}

	lAcceptCtrl = sts.dwControlsAccepted;
	ASSERT(pCurrentStatus);
	*pCurrentStatus = sts.dwCurrentState;
	
	CloseServiceHandle(h_SCServ);
	CloseServiceHandle(h_SCManager);
	return lAcceptCtrl;
}

BOOL CSerConfig::CtrlServiceStatus(CString strSrvName, DWORD dwNewStatus)
{
	BOOL bRet = FALSE;
	SC_HANDLE h_SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == h_SCManager)
	{
		return bRet;
	}

	SC_HANDLE h_SCSrv = OpenService(h_SCManager, strSrvName, SERVICE_ALL_ACCESS);
	if(NULL == h_SCSrv)
	{
		CloseServiceHandle(h_SCManager);
		return bRet;
	}

	SERVICE_STATUS sts = {0};
	if(!QueryServiceStatus(h_SCSrv, &sts))
	{
		CloseServiceHandle(h_SCSrv);
		CloseServiceHandle(h_SCManager);
		return bRet;
	}

	if (sts.dwCurrentState == dwNewStatus)//当前状态与修改状态一致
	{
		CloseServiceHandle(h_SCSrv);
		CloseServiceHandle(h_SCManager);
		return TRUE;
	}else if (SERVICE_START_PENDING == sts.dwCurrentState || 
			SERVICE_STOP_PENDING == sts.dwCurrentState || 
			SERVICE_CONTINUE_PENDING == sts.dwCurrentState || 
			SERVICE_PAUSE_PENDING == sts.dwCurrentState)
	{
		CloseServiceHandle(h_SCSrv);
		CloseServiceHandle(h_SCManager);
		return FALSE;
	}else if (SERVICE_STOPPED == sts.dwCurrentState && SERVICE_START == dwNewStatus)//当前状态为停止，需要启动服务
	{
		bRet = StartService(h_SCSrv, NULL, NULL);
		CloseServiceHandle(h_SCSrv);
		CloseServiceHandle(h_SCManager);
		return bRet;
	}else if ((SERVICE_RUNNING == sts.dwCurrentState || SERVICE_PAUSED == sts.dwCurrentState) && SERVICE_STOP == dwNewStatus)//当前状态为已启动或者暂停，需要停止服务
	{
		bRet = ControlService(h_SCSrv, SERVICE_CONTROL_STOP, &sts);
		CloseServiceHandle(h_SCSrv);
		CloseServiceHandle(h_SCManager);
		return bRet;
	}else if (SERVICE_PAUSED == sts.dwCurrentState && SERVICE_START == dwNewStatus)//当前状态为暂停，需要继续运行服务
	{
		bRet = ControlService(h_SCSrv, SERVICE_CONTROL_CONTINUE, &sts);
		CloseServiceHandle(h_SCSrv);
		CloseServiceHandle(h_SCManager);
		return bRet;
	}else if (SERVICE_RUNNING == sts.dwCurrentState && SERVICE_PAUSED == dwNewStatus)//当前状态为已启动，需要修改为暂停
	{
		bRet = ControlService(h_SCSrv, SERVICE_CONTROL_PAUSE, &sts);
		CloseServiceHandle(h_SCSrv);
		CloseServiceHandle(h_SCManager);
		return bRet;
	}
}

BOOL CSerConfig::ChangeServiceType(CString strSrvName, DWORD dwNewStartType)
{
	SC_HANDLE h_SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(NULL == h_SCManager)
	{
		return FALSE;
	}

	SC_HANDLE h_Service = OpenService(h_SCManager, strSrvName, SERVICE_ALL_ACCESS);
	if(NULL == h_Service)
	{
		CloseServiceHandle(h_SCManager);
		return FALSE;
	}

	BOOL bRet = ChangeServiceConfig(h_Service, SERVICE_WIN32_OWN_PROCESS, dwNewStartType, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	CloseServiceHandle(h_Service);
	CloseServiceHandle(h_SCManager);
	return bRet;
}
void CSerConfig::Clear()
{
	SRVITEM *pItem = m_pHead;
	SRVITEM *pTemp = pItem;
	while (NULL != pItem)
	{
		pTemp = pItem;
		delete pItem;
		pItem->m_pNext = NULL;
		pItem = pItem->m_pNext;
	}

	m_pHead = NULL;
	pItem = NULL;
	pTemp = NULL;
}

BOOL CSerConfig::DeleteService(SRVITEM &SrvItem)
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager)
	{
		return FALSE;
	}
	
	SC_HANDLE hSCService = OpenService(hSCManager, SrvItem.m_strSrvName, SERVICE_ALL_ACCESS);
	if (NULL == hSCService)
	{
		CloseServiceHandle(hSCManager);
		return FALSE;
	}

	BOOL bRet = FALSE;
	//要删除服务，该服务必须是停止状态
	if (SERVICE_STOPPED != SrvItem.m_dwSrvStatus)
	{
		bRet = CtrlServiceStatus(SrvItem.m_strSrvName, SERVICE_CONTROL_STOP);
		if (bRet)
		{
			bRet = ::DeleteService(hSCService);
		}
	}

	bRet = ::DeleteService(hSCService);
	
	return bRet;
}

BOOL  CSerConfig::CreateServices(CString &strSrvName, CString &strDisName, DWORD dwStartType, LPCTSTR lpszBinaryPathName, LPCTSTR lpszSrvDesc)
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCManager)
	{
		return FALSE; 
	}
 
	SC_HANDLE hSCRet = NULL;
	hSCRet = ::CreateService(hSCManager, strSrvName, strDisName, SC_MANAGER_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, dwStartType, SERVICE_ERROR_NORMAL, lpszBinaryPathName, NULL, NULL, NULL, NULL, NULL);
	if (NULL == hSCRet)
	{
		CloseServiceHandle(hSCManager);
		return FALSE;
	}

	if (NULL != lpszSrvDesc)
	{
		SERVICE_DESCRIPTION SrvDesc = {0};
		LPTSTR lpszBuf = (LPTSTR)new TCHAR[4096];
		_tcscpy_s(lpszBuf, 4096, lpszSrvDesc);
		SrvDesc.lpDescription = lpszBuf;
		ChangeServiceConfig2(hSCRet, SERVICE_CONFIG_DESCRIPTION, &SrvDesc);
	}

	return TRUE;
}
