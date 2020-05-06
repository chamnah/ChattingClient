#pragma once

#define BUF_SIZE 250
#include <unordered_map>
using std::unordered_map;

struct ServerData
{
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
};

class Room;
class ConverseDlg;
class ServerMgr
{
private:
	SOCKET m_hSocket;
	CString strText;
	class CChattingProgramDlg* m_pMainDlg;
	CString m_strMyName;
	unordered_map<string, Room*> m_mapRoom;
	MSG_STATE m_eFindUser;
	list<ConverseDlg*> m_listConverse;
	vector<ConverseDlg*> m_vecDel;
	CListBox* m_listFriend;
	list<Room*> m_listRoom;
	CTabCtrl* m_pTabCtrl;
public:
	static ServerMgr* GetInst()
	{
		static ServerMgr m_Inst;
		return &m_Inst;
	}
	ServerMgr();

	bool Connect();
	void Send(CString& _strText);
	void Recive();
	static void CALLBACK ReciveComplete(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
	void SetMainDlg(CChattingProgramDlg* _pDlg);
	CChattingProgramDlg* GetMainDlg() { return m_pMainDlg; }
	void SetMyName(CString& _strName) { m_strMyName = _strName; }
	CString& GetMyName() { return m_strMyName; }
	bool InsertChatDlg(CString& _strKey);
	MSG_STATE GetFindUser() { return m_eFindUser; }
	void SetFindUser(MSG_STATE _eFind) { m_eFindUser = _eFind; }
	void SliceText(char* _pOri,string* _arrText,int _iCount);
	void SliceText(char* _pOri,string& _First, string& _Second, string& _Third, string& _Four);
	void SliceText(char* _pOri,string& _First, string& _Second, string& _Third);
	void SliceText(char* _pOri,string& _First, string& _Second);
	void PushConverseList(ConverseDlg* _pDlg) { m_listConverse.push_back(_pDlg); }
	void SetFriendList(CListBox* _pList) { m_listFriend = _pList; }
	CListBox* GetFriendList() { return m_listFriend; }
	SOCKET GetSock() { return m_hSocket; }
	vector<ConverseDlg*>& GetDelList() { return m_vecDel; }
	list<Room*>& GetRoomList() { return m_listRoom; }
	void SortRoomList();
	void SetTabCtrl(CTabCtrl* _pTab) { m_pTabCtrl = _pTab; }
};