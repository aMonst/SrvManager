
// SrvManager.h : SrvManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSrvManagerApp:
// �йش����ʵ�֣������ SrvManager.cpp
//

class CSrvManagerApp : public CWinApp
{
public:
	CSrvManagerApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSrvManagerApp theApp;
