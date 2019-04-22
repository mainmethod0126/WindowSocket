﻿
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

	void ReadFrameData();


	void DestroySocket();

	void ConnectProcess(LPARAM lParam);

	void AddEventString(CString parmString);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
