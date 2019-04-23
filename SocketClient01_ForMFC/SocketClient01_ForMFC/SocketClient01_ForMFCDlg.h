
// SocketClient01_ForMFCDlg.h: 헤더 파일
//

#pragma once


// CSocketClient01ForMFCDlg 대화 상자
class CSocketClient01ForMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	CSocketClient01ForMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETCLIENT01_FORMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CListBox m_listEvent;

	SOCKET m_hSocket;

	void SendFrameData(SOCKET parm_hSocket, unsigned char parm_id, const void* parm_p_Data, int parm_Size);

	void ReadFrameData();


	void DestroySocket();

	void ConnectProcess(LPARAM lParam);

	void AddEventString(CString parmString);

	void ReceiveData(SOCKET parm_hSocket, char* parm_p_buffer, int parm_size);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedOk();
};
