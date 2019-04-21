
// ChatServer_IOCPServer_fotMFC.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "ChatServer_IOCPServer_fotMFC.h"
#include "ChatServer_IOCPServer_fotMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatServerIOCPServerfotMFCApp

BEGIN_MESSAGE_MAP(CChatServerIOCPServerfotMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChatServerIOCPServerfotMFCApp 생성

CChatServerIOCPServerfotMFCApp::CChatServerIOCPServerfotMFCApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CChatServerIOCPServerfotMFCApp 개체입니다.

CChatServerIOCPServerfotMFCApp theApp;


// CChatServerIOCPServerfotMFCApp 초기화

BOOL CChatServerIOCPServerfotMFCApp::InitInstance()
{
	
	CWinApp::InitInstance();

	WSADATA temp;
	WSAStartup(0x0202, &temp);

	CChatServerIOCPServerfotMFCDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	WSACleanup();

	/*
	내가 윈도우 소켓 API를 쓰겠다 라는 작업.
	WSADATA temp;
	WSAStartup(0x0202, &temp);
	WSACleanup();
	*/


	return FALSE;
}

