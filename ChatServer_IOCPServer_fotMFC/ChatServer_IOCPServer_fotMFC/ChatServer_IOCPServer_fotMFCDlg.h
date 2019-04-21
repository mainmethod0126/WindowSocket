
// ChatServer_IOCPServer_fotMFCDlg.h: 헤더 파일
//

#pragma once

#define MAX_CLIENT_COUNT 10


// CChatServerIOCPServerfotMFCDlg 대화 상자
class CChatServerIOCPServerfotMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatServerIOCPServerfotMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_IOCPSERVER_FOTMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


private:
	// Client 접속을 받아주는 소켓
	SOCKET m_hListenSocket;

	// 접속한 클라이언트를 관리할 자료구조 배열
	SOCKET m_hClientlist[MAX_CLIENT_COUNT];
	wchar_t m_Client_IP[MAX_CLIENT_COUNT][24];

	int m_nClinetCount;

	void AcceptProcess(SOCKET parm_hSocket);
	void ClientCloseProcess(SOCKET parm_hSocket, char parm_force_flag);
	void ReceiveData(SOCKET parm_hSocket, char* parm_p_buffer, int parm_size);
	void AddEventString(CString parm_string);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	CListBox m_ListEvent;
};
