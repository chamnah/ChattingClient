﻿// Chatting ProgramDlg.cpp: 구현 파일
//
#include "pch.h"
#include "framework.h"
#include "Chatting Program.h"
#include "Chatting ProgramDlg.h"
#include "afxdialogex.h"
#include "AddFriend.h"
#include "ConverseDlg.h"
#include "ServerMgr.h"
#include "UserList.h"
#include "Room.h"

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


// CChattingProgramDlg 대화 상자



CChattingProgramDlg::CChattingProgramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATTINGPROGRAM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChattingProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editMyID);
	DDX_Control(pDX, IDC_TAB1, m_tabList);
}

BEGIN_MESSAGE_MAP(CChattingProgramDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChattingProgramDlg::OnBnClickedButton1)
	ON_LBN_DBLCLK(IDC_LIST1, &CChattingProgramDlg::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChattingProgramDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CChattingProgramDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CChattingProgramDlg 메시지 처리기

BOOL CChattingProgramDlg::OnInitDialog()
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

	m_editMyID.SetWindowTextW(m_strMyID);
	//m_listFriend.AddString(TEXT("Hi"));
	m_tabList.InsertItem(0,TEXT("UserList"));
	m_tabList.InsertItem(1,TEXT("FriendList"));
	m_tabList.InsertItem(2,TEXT("Room"));

	CRect rt;
	m_tabList.GetWindowRect(&rt);

	m_pUserList = new UserList;
	m_pUserList->Create(IDD_UserList, &m_tabList);
	m_pUserList->MoveWindow(5, 25, rt.Width() - 10, rt.Height() - 30);
	m_pUserList->ShowWindow(SW_SHOW);
	
	m_pFriendList = new UserList;
	m_pFriendList->Create(IDD_UserList, &m_tabList);
	m_pFriendList->MoveWindow(5, 25, rt.Width() - 10, rt.Height() - 30);
	m_pFriendList->ShowWindow(SW_HIDE);

	ServerMgr::GetInst()->SetFriendList(&m_pFriendList->m_listUser);
	ServerMgr::GetInst()->SetTabCtrl(&m_tabList);

	/*Room* pRoom = new Room;
	pRoom->Create(IDD_Room, &m_tabList);
	pRoom->MoveWindow(5, 25, rt.Width() - 10, 100);
	pRoom->ShowWindow(SW_HIDE);*/

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChattingProgramDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChattingProgramDlg::OnPaint()
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
HCURSOR CChattingProgramDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChattingProgramDlg::OnBnClickedButton1()
{
	AddFriend Friend;
	Friend.DoModal();
	if(!Friend.m_strName.IsEmpty())
		m_pFriendList->m_listUser.AddString(Friend.m_strName);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CChattingProgramDlg::OnLbnDblclkList1()
{
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CChattingProgramDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN) // ENTER키 눌릴 시
		return TRUE;
	else if (pMsg->wParam == VK_ESCAPE) // ESC키 눌릴 시
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CChattingProgramDlg::OnBnClickedButton2()
{
	closesocket(ServerMgr::GetInst()->GetSock());
	AfxGetMainWnd()->PostMessage(WM_QUIT);
	delete ServerMgr::GetInst()->GetMainDlg();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CChattingProgramDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == 1300)
		ServerMgr::GetInst()->Recive();

	CDialogEx::OnTimer(nIDEvent);
}

void CChattingProgramDlg::AddUserList(CString& _strName)
{
	m_pUserList->m_listUser.AddString(_strName);
}


void CChattingProgramDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	int iCur = m_tabList.GetCurSel();

	list<Room*>::iterator iter = ServerMgr::GetInst()->GetRoomList().begin();
	list<Room*>::iterator iterEnd = ServerMgr::GetInst()->GetRoomList().end();

	switch (iCur)
	{
	case 0:
		m_pUserList->ShowWindow(SW_SHOW);
		m_pFriendList->ShowWindow(SW_HIDE);

		for (; iter != iterEnd; iter++)
			(*iter)->ShowWindow(SW_HIDE);

		break;
	case 1:
		m_pUserList->ShowWindow(SW_HIDE);
		m_pFriendList->ShowWindow(SW_SHOW);
		for (; iter != iterEnd; iter++)
			(*iter)->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pUserList->ShowWindow(SW_HIDE);
		m_pFriendList->ShowWindow(SW_HIDE);
		for (; iter != iterEnd; iter++)
			(*iter)->ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}