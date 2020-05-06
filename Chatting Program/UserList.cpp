// UserList.cpp: 구현 파일
#include "pch.h"
#include "Chatting Program.h"
#include "UserList.h"
#include "afxdialogex.h"
#include "ServerMgr.h"
#include "ConverseDlg.h"
// UserList 대화 상자

IMPLEMENT_DYNAMIC(UserList, CDialogEx)

UserList::UserList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UserList, pParent)
{
}

UserList::~UserList()
{
}

void UserList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listUser);
}

BEGIN_MESSAGE_MAP(UserList, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &UserList::OnLbnDblclkList1)
END_MESSAGE_MAP()
// UserList 메시지 처리기

void UserList::OnLbnDblclkList1()
{
	int iSel = m_listUser.GetCurSel();
	DWORD_PTR pData = m_listUser.GetItemData(iSel);

	CString strCurName;
	m_listUser.GetText(m_listUser.GetCurSel(), strCurName);


	ConverseDlg* pChat = new ConverseDlg;
	pChat->m_strName = strCurName;
	pChat->Create(IDD_ConverseDlg);
	pChat->ShowWindow(SW_SHOW);
	pChat->m_vecDel.push_back(iSel);

	if (pData)
		pChat->SettingRoom(pData);
	else
	{
		CString strSend;
		strSend = TEXT("3");

		strSend += strCurName + TEXT('/'); // 상대방 이름
		strSend += ServerMgr::GetInst()->GetMyName() + TEXT('\0'); // 나의 이름

		ServerMgr::GetInst()->Send(strSend);
		ServerMgr::GetInst()->PushConverseList(pChat);
	}
	//ServerMgr::GetInst()->InsertChatDlg(strCurName);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL UserList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}