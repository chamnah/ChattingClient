#pragma once


// AddFriend 대화 상자

class AddFriend : public CDialog
{
	DECLARE_DYNAMIC(AddFriend)

public:
	AddFriend(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AddFriend();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddFriend };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editName;
	CString m_strName;
	afx_msg void OnBnClickedButton1();
	CEdit m_editMsg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
