#pragma once
struct SRVITEM
{
	CString m_strSrvName;//服务名称
	CString m_strSrvDispName;//显示名称
	DWORD m_dwSrvStatus;//服务状态
	DWORD m_dwSrvStartStatus;//启动状态
	CString m_strBinPath;//所在路径
	CString m_strDescribInfo;//描述信息
	CString m_strDependSev;//依赖关系
	SRVITEM *m_pNext;
	SRVITEM()
	{
		m_dwSrvStatus = 0;
		m_pNext = NULL;
	}
};

class CSerConfig
{
public:
	CSerConfig(void);
	~CSerConfig(void);
	BOOL EnumServices();//枚举系统存在的服务
	CString GetServiceCurrentStatus(DWORD dwCurrentStatus);//将服务的当前状态转化为字符串
	BOOL GetSrvBinPathAndStartStatus(CString strSrvName, SRVITEM& SrvItem);//获取服务的路径和启动状态
	CString GetServiceStartStatus(DWORD dwStartStatus);//将服务的启动状态转化为字符串
	BOOL GetServiceDescribtion(CString strSrvName, SRVITEM& SrvItem);//获取服务的描述信息
	SRVITEM &GetSrvItemByIndex(int nIndex);//根据结点的位置查找结点
	long GetSrvCtrlAccept(CString strSrvName, DWORD *pCurrentStatus = NULL);//获取服务的当前状态和可控制状态
	BOOL CtrlServiceStatus(CString strSrvName, DWORD dwNewStatus);//控制服务状态
	BOOL ChangeServiceType(CString strSrvName, DWORD dwNewStartType);//控制服务的启动类型
	void Clear();//清空服务的列表
	BOOL DeleteService(SRVITEM &SrvItem);
	BOOL  CreateServices(CString &strSrvName, CString &strDisName, DWORD dwStartType, LPCTSTR lpszBinaryPathName, LPCTSTR lpszSrvDesc = NULL);//创建服务
	SRVITEM *m_pHead;
	
	//迭代器的定义
	class iterator
	{
	public:
		SRVITEM *pItem;

		void operator++()
		{
			pItem = pItem->m_pNext;
		}

		SRVITEM *operator ->()
		{
			return pItem;
		}

		operator SRVITEM * ()
		{
			return pItem;
		}

		BOOL operator== (iterator &it)
		{
			return (pItem == it.pItem)? TRUE : FALSE;
		}

		BOOL operator!= (iterator &it)
		{
			return (pItem == it.pItem)? FALSE : TRUE;
		}
	};

	iterator begin()
	{
		iterator it;
		it.pItem = m_pHead;
		return it;
	}

	iterator end()
	{
		iterator it;
		it.pItem = NULL;
		return it;
	}
};
