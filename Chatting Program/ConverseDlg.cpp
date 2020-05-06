// ConverseDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Chatting Program.h"
#include "ConverseDlg.h"
#include "afxdialogex.h"
#include "ServerMgr.h"
#include "Room.h"
#include "AddGroup.h"

// ConverseDlg 대화 상자

IMPLEMENT_DYNAMIC(ConverseDlg, CDialogEx)

ConverseDlg::ConverseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ConverseDlg, pParent)
	, m_bFocus(false)
{

}

ConverseDlg::~ConverseDlg()
{
}

void ConverseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listChat);
	DDX_Control(pDX, IDC_EDIT1, m_editSend);
	DDX_Control(pDX, IDC_EDIT2, m_editName);
}


BEGIN_MESSAGE_MAP(ConverseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ConverseDlg::OnBnClickedButton1)
	ON_EN_SETFOCUS(IDC_EDIT1, &ConverseDlg::OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &ConverseDlg::OnEnKillfocusEdit1)
	ON_EN_CHANGE(IDC_EDIT1, &ConverseDlg::OnEnChangeEdit1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &ConverseDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// ConverseDlg 메시지 처리기


void ConverseDlg::OnBnClickedButton1()
{
	TextSend();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL ConverseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->wParam == VK_RETURN) // ENTER키 눌릴 시
	{
		if (m_bFocus)
			TextSend();
		return TRUE;
	}
	else if (pMsg->wParam == VK_ESCAPE) // ESC키 눌릴 시
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void ConverseDlg::OnEnSetfocusEdit1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bFocus = true;
}


void ConverseDlg::OnEnKillfocusEdit1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bFocus = false;
}

void ConverseDlg::TextSend()
{
	if (m_editSend.GetWindowTextLengthW() == 0)
		return;

	/*m_listChat.InsertString(m_listChat.GetCount(), TEXT("내가 보낸 말 : "));
	m_listChat.InsertString(m_listChat.GetCount(), m_strText);*/
	CString strMyName = ServerMgr::GetInst()->GetMyName();
	CString strSend;
	CString strTo;
	m_editName.GetWindowTextW(strTo);
	// 현재 채팅 했던 룸이 있다면
	if (pRoom)
	{
		strSend = TEXT("2");
		strSend += pRoom->GetID() + TEXT('/'); // 방 이름
		strSend += strMyName + TEXT('/'); // 나의 이름
		strSend += strTo + TEXT('/'); // 받는 사람
		strSend += m_strText + TEXT('\0'); // 내용
		
	}

	ServerMgr::GetInst()->Send(strSend);
	m_strText = TEXT("");
	strSend = TEXT("");
	m_editSend.SetWindowTextW(TEXT(""));
}

void ConverseDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_editSend.GetWindowTextW(m_strText);
}

BOOL ConverseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editName.SetWindowTextW(m_strName);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void ConverseDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void ConverseDlg::SettingRoom(DWORD_PTR _pRoom)
{
	pRoom = (Room*)_pRoom;
	pRoom->m_pConverse = this;

	CString strText = TEXT("");
	for (size_t i = 0; i < pRoom->m_vecText.size(); i++)
	{
		m_listChat.AddString(CString(pRoom->m_vecText[i].c_str()));
	}
}

void ConverseDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	pRoom->m_pConverse = nullptr;
	ServerMgr::GetInst()->GetDelList().push_back(this);
	CDialogEx::OnCancel();
}


void ConverseDlg::OnBnClickedButton2()
{
	AddGroup Group;
	Group.m_pConverse = this;
	Group.DoModal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void ConverseDlg::SetToName(CString& _strName)
{
	m_strName = _strName;
	m_editName.SetWindowTextW(m_strName);
}