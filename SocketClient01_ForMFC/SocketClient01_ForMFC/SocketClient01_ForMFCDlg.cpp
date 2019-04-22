
// SocketClient01_ForMFCDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SocketClient01_ForMFC.h"
#include "SocketClient01_ForMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSocketClient01ForMFCDlg 대화 상자



CSocketClient01ForMFCDlg::CSocketClient01ForMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETCLIENT01_FORMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketClient01ForMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EVENT, m_listEvent);
}

BEGIN_MESSAGE_MAP(CSocketClient01ForMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CSocketClient01ForMFCDlg 메시지 처리기

BOOL CSocketClient01ForMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// socket(채널방식, TCP, 프로토콜 방식);
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in srv_addr;

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr("192.168.159.128"); // 서버 listen 소켓의 ip주소
	srv_addr.sin_port = htons(20001); // 서버 포트와 동일하게

	// FD_CONNECT 서버 접속에대한 결과가 정해지면 m_hWnd에게 27001 번 요청
	WSAAsyncSelect(m_hSocket, m_hWnd, 27001, FD_CONNECT);
	

	// 혹시나 서버가 다운되어있는 상태라면
	// 최대 약 28초정도 응답없음 상태에 빠진다.
	connect(m_hSocket, (sockaddr*)&srv_addr, sizeof(srv_addr));

	//접속 결과의 여부는 27001번의 이벤트가 발생하였을때 판단한다

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSocketClient01ForMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSocketClient01ForMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSocketClient01ForMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSocketClient01ForMFCDlg::AddEventString(CString parmString)
{
	int nIndex = m_listEvent.InsertString(-1, parmString);
	m_listEvent.SetCurSel(nIndex);
}

void CSocketClient01ForMFCDlg::ConnectProcess(LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam) == 0)
	{
		// 접속성공

		// 현재 비동기가 FD_CONNECT로 걸려있으니 바꿔줘야함
		// 서버한테 정보가 오는지 끊겼는지 확인하는 코드
		WSAAsyncSelect(m_hSocket, m_hWnd, 27002, FD_READ | FD_CLOSE);;

		AddEventString(_T("서버에 접속했습니다."));
	}
	else
	{
		DestroySocket();
		AddEventString(_T("서버에 접속할 수 없습니다."));
	}
}

void CSocketClient01ForMFCDlg::DestroySocket()
{
	LINGER temp_linger = { TRUE, 0 };
	// 데이터 오는거 안기다리고 바로 끊기
	setsockopt(m_hSocket, SOL_SOCKET, SO_LINGER, (char*)&temp_linger, sizeof(temp_linger));
	
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET; // 소켓 비워주기

}

void CSocketClient01ForMFCDlg::ReadFrameData()
{
	char key, message_id;
	// 수신 데이터 1바이트만 가져온다 는 결국 헤더에서 맨앞
	recv(m_hSocket, &key, 1, 0);
	if (key == 27) // 신뢰가능한 데이터 판단
	{
		unsigned short int body_size;
		recv(m_hSocket, (char*)&body_size, 2, 0);
		recv(m_hSocket, &message_id, 1, 0);
	}
	else // 27이 아니면 잘못된 프로토콜
	{
		DestroySocket();
		AddEventString(_T("잘못된 프로토콜입니다."));
	}
}

LRESULT CSocketClient01ForMFCDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (message == 27001) // 무조건 FD_CONNECT 이벤트
	{
		ConnectProcess(lParam);
	}
	else if (message == 27002) // FD_READ or FD_CLOSE
	{
		// 매크로 함수 이용해서 lParam에서 이벤트 파트만 때어내서 확인
		if (WSAGETSELECTEVENT(lParam) == FD_READ)
		{
			// 무한 호출 방지
			WSAAsyncSelect(m_hSocket, m_hWnd, 27002, FD_CLOSE);

			ReadFrameData();
			
			// 이미 파괴된 소켓이면 비동기 걸면안되서
			if (m_hSocket != INVALID_SOCKET)
			{
				WSAAsyncSelect(m_hSocket, m_hWnd, 27002, FD_READ | FD_CLOSE);
			}
		}
		else // FD_CLOSED의 뜻은 내가 끊은게 아니라 상대방(서버)가 나를 끊었다.
		{
			DestroySocket();
			AddEventString(_T("서버에서 연결을 해제하였습니다."));
		}
	}



	return CDialogEx::WindowProc(message, wParam, lParam);
}
