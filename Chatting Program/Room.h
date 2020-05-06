#pragma once


// Room 대화 상자
class ConverseDlg;
class Room : public CDialogEx
{
	DECLARE_DYNAMIC(Room)

private:
	bool bColor;

public:
	Room(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Room();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Room };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	CMFCButton m_btnRoom;
	vector<string> m_vecText;
	ConverseDlg* m_pConverse;
	bool m_bMulti;
	CEdit m_editText;
	afx_msg void OnBnDoubleclickedMfcbutton1();
private:
	CString m_strName;
	CString m_strID;
	CEdit m_editRecvName;

public:
	void SetID(CString& _strID) { m_strID = _strID; }
	CString& GetID() { return m_strID; }
	void SetName(CString& _strName) 
	{ 
		m_strName = _strName; 
		m_editRecvName.SetWindowTextW(m_strName); 
	}
	CString& GetName() { return m_strName; }
};