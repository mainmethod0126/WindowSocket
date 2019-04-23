
// ChatServer_IOCPServer_fotMFCDlg.cpp: 구현 파일
//



#include "stdafx.h"
#include <stdio.h>
#include "ChatServer_IOCPServer_fotMFC.h"
#include "ChatServer_IOCPServer_fotMFCDlg.h"
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


// CChatServerIOCPServerfotMFCDlg 대화 상자



CChatServerIOCPServerfotMFCDlg::CChatServerIOCPServerfotMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_IOCPSERVER_FOTMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nClinetCount = 0;
}

void CChatServerIOCPServerfotMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListEvent);
}

BEGIN_MESSAGE_MAP(CChatServerIOCPServerfotMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CChatServerIOCPServerfotMFCDlg 메시지 처리기

BOOL CChatServerIOCPServerfotMFCDlg::OnInitDialog()
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


	// 소켓 만들기 일종의 전화를 위한 핸드폰 만들기
	// AF_INET : 일종의 KT, SKT, LG 주파수 대역망 처럼 주소망 웬만한 소켓이랑 다 호환됨
	// TCP : 보통 1:1 방식
	// UDP : 보통 N : N, 1 : N 등 브로드케스트처럼 방송하는 느낌으로 할때 사용함
	// SOCK_STREAM : TCP 방식
	// 0 은 프로토콜을 앞에 TCP or UDP에 따라 알아서 함수자체가 프로토콜을 정해줌
	m_hListenSocket = socket(AF_INET, SOCK_STREAM, 0); // 여기까지가 핸드폰을 만들었다는 느낌임.

	// 내 네트워크 카드에 소켓을 어떤방식으로 쓰겠다 할때 씀
	struct sockaddr_in srv_Addr;
	
	// 어떤 주소체계를 사용할지 정해야함
	srv_Addr.sin_family = AF_INET;
	
	// 내 컴퓨터에 있는 어떤 네트워크 카드를 사용할지.
	// 노트북 같은경우는 무선, 유선 해서 랜카드가 보통 2개 둘다 ip가 다르게 부여되기에 어떤거를 사용하지 정해주어야함
	// s_addr = 저장될때 1바이트짜리 4개의 정수로 저장되기 때문에 문자열을  ip처럼 바꿔줘야함 . 기준으로 자름
	// "127.0.0.1" 자기 자신 네트워크
	//srv_Addr.sin_addr.s_addr = inet_addr("192.168.245.131");

	srv_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	// 아이피를 통해서 서버 PC에는 접근할 수 있는데 그 서버에서 어떤 프로그램에 접근할지는 Port를 통하여 구별가능
	// 서버에 여러가지 네트워크 서비스가 이루어지고 있을 수 있으니 포트를 명시해줘야함
	// 어떤 항구 번호로 들어갈지
	// 이 프로그램이 사용 포트를 20001로 하겠다
	// htons : 리눅스는 빅인디안 사용하고 인텔은 리틀인디알 쓰기 때문에 리틀을쓰든 빅인디안을쓰든 통신이 될 수 있도록 맞춰주는 함수 htons()
	// 여기까지가 핸드폰 개통서류를 만든거임.
	srv_Addr.sin_port = htons(20001);

	// 핸드폰 개통하는 함수
	// 어떤거 등록할거니? 소켓 변수, 어디다 등록할거니? 정보적어놓은 서류, 사이즈는 서류 내용만큼 
	// 여기까지가 소켓(핸드폰)만들고, 어떤방식으로 통신할건지 서류정보(개통서류) 작성하고, 그거를 네트워크 카드에 등록(개통) 하는게 끝남
	bind(m_hListenSocket, (LPSOCKADDR)&srv_Addr, sizeof(srv_Addr));


	// 이제 핸드폰 키는거임, 전화 받을 수 있음
	// 1은 현재 접속을 받아주는 소켓이 동시에 여러 클라이언트가 접속했을때 한번에 몇명을 받을지, 5명접속해도 1명만 처리함 나머지는 다시 접속해야함
	listen(m_hListenSocket, 1);

	AddEventString(_T("클라이언트의 접속을 허락합니다.. "));


	// accept()를 호출하면 스레드가 클라이언트가 접속할때까지 대기상태로 들어간다.
	// 그것을 해결하기 위해 동기, 비동기 두 방식을 선택할 수 있는데
	// 동기는 스레드 써야하고
	// 비동기는 운영체제의 소켓매니저한테 소켓에 무슨 신호가 오면 나한테 알려줘~ 명령내리고 운영체제로부터 소켓의 상태값을 받는거임
	// 클라이언트가 접속,접속해제 등의 상태를 확인가능
	// WSAAsyncSelect : 소켓에 특정 비동기를 걸어둘 수 있음. 소켓에 상태가 변경되면 스레드한테 알려줘라. 나는 다른거 하고있을게~
	// FD_ACCEPT : 지금 소켓에 누가 접속을 시도한다 라는 이벤트를 뜻함
	// 함수를 설명하자면 m_hListenSocket 에 누가 접속하려하면, m_hWnd(현재 대화상자)에게 27001 번을 달라고 한다.
	// 결과적으로 클라이언트가 해당 소켓에 붙으려고하면 해당 대화상자가 27001 이라는 이벤트 신호를 보냄 그럼 나는 27001신호가 오면 아! 클라이언트가 접속하는구나 알 수 있음
	WSAAsyncSelect(m_hListenSocket, m_hWnd, 27001, FD_ACCEPT);


	//accept();





	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatServerIOCPServerfotMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatServerIOCPServerfotMFCDlg::OnPaint()
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
HCURSOR CChatServerIOCPServerfotMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatServerIOCPServerfotMFCDlg::AcceptProcess(SOCKET parm_hSocket)
{
	if (m_nClinetCount <= MAX_CLIENT_COUNT)
	{
		// 소켓핸들이지만 프로그램에 소켓이 여러개일 수 있으니 가져온걸로 하나 만들어서 쓰는게 좋음
		// 캐스팅 가능
		SOCKET hSocket = (SOCKET)parm_hSocket; // m_hListenSocket 핸들값과 같음

		// 클라이언트의 정보를 저장할 서류.
		struct sockaddr_in client_addr;

		int socketaddr_in_size = sizeof(client_addr);

		// 이미 클라이언트가 들어온거니까 accept 함수 바로 빠져나간다
		// Listen 소켓은 클라이언트 접속 확인 담당이니까 클라이언트랑 직접 통신하는 새로운 소켓은 만들어주어야 한다
		// 매개변수 1번 : 어떤 소켓을 복사할거냐
		// 매개변수 2번 : 어떤 IP 를 가진 클라이언트가 들어왔는지 확인하는 매개변수;
		// 맴버배열로 넣어주는 이유는 이 함수를 빠져나가면 소켓이 사라지니까 통신을 계속 이어갈 수 있도록.
		m_hClientlist[m_nClinetCount] = accept(hSocket, (LPSOCKADDR)&client_addr, &socketaddr_in_size);

		// 비동기 걸기
		// 상대방이 나한테 값을 보내고 있는지 연결을 끊었는지는 모른다.
		// 그렇기에 비동기 걸고 FD_READ : 데이터를 받았다, FD_CLOSE 상대방이 끊었다. 라는 신호가 왔을때 27002번이 들어온다
		WSAAsyncSelect(m_hClientlist[m_nClinetCount], m_hWnd, 27002, FD_READ | FD_CLOSE); // 이벤트 호출이니까 WindowProc()함수가 호출된다.

		CString strIPAddress;
		strIPAddress = inet_ntoa(client_addr.sin_addr);
		wcscpy_s(m_Client_IP[m_nClinetCount], strIPAddress);

		AddEventString(_T("새로운 클라이언트가 접속했습니다 : " + strIPAddress));

		m_nClinetCount++;

	}
	else
	{

	}
}

// 리스트 뷰 관리
void CChatServerIOCPServerfotMFCDlg::AddEventString(CString parm_string)
{
	// InserString 이면 맨 마지막 인덱스에 추가됨
	int nIndex = m_ListEvent.InsertString(-1, parm_string);
	m_ListEvent.SetCurSel(nIndex); // 자동 스크롤
}




void CChatServerIOCPServerfotMFCDlg::ClientCloseProcess(SOCKET parm_hSocket, char parm_force_flag)
{

	if (parm_force_flag == 1)
	{
		//LINGER 소켓을 끊을때 어떻게 끊을까를 명시하는 것
		LINGER linger = { TRUE, 0 }; // 데이터를 받고있거나 말거나 즉각적으로 바로 소켓을 끊겠다 라는 옵션

		// 소켓의 여러가지 옵션들을 바꿔줄 수 있음
		// parm_hSocket 소켓을 즉각적으로 끊어지게 만들게 옵션을 바꾼다.
		setsockopt(parm_hSocket, SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof(linger));
	}

	// closesocket는 현재 소켓으로부터 데이터를 받고있으면 기다렸다가 닫게된다.
	// 내가 원하는 클라이언트가 아니라면 데이터 송수신없이 바로 끊어버려야한다.
	closesocket(parm_hSocket); // 소켓 닫아버리면 된다.

	for (int i = 0; i < m_nClinetCount; i++)
	{
		// 나간 소켓이 발견되면
		// 나간놈 비워주고 맨 마지막에를 가져다가 그 인덱스로 옮겨준다
		if (m_hClientlist[i] == parm_hSocket)
		{
			m_nClinetCount--;
			if (i != m_nClinetCount)
			{
				m_hClientlist[i] = m_hClientlist[m_nClinetCount];
				// ip도 소켓과 같이 같은 인덱스로 관리 됨
				wcscpy_s(m_Client_IP[i], m_Client_IP[m_nClinetCount]);
				m_hClientlist[m_nClinetCount] = NULL;
			}
		}
	}
}


void CChatServerIOCPServerfotMFCDlg::ReceiveData(SOCKET parm_hSocket, char* parm_p_buffer, int parm_size)
{
	int current_size, total_size = 0, retry_count = 0;

	// total_size 가 body_size 보다 작다면
	// 다 받을때까지 아래 작업들을 반복해야한다.
	while (total_size < parm_size)
	{
		// body_size 만큼 읽어서 p_body_date에 저장해라
		// recv 함수는 반드시 성공하는 함수가 아니다
		// 두 피씨간의 속도차이로 인해서 1000바이트를 보낸다고 해도 100~200~300 이런식으로 나눠서 들어와서 합산해서 1000바이트 들어오는 경우가 있다.
		// 그렇기에 한번에 다 받아내지 못했을경우 재시도해서 다 받게만들어야한다.
		// 읽다가 클라이언트가 끊어졌을 경우 current_size 에 에러값이 들어올수도 있다.
		// body_size - total_size : 받아야하는 바디사이즈가 200인데 이미 100을 받았을 경우 또 200을 받으면 안되니까 100만큼만 받아야한다
		// p_body_date + body_size : 받아서 저장할 메모리의 시작 주소인데 계속 처음부분에다가 쓰면 덮어쓰기니까
		// 실제로 받은 사이즈 뒤에 다가 써야지 실제 모든 데이터를 받을 수 있음
		current_size = recv(parm_hSocket, parm_p_buffer + total_size, parm_size - total_size, 0);

		// 에러가 발생하자마자 브레이크 걸면 프로그램 깨질수도있다
		// 에러가 발생하면 재시도를 몇번정도 진행해주자
		if (current_size == SOCKET_ERROR)
		{
			retry_count++;
			// 5번정도면 정말 빨리 재시도가 끝나기에 sleep으로 딜레이를 주자
			// 근데 문제는 Sleep 존나 안좋은 방법 다른 좋은 방법도 있을것이다.
			// 0.05초 6번이니까 0.3초정도 재시도시간을 준다
			Sleep(50);
			if (retry_count > 5) break;

		}
		else
		{
			// 에러가 났다가 말았다가 났다가 말았다가 할경우 에러 카운트를 초기화 해줘야한다.
			retry_count = 0;

			// 에러가 나지않았다면 받아온값을 누적해서
			// 받은 토탈사이즈가 body_size와 같다면 데이터를 다 받았다는 소리다.
			total_size = total_size + current_size;
		}
	}
}

// parm_p_Data 파일, 스트링, 등등 어떤거든 보낼 수 있도록 void*
// parm_hSocket 을 이용해서 parm_p_Data 있는 데이터를 parm_Size 크기 만큼 보내겠다.
void CChatServerIOCPServerfotMFCDlg::SendFrameData(SOCKET parm_hSocket, unsigned char parm_id, const void* parm_p_Data, int parm_Size)
{
	// + 4는 앞에 헤더가 4byte라서
	char* p_send_data = new char[parm_Size + 4];

	// 첫번째 1byte에 정상 프로토콜 확인 여부 27
	p_send_data[0] = 27;
	// 두번재 2byte에 넣기위해 2byte를 차지하도록 캐스팅하고 body 사이즈를 넣어주는 곳이니 parm_size넣어준다
	*(unsigned short int*)(p_send_data + 1) = parm_Size;
	// id값
	*(p_send_data + 3) = parm_id;

	memcpy(p_send_data + 4, parm_p_Data, parm_Size);

	send(parm_hSocket, p_send_data, parm_Size + 4, 0);

	delete[] p_send_data;

}



LRESULT CChatServerIOCPServerfotMFCDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// wParam : m_hListenSocket 핸들 정보가 들어있고
	// lParam : 소켓에 에러가 있는지, 어떤 이벤트 때문에 이 메세지가 발생했는지의 정보가 들어있음.
	if (message == 27001)
	{
		AcceptProcess((SOCKET)wParam);
	}
	else if (message == 27002) // FD_READ, FD_CLOSE
	{
		SOCKET hSocket = (SOCKET)wParam;

		// 32bit lParam에서 어떤 이벤트 정보가 들어있는지 담겨있는 하위인지 상위인지 모르는 16bit를 빼서 반환해줌
		// FD_READ 발생 조건
		// 1 : 수신 버퍼가 다 비어있을때 새로운 데이터가 들어오면 발생
		// 2 : 비동기 거는 함수를 가지고 FD_READ에 관심있다고 걸때 수신 버퍼에 데이터가 1바이트라도 있으면 발생
		// 3 : recv함수를 통하여 수신버퍼에서 값을 가져왔을때 아직 수신버퍼에 값이 남아있다면 발생
		// 송신측에서 데이터를 보낼때 아무 약속도 없이 전송하면 수신측에서 굉장히 데이터 받음에 어려움이 있기에
		// 서로 약속을 정하기 위해 헤더를 만들어서 헤더에 내가 얼마만큼 주겠다, 어떤 종류의 메세지이다, 어떤 종류의 구별자이다 를 정의하고
		// 수신측은 헤더가 반드시 들어온다고 생각하고 받고 그 헤더를 통해서 나머지 받을 데이터를 유추해서 코드를 구성한다.

		// 헤더는 4byte크기의 헤더를 사용하며
		// 첫번째는 이 프로토콜이 정상적인, 내가 원하는 클라이언트가 보낸건지 구별하는 데이터가 1byte 나오고
		// 두번째는 Body크기 실제 데이터가 얼마인지의 정보가 2byte가 들어오고
		// 세번째로는 이 데이터가 어떤데이터인지 1byte가 들어온다
		if (WSAGETSELECTEVENT(lParam) == FD_READ)
		{
			// 메세지를 끊어읽으면 계속해서 FD_READ가 발생한다
			// 그렇기에 헤어 앞만 끊어읽을때 FD_CLOSE일경우에만 발생하게 걸어주고
			WSAAsyncSelect(hSocket, m_hWnd, 27002, FD_CLOSE);

			// 서버에는 내가 원하지 않는 프로그램이 접근할 수 있으니
			// 내가 원하는 클라이언트인지 구분하기 위해서
			char key, network_message_id; // 1Byte짜리 char 변수

			// hSocket으로부터 데이터를 수신하겠다.
			// 1Byte만 받겠다.
			// hSocket에서 1Byte만큼만 받아서 key넣는다.
			// 0은 따른 용도로 사용하지 않는다.
			recv(hSocket, &key, 1, 0);		// 여기서 정상적인 클라이언트인지 확인하는 구별자 1Byte 읽고
			
			// 27이면 내가 원하는 프로그램에서 보낸것이 맞다
			if (key == 27)
			{
				// recv로 가져온 값이 1000byte가 됬는지를 확인해야하니까
				int current_size, total_size = 0, retry_count = 0;

				// 데이터를 읽으려고 body부분만 읽어오면되기에
				unsigned short int body_size; // 2byte
				recv(hSocket, (char *)&body_size, sizeof(body_size), 0); // 여기서 2Byte 읽고 실제 데이터의 사이즈가 얼마나 들어왔는지 확인하는 바디사이즈 확인 2byte
				recv(hSocket, &network_message_id, 1, 0); // 여기서 1Byte 읽고 어떤 종류의 데이터가 바디에 들어있는지 1byte 클라이언트와 약속한 값

				// 항상데이터가 있지는 않을 수 있다 bodysize 0 이면 데이터가 없는겨
				if (body_size > 0)
				{
					// 몇이 들어올지 모르니까
					// 캐릭터 배열 동적할당
					// 바디를 저장할 수 있는 메모리 만들었고
					char* p_body_date = new char[body_size];

					// 데이터를 계속 받아주는 작업
					// 1000짜리 한번에 못읽으니까 100, 200, 300 이렇게 나눠서 읽어야함
					ReceiveData(hSocket, p_body_date, body_size);


					// 나중에 이게 1이면 채팅데이터가 2면 로그인 데이터다 이렇게 구분할거임
					if (network_message_id == 1)
					{
						CString str;
						for (int i = 0; i < m_nClinetCount; i++)
						{
							if (hSocket == m_hClientlist[i])
							{
								// 서버측에서 붙여서 보내기, 클라이언트가 보내면 조작의 가능성이 있음
								// "192.168.0.1 : abc"
								str.Format(_T("%s : %s"), m_Client_IP[i], p_body_date);
								break;
							}
						}

						// 실제로 클라이언트가 보내준 데이터를 처리 (p_body_data);
						AddEventString(str); // 유니코드 이기때문에 캐스팅해줘야한다
						
						// 접속된 모든 클라이언트한테 전송. 브로드캐스트
						for (int i = 0; i <m_nClinetCount; i++)
						{
							// 모드 브로드캐스트
							// 1 : 채팅데이터
							// p_body_date : 클라이언트로부터 받은 데이터 그대로 다시보내기
							SendFrameData(m_hClientlist[i], 1, (const wchar_t *)str, (str.GetLength() + 1) * 2);
						}
					}

					delete[] p_body_date;
				}

				// 정상적인 프로그램이고 헤더를 다 읽었다면 다시 FD_READ를 걸어준다
				WSAAsyncSelect(hSocket, m_hWnd, 27002, FD_CLOSE | FD_READ);
			}
			else // 내가 원하는 프로그램이 아니면 그냥 소켓파괴
			{
				// flag를 1을 줘서 데이터를 주고받든말든 바로 소켓을 닫겠다.
				ClientCloseProcess(hSocket, 1);
			}
		}
		else // FD_CLOSE
		{
			// flag를 0으로 줘서 정상적으로
			ClientCloseProcess(hSocket, 0);
			AddEventString(_T("클라이언트가 접속을 해제했습니다."));
		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


