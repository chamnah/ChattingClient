// Room.cpp: 구현 파일
//

#include "pch.h"
#include "Chatting Program.h"
#include "Room.h"
#include "afxdialogex.h"
#include "ConverseDlg.h"


// Room 대화 상자

IMPLEMENT_DYNAMIC(Room, CDialogEx)

Room::Room(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Room, pParent)
{
    bColor = false;
    m_pConverse = nullptr;
    m_bMulti = false;
}

Room::~Room()
{
}

void Room::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MFCBUTTON1, m_btnRoom);
    DDX_Control(pDX, IDC_EDIT1, m_editRecvName);
    DDX_Control(pDX, IDC_EDIT2, m_editText);
}


BEGIN_MESSAGE_MAP(Room, CDialogEx)
	ON_WM_CTLCOLOR()
    ON_BN_DOUBLECLICKED(IDC_MFCBUTTON1, &Room::OnBnDoubleclickedMfcbutton1)
END_MESSAGE_MAP()


// Room 메시지 처리기


HBRUSH Room::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (nCtlColor)
    {
        case CTLCOLOR_EDIT:
        case CTLCOLOR_MSGBOX:
        case CTLCOLOR_STATIC:
        {
                pDC->SetTextColor(RGB(255, 0, 0));
                pDC->SetBkColor(RGB(255, 255, 255));
                pDC->SetBkMode(TRANSPARENT);
                if (!bColor)
                {
                    bColor = true;
                
                }
                return (HBRUSH)GetStockObject(WHITE_BRUSH);
                break;
        }
        case CTLCOLOR_DLG:
        {
            pDC->SetBkColor(RGB(255, 255, 255));
            hbr = ::CreateSolidBrush(RGB(0, 0, 255));
         //   return (HBRUSH)GetStockObject(NULL_BRUSH);;
        }
    }
	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL Room::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_editRecvName.SetParent(&m_btnRoom);
    m_editText.SetParent(&m_btnRoom);
    
    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    //m_editRecvName.SetParent(&m_btnRoom);
    //m_editRecvName.SetWindowTextW(TEXT("AAA"));
    /*
    CStatic* pText = new CStatic;
    pText->Create(TEXT("보낸 사람 : "), WS_CHILD | WS_VISIBLE, rt, &m_btnRoom, IDC_STATIC1);*/
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void Room::OnBnDoubleclickedMfcbutton1()
{
    if (m_pConverse)
    {
        ConverseDlg* pConverse = new ConverseDlg;
        pConverse->Create(IDD_Room);
    }
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}