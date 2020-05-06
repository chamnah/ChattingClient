#pragma once


// AddGroup 대화 상자

class AddGroup : public CDialogEx
{
	DECLARE_DYNAMIC(AddGroup)

public:
	AddGroup(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AddGroup();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddGroup };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listFriend;
	virtual BOOL OnInitDialog();
	class ConverseDlg* m_pConverse;
	afx_msg void OnLbnDblclkList1();
};
