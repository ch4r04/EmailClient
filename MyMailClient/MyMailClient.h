
// MyMailClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyMailClientApp: 
// �йش����ʵ�֣������ MyMailClient.cpp
//

class CMyMailClientApp : public CWinApp
{
public:
	CMyMailClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyMailClientApp theApp;