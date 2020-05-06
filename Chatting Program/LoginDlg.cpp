// Login.cpp: 구현 파일
//

#include "pch.h"
#include "Chatting Program.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "ServerMgr.h"


// Login 대화 상자

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Login, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editID);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &LoginDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

// Login 메시지 처리기


void LoginDlg::OnBnClickedButton1()
{
	if (ServerMgr::GetInst()->Connect())
	{
		CString strText;
		CString strSend = TEXT("0");
		m_editID.GetWindowTextW(strText);

		strSend += strText;
		ServerMgr::GetInst()->Send(strSend);
		int iResult = 0;
		ServerMgr::GetInst()->SetMyName(strText);
		EndDialog(iResult);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}