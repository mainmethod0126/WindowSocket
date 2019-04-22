﻿
// SocketClient01_ForMFC.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "SocketClient01_ForMFC.h"
#include "SocketClient01_ForMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketClient01ForMFCApp

BEGIN_MESSAGE_MAP(CSocketClient01ForMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSocketClient01ForMFCApp 생성

CSocketClient01ForMFCApp::CSocketClient01ForMFCApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CSocketClient01ForMFCApp 개체입니다.

CSocketClient01ForMFCApp theApp;


// CSocketClient01ForMFCApp 초기화

BOOL CSocketClient01ForMFCApp::InitInstance()
{
	CWinApp::InitInstance();

	WSADATA temp;
	WSAStartup(0x0202, &temp);

	CSocketClient01ForMFCDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	WSACleanup();

	return FALSE;
}

