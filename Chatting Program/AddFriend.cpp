// AddFriend.cpp: 구현 파일
#include "pch.h"
#include "Chatting Program.h"
#include "AddFriend.h"
#include "afxdialogex.h"
#include "ServerMgr.h"
// AddFriend 대화 상자

IMPLEMENT_DYNAMIC(AddFriend, CDialog)

AddFriend::AddFriend(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_AddFriend, pParent)
	, m_strName("")
{
}

AddFriend::~AddFriend()
{
}

void AddFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editName);
	DDX_Control(pDX, IDC_EDIT2, m_editMsg);
}


BEGIN_MESSAGE_MAP(AddFriend, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &AddFriend::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// AddFriend 메시지 처리기


void AddFriend::OnBnClickedButton1()
{
	CString strName;
	m_editName.GetWindowTextW(strName);
	
	CString strSend = TEXT("1");
	strSend += strName;
	ServerMgr::GetInst()->Send(strSend);
	ServerMgr::GetInst()->SetFindUser(NONE);
	m_editMsg.SetWindowTextW(TEXT(""));
}

void AddFriend::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1200)
	{
		if (ServerMgr::GetInst()->GetFindUser() == FIRST)
		{
			ServerMgr::GetInst()->SetFindUser(NONE);
			int iResult = 0;
			m_editName.GetWindowTextW(m_strName);

			EndDialog(iResult);
		}
		else if (ServerMgr::GetInst()->GetFindUser() == SECOND)
		{
			ServerMgr::GetInst()->SetFindUser(NONE);
			m_strName = "";
			m_editMsg.SetWindowTextW(TEXT("없는 이름입니다."));
		}
		// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL AddFriend::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1200, 500, NULL);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}