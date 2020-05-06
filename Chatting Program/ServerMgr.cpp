#include "pch.h"
#include "ServerMgr.h"
#include "Chatting ProgramDlg.h"
#include "ConverseDlg.h"
#include "Room.h"
#include "resource.h"
#include <WinSock2.h>
#include <WS2tcpip.h >
#pragma comment(lib,"ws2_32")

ServerMgr::ServerMgr()
	:m_pMainDlg(nullptr)
	, m_eFindUser(NONE)
{
}

bool ServerMgr::Connect()
{
	WSADATA Data;
	SOCKADDR_IN servAdr;
	if (WSAStartup(MAKEWORD(2, 2), &Data) != 0)
		return false;

	m_hSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_hSocket == INVALID_SOCKET)
		return false;

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servAdr.sin_addr.S_un.S_addr);
	//servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(9100);

	if (connect(m_hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		return false;

	return true;
}

void CALLBACK SendComplete(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
}

void ServerMgr::Send(CString& _strText)
{
	// 0 : 로그인 정보
	// 1 : 유저 검색
	// 2 : 방이 없는 상태에서 채팅을 보낸다.
	// 3 : 방이 할당된 상태에서 채팅을 보낸다.
	// 4 : 그룹 추가

	WSABUF Buff;
	CStringA strText(_strText);
	DWORD SentByte;
	WSAOVERLAPPED overlap;
	memset(&overlap, 0, sizeof(overlap));

	Buff.buf = strText.GetBuffer();
	Buff.len = strText.GetLength() + 1;
	WSASend(m_hSocket, &Buff,1, &SentByte, 0,&overlap, SendComplete);
}

void ServerMgr::Recive()
{
	for (size_t i = 0; i < m_vecDel.size(); i++)
		delete m_vecDel[i];

	m_vecDel.clear();
	SleepEx(100, TRUE);
	DWORD dwNum = 0;
	DWORD dwFlag = 0;

	LPWSAOVERLAPPED overlap = (LPWSAOVERLAPPED)(malloc(sizeof(WSAOVERLAPPED)));
	memset(overlap, 0, sizeof(WSAOVERLAPPED));
	
	ServerData* RecvData = (ServerData*)(malloc(sizeof(ServerData)));
	memset(RecvData, 0, sizeof(ServerData));

	RecvData->wsaBuf.buf = RecvData->buffer;
	RecvData->wsaBuf.len = BUF_SIZE;

	overlap->hEvent = (HANDLE)RecvData;

	WSARecv(m_hSocket, &RecvData->wsaBuf,1, &dwNum,&dwFlag, overlap, ReciveComplete);
}

void ServerMgr::ReciveComplete(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	WSABUF* Data = (WSABUF*)(lpOverlapped->hEvent);
	
	if (Data->buf[0] == '0')
	{
		int i = 1;
		CString strText;
		while(true)
		{
			if (Data->buf[i] == '0')
			{
				GetInst()->m_pMainDlg->AddUserList(strText);
				strText = TEXT("");
			}
			else if (Data->buf[i] == '\0')
			{
				GetInst()->m_pMainDlg->AddUserList(strText);
				break;
			}
			else
			{
				strText += Data->buf[i];
			}

			++i;
		}
	}
	else if (Data->buf[0] == '1')
	{
		if (Data->buf[1] == '\0')
			GetInst()->m_eFindUser = SECOND;
		else
			GetInst()->m_eFindUser = FIRST;
	}
	else if(Data->buf[0] == '2')
	{
		string strRoom = "";
		string strFrom = "";
		string strTo = "";
		string strText = "";
		string strIn = "";
		GetInst()->SliceText(Data->buf, strRoom, strFrom, strTo,strText);

		unordered_map<string, Room*>::iterator iter = GetInst()->m_mapRoom.find(strRoom);
		
		if (iter == GetInst()->m_mapRoom.end())
		{
			Room* pRoom = new Room;
			pRoom->Create(IDD_Room, GetInst()->m_pTabCtrl);
			pRoom->ShowWindow(SW_HIDE);
			GetInst()->m_mapRoom.insert(unordered_map<string, Room*>::value_type(strRoom, pRoom));
			iter = GetInst()->m_mapRoom.find(strRoom);
			pRoom->SetID(CString(strRoom.c_str()));
		}

		if (GetInst()->m_strMyName == strFrom.c_str())
		{
			strIn = "내가 보낸 메시지 : ";
		}
		else
			strIn = "보낸 사람 " + strFrom + " :";
		iter->second->m_vecText.push_back(strIn);
		iter->second->m_vecText.push_back(strText);
		
		iter->second->m_editText.SetWindowTextW(CString(strText.c_str()));

		list<Room*>::iterator iterRoom = GetInst()->m_listRoom.begin();
		for (; iterRoom != GetInst()->m_listRoom.end(); iterRoom++)
		{
			if (*iterRoom == iter->second)
			{
				GetInst()->m_listRoom.erase(iterRoom);
				break;
			}
		}

		GetInst()->m_listRoom.push_front(iter->second);
		GetInst()->SortRoomList();

		if (iter->second->m_pConverse)
		{
			int iCount = iter->second->m_pConverse->m_listChat.GetCount();
			iter->second->m_pConverse->m_listChat.InsertString(iCount, CString(strIn.c_str()));
			iter->second->m_pConverse->m_listChat.InsertString(iCount + 1, CString(strText.c_str()));
		}
	}
	else if (Data->buf[0] == '3')
	{
		string strRoom = "";
		string strTo = "";
		string strFrom = "";
		
		GetInst()->SliceText(Data->buf, strRoom,strTo,strFrom);
		Room* pRoom = new Room;
		pRoom->Create(IDD_Room, GetInst()->m_pTabCtrl);
		pRoom->ShowWindow(SW_HIDE);
		// 메시지를 받는 사람이 자신이라면
		if (GetInst()->m_strMyName == strTo.c_str())
			strTo = strFrom;
		int iIdx = GetInst()->m_listFriend->FindString(0, CString(strTo.c_str()));
		GetInst()->m_listFriend->SetItemData(iIdx, (DWORD_PTR)pRoom);


		list<ConverseDlg*>::iterator iter = GetInst()->m_listConverse.begin();
		for (; iter != GetInst()->m_listConverse.end(); ++iter)
		{
			if ((*iter)->m_strName == strTo.c_str())
			{
				(*iter)->pRoom = pRoom;
				pRoom->m_pConverse = (*iter);
				GetInst()->m_listConverse.erase(iter);
				break;
			}
		}
		pRoom->SetID(CString(strRoom.c_str()));
		pRoom->SetName(CString(strTo.c_str()));
		GetInst()->m_mapRoom.insert(unordered_map<string,Room*>::value_type(strRoom,pRoom));
	}
	else if (Data->buf[0] == '4')
	{
		Room* pRoom = nullptr;
		string strTo;
		string strRoomID;
		string strRoomName;
		GetInst()->SliceText(Data->buf, strTo, strRoomID, strRoomName);

		strRoomName += ", " + strTo;

		unordered_map<string, Room*>::iterator iter = GetInst()->m_mapRoom.find(strRoomID);
		if (iter == GetInst()->m_mapRoom.end())
		{
			pRoom = new Room;
			pRoom->Create(IDD_Room, GetInst()->m_pTabCtrl);
			pRoom->ShowWindow(SW_HIDE);
			pRoom->SetName(CString(strRoomName.c_str()));
			GetInst()->m_mapRoom.insert(unordered_map<string, Room*>::value_type(strRoomID, pRoom));
		}
		else
			iter->second->SetName(CString(strRoomName.c_str()));
	}
	else if (Data->buf[0] == '5')
	{
		string strTo;
		string strFrom;
		string strRoomID;
		string strRoomName;
		GetInst()->SliceText(Data->buf,strTo, strFrom, strRoomID,strRoomName);

		if (GetInst()->m_strMyName == strRoomName.c_str())
			strRoomName = strFrom;

		int iFind = GetInst()->m_listFriend->FindString(0, CString(strRoomName.c_str()));
		GetInst()->m_listFriend->SetItemData(iFind,NULL);

		strRoomName += ", " + strTo;

		unordered_map<string, Room*>::iterator iter = GetInst()->m_mapRoom.find(strRoomID);
		if (iter == GetInst()->m_mapRoom.end())
		{
			Room* pRoom = new Room;
			pRoom->Create(IDD_Room, GetInst()->m_pTabCtrl);
			pRoom->ShowWindow(SW_HIDE);
			pRoom->SetName(CString(strRoomName.c_str()));
			GetInst()->m_mapRoom.insert(unordered_map<string, Room*>::value_type(strRoomID, pRoom));
		}
		else
			iter->second->SetName(CString(strRoomName.c_str()));
	}
}

void ServerMgr::SetMainDlg(CChattingProgramDlg* _pDlg)
{
	m_pMainDlg = _pDlg;
}

bool ServerMgr::InsertChatDlg(CString& _strKey)
{
	string strKey = CStringA(_strKey);
	unordered_map<string, Room*>::iterator iter = m_mapRoom.find(strKey);
	if (iter == m_mapRoom.end())
	{
		
		//m_mapRoom.insert(unordered_map<string, ChatText>::value_type(strKey, ChatText(pChat)));
	}
	else
	{
		//iter->second.pConverseDlg->ShowWindow(SW_SHOW);
		return false;
	}
	return true;
}

void ServerMgr::SliceText(char* _pOri, string* _arrText, int _iCount)
{
	int iNext = 1;
	size_t i = 0;
	for (; i < _iCount - 1; i++)
	{
		while (true)
		{
			if (_pOri[iNext] == '/')
			{
				++iNext;
				break;
			}
			_arrText[i] += _pOri[iNext];
			++iNext;
		}
	}
    _arrText[i + 1] = &_pOri[iNext];
}

void ServerMgr::SliceText(char* _pOri, string& _First, string& _Second, string& _Third, string& _Four)
{
	int iNext = 1;
	string strText = "";
	while (true)
	{
		if (_pOri[iNext] == '/')
		{
			++iNext;
			if (_First.empty())
				_First = strText;
			else if (_Second.empty())
				_Second = strText;
			else
			{
				_Third = strText;
				break;
			}
			strText = "";
		}
		strText += _pOri[iNext];
		++iNext;
	}
	_Four = &_pOri[iNext];
}

void ServerMgr::SliceText(char* _pOri, string& _First, string& _Second, string& _Third)
{
	int iNext = 1;
	string strText = "";
	while (true)
	{
		if (_pOri[iNext] == '/')
		{
			++iNext;
			if (_First.empty())
				_First = strText;
			else
			{
				_Second = strText;
				break;
			}
			strText = "";
		}
		strText += _pOri[iNext];
		++iNext;
	}
	_Third = &_pOri[iNext];
}

void ServerMgr::SliceText(char* _pOri, string& _First, string& _Second)
{
	int iNext = 1;
	while (true)
	{
		if (_pOri[iNext] == '/')
			break;
		_First += _pOri[iNext];
		++iNext;
	}
	_Second = &_pOri[iNext + 1];
}

void ServerMgr::SortRoomList()
{
	int iNext = 0;
	CRect tabRect;
	int iInterval = 60;
	m_pTabCtrl->GetClientRect(&tabRect);
	CRect rt(0,20, tabRect.Width() - 2, 80);
	list<Room*>::iterator iterRoom = GetInst()->m_listRoom.begin();
	for (; iterRoom != GetInst()->m_listRoom.end(); iterRoom++)
	{
		rt.top = 20 + (iNext * iInterval);
		rt.bottom = 80 + (iNext * iInterval);
		(*iterRoom)->MoveWindow(&rt);
		++iNext;
		if (m_pTabCtrl->GetCurSel() == 2)
			(*iterRoom)->ShowWindow(SW_SHOW);
	}
}