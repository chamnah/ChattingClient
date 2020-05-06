#pragma once


// ConverseDlg 대화 상자

class ConverseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConverseDlg)

public:
	ConverseDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ConverseDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ConverseDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listChat;
	afx_msg void OnBnClickedButton1();
	CEdit m_editSend;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnKillfocusEdit1();
	bool m_bFocus;
	void TextSend();
	CString m_strText;
	afx_msg void OnEnChangeEdit1();
	CEdit m_editName;
	CString m_strName;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strRoom;
	class Room* pRoom;
	vector<int> m_vecDel;
public:
	void SettingRoom(DWORD_PTR _pRoom);
	virtual void OnCancel();
	afx_msg void OnBnClickedButton2();
	void SetToName(CString& _strName);
};