// AddGroup.cpp: 구현 파일
#include "pch.h"
#include "Chatting Program.h"
#include "AddGroup.h"
#include "afxdialogex.h"
#include "ServerMgr.h"
#include "ConverseDlg.h"
#include "Room.h"

// AddGroup 대화 상자

IMPLEMENT_DYNAMIC(AddGroup, CDialogEx)

AddGroup::AddGroup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddGroup, pParent)
{

}

AddGroup::~AddGroup()
{
}

void AddGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listFriend);
}


BEGIN_MESSAGE_MAP(AddGroup, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &AddGroup::OnLbnDblclkList1)
END_MESSAGE_MAP()


// AddGroup 메시지 처리기


BOOL AddGroup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CListBox* pList = ServerMgr::GetInst()->GetFriendList();
	CString strText = TEXT("");
	bool bContinue = false;
	int iNext = 0;
	vector<int>* pDel = &m_pConverse->m_vecDel;
	for (size_t i = 0; i < pList->GetCount(); ++i)
	{
		for (size_t j = 0; j < pDel->size(); j++)
		{
			if (((*pDel)[j]) == i)
			{
				bContinue = true;
			}
		}

		if (bContinue)
		{
			bContinue = false;
			continue;
		}
		pList->GetText(i, strText);
		m_listFriend.InsertString(iNext,strText);
		++iNext;
	}

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void AddGroup::OnLbnDblclkList1()
{
	CString strFriend;
	int iSel = m_listFriend.GetCurSel();
	m_listFriend.GetText(iSel, strFriend);
	m_listFriend.DeleteString(iSel);

	CString strText = TEXT("4");
	if (!m_pConverse->pRoom->m_bMulti)
	{
		strText = TEXT("5");
		m_pConverse->pRoom->m_bMulti = true;
	}

	strText += m_pConverse->pRoom->GetID() + '/' + strFriend + '/' + ServerMgr::GetInst()->GetMyName() + '/' + m_pConverse->m_strName;

	m_pConverse->m_strName += TEXT(",") + strFriend;
	m_pConverse->m_editName.SetWindowTextW(m_pConverse->m_strName);

	ServerMgr::GetInst()->Send(strText);

	
}