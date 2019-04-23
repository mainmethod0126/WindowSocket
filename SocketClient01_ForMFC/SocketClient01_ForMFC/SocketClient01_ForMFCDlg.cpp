
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
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSocketClient01ForMFCDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDOK, &CSocketClient01ForMFCDlg::OnBnClickedOk)
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
	srv_addr.sin_addr.s_addr = inet_addr("192.168.245.131"); // 서버 listen 소켓의 ip주소
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

// 데이터를 한번에 다 못받을 수 있으니
// 데이터를 재요청하는 함수
void CSocketClient01ForMFCDlg::ReceiveData(SOCKET parm_hSocket, char* parm_p_buffer, int parm_size)
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

		if (body_size > 0)
		{
			// 데이터 담을 버퍼 만들어주고
			char* pBodyData = new char[body_size];

			ReceiveData(m_hSocket, pBodyData, body_size);
			
			
			if (message_id == 1)
			{
				AddEventString((wchar_t*)pBodyData);
			}
			
			delete [] pBodyData;
		}
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

// parm_p_Data 파일, 스트링, 등등 어떤거든 보낼 수 있도록 void*
// parm_hSocket 을 이용해서 parm_p_Data 있는 데이터를 parm_Size 크기 만큼 보내겠다.
void CSocketClient01ForMFCDlg::SendFrameData(SOCKET parm_hSocket, unsigned char parm_id, const void* parm_p_Data, int parm_Size)
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


void CSocketClient01ForMFCDlg::OnBnClickedButtonSend()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		CString str;
		GetDlgItemText(IDC_EDIT_CHET, str);

		// 채팅 데이터는 메세지 id가 1
		SendFrameData(m_hSocket, 1, (const wchar_t*)str/*연산자 오버로딩 되어있어서 가능*/, (str.GetLength() + 1) * 2 /*문자열 뒤에 공백까지 포함해서 *2는 유니코드이기에 char 가 하나당 2byte이다 그렇기에*/);
	}
	else
	{
		AddEventString(_T("서버에 접속이 되지않은 상태입니다."));
	}

}


void CSocketClient01ForMFCDlg::OnBnClickedOk()
{
	OnBnClickedButtonSend();
	// 보내고 바로 다시
	GotoDlgCtrl(GetDlgItem(IDC_EDIT_CHET));

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}
