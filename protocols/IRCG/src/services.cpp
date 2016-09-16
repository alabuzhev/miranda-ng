/*
IRC plugin for Miranda IM

Copyright (C) 2003-05 Jurgen Persson
Copyright (C) 2007-09 George Hazan

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "stdafx.h"

void CIrcProto::InitMainMenus(void)
{
	CMenuItem mi;
	mi.root = Menu_GetProtocolRoot(this);

	mi.name.a = LPGEN("&Quick connect");
	mi.hIcolibItem = GetIconHandle(IDI_QUICK);
	mi.pszService = IRC_QUICKCONNECT;
	mi.position = 201001;
	hMenuQuick = Menu_AddProtoMenuItem(&mi, m_szModuleName);

	if (m_iStatus != ID_STATUS_OFFLINE) mi.flags |= CMIF_GRAYED;

	mi.name.a = LPGEN("&Join channel");
	mi.hIcolibItem = Skin_GetIconHandle(SKINICON_CHAT_JOIN);
	mi.pszService = IRC_JOINCHANNEL;
	mi.position = 201002;
	hMenuJoin = Menu_AddProtoMenuItem(&mi, m_szModuleName);

	mi.name.a = LPGEN("&Change your nickname");
	mi.hIcolibItem = GetIconHandle(IDI_RENAME);
	mi.pszService = IRC_CHANGENICK;
	mi.position = 201003;
	hMenuNick = Menu_AddProtoMenuItem(&mi, m_szModuleName);

	mi.name.a = LPGEN("Show the &list of available channels");
	mi.hIcolibItem = GetIconHandle(IDI_LIST);
	mi.pszService = IRC_SHOWLIST;
	mi.position = 201004;
	hMenuList = Menu_AddProtoMenuItem(&mi, m_szModuleName);

	if (m_useServer) mi.flags &= ~CMIF_GRAYED;
	mi.name.a = LPGEN("&Show the server window");
	mi.hIcolibItem = GetIconHandle(IDI_SERVER);
	mi.pszService = IRC_SHOWSERVER;
	mi.position = 201005;
	hMenuServer = Menu_AddProtoMenuItem(&mi, m_szModuleName);
}

/////////////////////////////////////////////////////////////////////////////////////////

static HGENMENU hUMenuChanSettings, hUMenuWhois, hUMenuDisconnect, hUMenuIgnore;
static HANDLE hMenuChanSettings, hMenuWhois, hMenuDisconnect, hMenuIgnore;

static CIrcProto* IrcGetInstanceByHContact(MCONTACT hContact)
{
	char* szProto = GetContactProto(hContact);
	if (szProto == NULL)
		return NULL;

	for (int i = 0; i < g_Instances.getCount(); i++)
		if (!mir_strcmp(szProto, g_Instances[i]->m_szModuleName))
			return g_Instances[i];

	return NULL;
}

static INT_PTR IrcMenuChanSettings(WPARAM wParam, LPARAM lParam)
{
	CIrcProto *ppro = IrcGetInstanceByHContact(wParam);
	return (ppro) ? ppro->OnMenuChanSettings(wParam, lParam) : 0;
}

static INT_PTR IrcMenuWhois(WPARAM wParam, LPARAM lParam)
{
	CIrcProto *ppro = IrcGetInstanceByHContact(wParam);
	return (ppro) ? ppro->OnMenuWhois(wParam, lParam) : 0;
}

static INT_PTR IrcMenuDisconnect(WPARAM wParam, LPARAM lParam)
{
	CIrcProto *ppro = IrcGetInstanceByHContact(wParam);
	return (ppro) ? ppro->OnMenuDisconnect(wParam, lParam) : 0;
}

static INT_PTR IrcMenuIgnore(WPARAM wParam, LPARAM lParam)
{
	CIrcProto *ppro = IrcGetInstanceByHContact(wParam);
	return (ppro) ? ppro->OnMenuIgnore(wParam, lParam) : 0;
}

int IrcPrebuildContactMenu(WPARAM wParam, LPARAM lParam)
{
	Menu_ShowItem(hUMenuChanSettings, false);
	Menu_ShowItem(hUMenuWhois, false);
	Menu_ShowItem(hUMenuDisconnect, false);
	Menu_ShowItem(hUMenuIgnore, false);

	CIrcProto *ppro = IrcGetInstanceByHContact(wParam);
	return (ppro) ? ppro->OnMenuPreBuild(wParam, lParam) : 0;
}

void InitContactMenus(void)
{
	char temp[MAXMODULELABELLENGTH];
	char *d = temp + mir_snprintf(temp, "IRC");

	CMenuItem mi;
	mi.pszService = temp;

	SET_UID(mi, 0x5f01196f, 0xfbcd, 0x4034, 0xbd, 0x90, 0x12, 0xa0, 0x20, 0x68, 0x15, 0xc0);
	mi.name.a = LPGEN("Channel &settings");
	mi.hIcolibItem = GetIconHandle(IDI_MANAGER);
	mir_strcpy(d, IRC_UM_CHANSETTINGS);
	mi.position = 500090002;
	hUMenuChanSettings = Menu_AddContactMenuItem(&mi);
	hMenuChanSettings = CreateServiceFunction(temp, IrcMenuChanSettings);

	SET_UID(mi, 0x778eb1f6, 0x73c4, 0x4951, 0xb2, 0xca, 0xa1, 0x69, 0x94, 0x7b, 0xb7, 0x87);
	mi.name.a = LPGEN("&WhoIs info");
	mi.hIcolibItem = GetIconHandle(IDI_WHOIS);
	mir_strcpy(d, IRC_UM_WHOIS);
	mi.position = 500090001;
	hUMenuWhois = Menu_AddContactMenuItem(&mi);
	hMenuWhois = CreateServiceFunction(temp, IrcMenuWhois);

	SET_UID(mi, 0x1c51ae05, 0x9eee, 0x4887, 0x88, 0x96, 0x55, 0xd2, 0xdd, 0xf9, 0x25, 0x6f);
	mi.name.a = LPGEN("Di&sconnect");
	mi.hIcolibItem = GetIconHandle(IDI_DELETE);
	mir_strcpy(d, IRC_UM_DISCONNECT);
	mi.position = 500090001;
	hUMenuDisconnect = Menu_AddContactMenuItem(&mi);
	hMenuDisconnect = CreateServiceFunction(temp, IrcMenuDisconnect);

	SET_UID(mi, 0xc6169b8f, 0x53ab, 0x4242, 0xbe, 0x90, 0xe2, 0x4a, 0xa5, 0x73, 0x88, 0x32);
	mi.name.a = LPGEN("&Add to ignore list");
	mi.hIcolibItem = GetIconHandle(IDI_BLOCK);
	mir_strcpy(d, IRC_UM_IGNORE);
	mi.position = 500090002;
	hUMenuIgnore = Menu_AddContactMenuItem(&mi);
	hMenuIgnore = CreateServiceFunction(temp, IrcMenuIgnore);

	HookEvent(ME_CLIST_PREBUILDCONTACTMENU, IrcPrebuildContactMenu);
}

void UninitContactMenus(void)
{
	Menu_RemoveItem(hUMenuChanSettings);
	Menu_RemoveItem(hUMenuWhois);
	Menu_RemoveItem(hUMenuDisconnect);
	Menu_RemoveItem(hUMenuIgnore);
	
	DestroyServiceFunction(hMenuChanSettings);
	DestroyServiceFunction(hMenuWhois);
	DestroyServiceFunction(hMenuDisconnect);
	DestroyServiceFunction(hMenuIgnore);
}

INT_PTR __cdecl CIrcProto::OnDoubleclicked(WPARAM, LPARAM lParam)
{
	if (!lParam)
		return 0;

	CLISTEVENT* pcle = (CLISTEVENT*)lParam;

	if (getByte(pcle->hContact, "DCC", 0) != 0) {
		DCCINFO* pdci = (DCCINFO*)pcle->lParam;
		CMessageBoxDlg* dlg = new CMessageBoxDlg(this, pdci);
		dlg->Show();
		HWND hWnd = dlg->GetHwnd();
		wchar_t szTemp[500];
		mir_snwprintf(szTemp, TranslateT("%s (%s) is requesting a client-to-client chat connection."),
			pdci->sContactName.c_str(), pdci->sHostmask.c_str());
		SetDlgItemText(hWnd, IDC_TEXT, szTemp);
		ShowWindow(hWnd, SW_SHOW);
		return 1;
	}
	return 0;
}

int __cdecl CIrcProto::OnContactDeleted(WPARAM wp, LPARAM)
{
	MCONTACT hContact = (MCONTACT)wp;
	if (!hContact)
		return 0;

	DBVARIANT dbv;
	if (!getWString(hContact, "Nick", &dbv)) {
		int type = getByte(hContact, "ChatRoom", 0);
		if (type != 0) {
			CMStringW S = L"";
			if (type == GCW_CHATROOM)
				S = MakeWndID(dbv.ptszVal);
			if (type == GCW_SERVER)
				S = SERVERWINDOW;
			GCDEST gcd = { m_szModuleName, S.c_str(), GC_EVENT_CONTROL };
			GCEVENT gce = { sizeof(gce), &gcd };
			int i = CallChatEvent(SESSION_TERMINATE, &gce);
			if (i && type == GCW_CHATROOM)
				PostIrcMessage(L"/PART %s %s", dbv.ptszVal, m_userInfo);
		}
		else {
			BYTE bDCC = getByte((MCONTACT)wp, "DCC", 0);
			if (bDCC) {
				CDccSession* dcc = FindDCCSession((MCONTACT)wp);
				if (dcc)
					dcc->Disconnect();
			}
		}

		db_free(&dbv);
	}
	return 0;
}

INT_PTR __cdecl CIrcProto::OnJoinChat(WPARAM wp, LPARAM)
{
	if (!wp)
		return 0;

	DBVARIANT dbv;
	if (!getWString((MCONTACT)wp, "Nick", &dbv)) {
		if (getByte((MCONTACT)wp, "ChatRoom", 0) == GCW_CHATROOM)
			PostIrcMessage(L"/JOIN %s", dbv.ptszVal);
		db_free(&dbv);
	}
	return 0;
}

INT_PTR __cdecl CIrcProto::OnLeaveChat(WPARAM wp, LPARAM)
{
	if (!wp)
		return 0;

	DBVARIANT dbv;
	if (!getWString((MCONTACT)wp, "Nick", &dbv)) {
		if (getByte((MCONTACT)wp, "ChatRoom", 0) == GCW_CHATROOM) {
			PostIrcMessage(L"/PART %s %s", dbv.ptszVal, m_userInfo);

			CMStringW S = MakeWndID(dbv.ptszVal);
			GCDEST gcd = { m_szModuleName, S.c_str(), GC_EVENT_CONTROL };
			GCEVENT gce = { sizeof(gce), &gcd };
			CallChatEvent(SESSION_TERMINATE, &gce);
		}
		db_free(&dbv);
	}
	return 0;
}

INT_PTR __cdecl CIrcProto::OnMenuChanSettings(WPARAM wp, LPARAM)
{
	if (!wp)
		return 0;

	MCONTACT hContact = (MCONTACT)wp;
	DBVARIANT dbv;
	if (!getWString(hContact, "Nick", &dbv)) {
		PostIrcMessageWnd(dbv.ptszVal, NULL, L"/CHANNELMANAGER");
		db_free(&dbv);
	}
	return 0;
}

INT_PTR __cdecl CIrcProto::OnMenuWhois(WPARAM wp, LPARAM)
{
	if (!wp)
		return 0;

	DBVARIANT dbv;

	if (!getWString((MCONTACT)wp, "Nick", &dbv)) {
		PostIrcMessage(L"/WHOIS %s %s", dbv.ptszVal, dbv.ptszVal);
		db_free(&dbv);
	}
	return 0;
}

INT_PTR __cdecl CIrcProto::OnMenuDisconnect(WPARAM wp, LPARAM)
{
	CDccSession* dcc = FindDCCSession((MCONTACT)wp);
	if (dcc)
		dcc->Disconnect();
	return 0;
}

INT_PTR __cdecl CIrcProto::OnMenuIgnore(WPARAM wp, LPARAM)
{
	if (!wp)
		return 0;

	MCONTACT hContact = (MCONTACT)wp;
	DBVARIANT dbv;
	if (!getWString(hContact, "Nick", &dbv)) {
		if (!isChatRoom(hContact)) {
			char* host = NULL;
			DBVARIANT dbv1;
			if (!getString(hContact, "Host", &dbv1))
				host = dbv1.pszVal;

			if (host) {
				CMStringA S;
				if (m_ignoreChannelDefault)
					S = "+qnidcm";
				else
					S = "+qnidc";
				PostIrcMessage(L"/IGNORE %%question=\"%s\",\"%s\",\"*!*@%S\" %s",
					TranslateT("Please enter the hostmask (nick!user@host)\nNOTE! Contacts on your contact list are never ignored"),
					TranslateT("Ignore"), host, S.c_str());
				db_free(&dbv1);
			}
		}
		db_free(&dbv);
	}
	return 0;
}

INT_PTR __cdecl CIrcProto::OnJoinMenuCommand(WPARAM, LPARAM)
{
	if (!m_joinDlg) {
		m_joinDlg = new CJoinDlg(this);
		m_joinDlg->Show();
	}

	SetDlgItemText(m_joinDlg->GetHwnd(), IDC_CAPTION, TranslateT("Join channel"));
	SetDlgItemText(m_joinDlg->GetHwnd(), IDC_TEXT, TranslateT("Please enter a channel to join"));
	SendDlgItemMessage(m_joinDlg->GetHwnd(), IDC_ENICK, EM_SETSEL, 0, MAKELPARAM(0, -1));
	ShowWindow(m_joinDlg->GetHwnd(), SW_SHOW);
	SetActiveWindow(m_joinDlg->GetHwnd());
	return 0;
}

INT_PTR __cdecl CIrcProto::OnQuickConnectMenuCommand(WPARAM, LPARAM)
{
	if (!m_quickDlg) {
		m_quickDlg = new CQuickDlg(this);
		m_quickDlg->Show();

		SetWindowText(m_quickDlg->GetHwnd(), TranslateT("Quick connect"));
		SetDlgItemText(m_quickDlg->GetHwnd(), IDC_TEXT, TranslateT("Please select IRC network and enter the password if needed"));
		SetDlgItemText(m_quickDlg->GetHwnd(), IDC_CAPTION, TranslateT("Quick connect"));
		Window_SetIcon_IcoLib(m_quickDlg->GetHwnd(), GetIconHandle(IDI_QUICK));
	}

	ShowWindow(m_quickDlg->GetHwnd(), SW_SHOW);
	SetActiveWindow(m_quickDlg->GetHwnd());
	return 0;
}

INT_PTR __cdecl CIrcProto::OnShowListMenuCommand(WPARAM, LPARAM)
{
	PostIrcMessage(L"/LIST");
	return 0;
}

INT_PTR __cdecl CIrcProto::OnShowServerMenuCommand(WPARAM, LPARAM)
{
	GCDEST gcd = { m_szModuleName, SERVERWINDOW, GC_EVENT_CONTROL };
	GCEVENT gce = { sizeof(gce), &gcd };
	CallChatEvent(WINDOW_VISIBLE, &gce);
	return 0;
}

INT_PTR __cdecl CIrcProto::OnChangeNickMenuCommand(WPARAM, LPARAM)
{
	if (!m_nickDlg) {
		m_nickDlg = new CNickDlg(this);
		m_nickDlg->Show();
	}

	SetDlgItemText(m_nickDlg->GetHwnd(), IDC_CAPTION, TranslateT("Change nickname"));
	SetDlgItemText(m_nickDlg->GetHwnd(), IDC_TEXT, TranslateT("Please enter a unique nickname"));
	m_nickDlg->m_Enick.SetText(m_info.sNick.c_str());
	m_nickDlg->m_Enick.SendMsg(CB_SETEDITSEL, 0, MAKELPARAM(0, -1));
	ShowWindow(m_nickDlg->GetHwnd(), SW_SHOW);
	SetActiveWindow(m_nickDlg->GetHwnd());
	return 0;
}

static void DoChatFormatting(wchar_t* pszText)
{
	wchar_t* p1 = pszText;
	int iFG = -1;
	int iRemoveChars;
	wchar_t InsertThis[50];

	while (*p1 != '\0') {
		iRemoveChars = 0;
		InsertThis[0] = 0;

		if (*p1 == '%') {
			switch (p1[1]) {
			case 'B':
			case 'b':
				mir_wstrcpy(InsertThis, L"\002");
				iRemoveChars = 2;
				break;
			case 'I':
			case 'i':
				mir_wstrcpy(InsertThis, L"\026");
				iRemoveChars = 2;
				break;
			case 'U':
			case 'u':
				mir_wstrcpy(InsertThis, L"\037");
				iRemoveChars = 2;
				break;
			case 'c':
			{
				mir_wstrcpy(InsertThis, L"\003");
				iRemoveChars = 2;

				wchar_t szTemp[3];
				mir_wstrncpy(szTemp, p1 + 2, 3);
				iFG = _wtoi(szTemp);
			}
				break;
			case 'C':
				if (p1[2] == '%' && p1[3] == 'F') {
					mir_wstrcpy(InsertThis, L"\x0399,99");
					iRemoveChars = 4;
				}
				else {
					mir_wstrcpy(InsertThis, L"\x0399");
					iRemoveChars = 2;
				}
				iFG = -1;
				break;
			case 'f':
				if (p1 - 3 >= pszText && p1[-3] == '\003')
					mir_wstrcpy(InsertThis, L",");
				else if (iFG >= 0)
					mir_snwprintf(InsertThis, L"\x03%u,", iFG);
				else
					mir_wstrcpy(InsertThis, L"\x0399,");

				iRemoveChars = 2;
				break;

			case 'F':
				if (iFG >= 0)
					mir_snwprintf(InsertThis, L"\x03%u,99", iFG);
				else
					mir_wstrcpy(InsertThis, L"\x0399,99");
				iRemoveChars = 2;
				break;

			case '%':
				mir_wstrcpy(InsertThis, L"%");
				iRemoveChars = 2;
				break;

			default:
				iRemoveChars = 2;
				break;
			}

			memmove(p1 + mir_wstrlen(InsertThis), p1 + iRemoveChars, sizeof(wchar_t)*(mir_wstrlen(p1) - iRemoveChars + 1));
			memcpy(p1, InsertThis, sizeof(wchar_t)*mir_wstrlen(InsertThis));
			if (iRemoveChars || mir_wstrlen(InsertThis))
				p1 += mir_wstrlen(InsertThis);
			else
				p1++;
		}
		else p1++;
	}
}

int __cdecl CIrcProto::GCEventHook(WPARAM, LPARAM lParam)
{
	GCHOOK *gch = (GCHOOK*)lParam;
	CMStringW S = L"";

	mir_cslock lock(m_gchook);

	// handle the hook
	if (gch) {
		if (!mir_strcmpi(gch->pDest->pszModule, m_szModuleName)) {
			wchar_t *p1 = mir_wstrdup(gch->pDest->ptszID);
			wchar_t *p2 = wcsstr(p1, L" - ");
			if (p2)
				*p2 = '\0';

			switch (gch->pDest->iType) {
			case GC_SESSION_TERMINATE:
				FreeWindowItemData(p1, (CHANNELINFO*)gch->dwData);
				break;

			case GC_USER_MESSAGE:
				if (gch && gch->ptszText && *gch->ptszText) {
					wchar_t* pszText = new wchar_t[mir_wstrlen(gch->ptszText) + 1000];
					mir_wstrcpy(pszText, gch->ptszText);
					DoChatFormatting(pszText);
					PostIrcMessageWnd(p1, NULL, pszText);
					delete[]pszText;
				}
				break;

			case GC_USER_CHANMGR:
				PostIrcMessageWnd(p1, NULL, L"/CHANNELMANAGER");
				break;

			case GC_USER_PRIVMESS:
			{
				wchar_t szTemp[4000];
				mir_snwprintf(szTemp, L"/QUERY %s", gch->ptszUID);
				PostIrcMessageWnd(p1, NULL, szTemp);
			}
				break;

			case GC_USER_LOGMENU:
				switch (gch->dwData) {
				case 1:
					OnChangeNickMenuCommand(NULL, NULL);
					break;
				case 2:
					PostIrcMessageWnd(p1, NULL, L"/CHANNELMANAGER");
					break;

				case 3:
					PostIrcMessage(L"/PART %s %s", p1, m_userInfo);
					{
						S = MakeWndID(p1);
						GCDEST gcd = { m_szModuleName, S.c_str(), GC_EVENT_CONTROL };
						GCEVENT gce = { sizeof(gce), &gcd };
						CallChatEvent(SESSION_TERMINATE, &gce);
					}
					break;
				case 4:		// show server window
					PostIrcMessageWnd(p1, NULL, L"/SERVERSHOW");
					break;
					/*					case 5:		// nickserv register nick
					PostIrcMessage( L"/nickserv REGISTER %%question=\"%s\",\"%s\"",
					TranslateT("Please enter your authentication code"), TranslateT("Authenticate nick"));
					break;
					*/
				case 6:		// nickserv Identify
					PostIrcMessage(L"/nickserv AUTH %%question=\"%s\",\"%s\"",
						TranslateT("Please enter your authentication code"), TranslateT("Authenticate nick"));
					break;
				case 7:		// nickserv drop nick
					if (MessageBox(0, TranslateT("Are you sure you want to unregister your current nick?"), TranslateT("Delete nick"),
						MB_ICONERROR + MB_YESNO + MB_DEFBUTTON2) == IDYES)
						PostIrcMessage(L"/nickserv DROP");
					break;
				case 8:		// nickserv Identify
				{
					CQuestionDlg* dlg = new CQuestionDlg(this);
					dlg->Show();
					HWND question_hWnd = dlg->GetHwnd();
					HWND hEditCtrl = GetDlgItem(question_hWnd, IDC_EDIT);
					SetDlgItemText(question_hWnd, IDC_CAPTION, TranslateT("Identify nick"));
					SetDlgItemText(question_hWnd, IDC_TEXT, TranslateT("Please enter your password"));
					SetDlgItemText(question_hWnd, IDC_HIDDENEDIT, L"/nickserv IDENTIFY %question=\"%s\",\"%s\"");
					SetWindowLongPtr(GetDlgItem(question_hWnd, IDC_EDIT), GWL_STYLE,
						(LONG)GetWindowLongPtr(GetDlgItem(question_hWnd, IDC_EDIT), GWL_STYLE) | ES_PASSWORD);
					SendMessage(hEditCtrl, EM_SETPASSWORDCHAR, (WPARAM)'*', 0);
					SetFocus(hEditCtrl);
					dlg->Activate();
				}
					break;
				case 9:		// nickserv remind password
				{
					DBVARIANT dbv;
					if (!getWString("Nick", &dbv)) {
						PostIrcMessage(L"/nickserv SENDPASS %s", dbv.ptszVal);
						db_free(&dbv);
					}
				}
					break;
				case 10:		// nickserv set new password
					PostIrcMessage(L"/nickserv SET PASSWORD %%question=\"%s\",\"%s\"",
						TranslateT("Please enter your new password"), TranslateT("Set new password"));
					break;
				case 11:		// nickserv set language
					PostIrcMessage(L"/nickserv SET LANGUAGE %%question=\"%s\",\"%s\"",
						TranslateT("Please enter desired language ID (numeric value, depends on server)"), TranslateT("Change language of NickServ messages"));
					break;
				case 12:		// nickserv set homepage
					PostIrcMessage(L"/nickserv SET URL %%question=\"%s\",\"%s\"",
						TranslateT("Please enter URL that will be linked to your nick"), TranslateT("Set URL, linked to nick"));
					break;
				case 13:		// nickserv set email
					PostIrcMessage(L"/nickserv SET EMAIL %%question=\"%s\",\"%s\"",
						TranslateT("Please enter your e-mail, that will be linked to your nick"), TranslateT("Set e-mail, linked to nick"));
					break;
				case 14:		// nickserv set info
					PostIrcMessage(L"/nickserv SET INFO %%question=\"%s\",\"%s\"",
						TranslateT("Please enter some information about your nick"), TranslateT("Set information for nick"));
					break;
				case 15:		// nickserv kill unauth off
					PostIrcMessage(L"/nickserv SET KILL OFF");
					break;
				case 16:		// nickserv kill unauth on
					PostIrcMessage(L"/nickserv SET KILL ON");
					break;
				case 17:		// nickserv kill unauth quick
					PostIrcMessage(L"/nickserv SET KILL QUICK");
					break;
				case 18:		// nickserv hide nick from /LIST
					PostIrcMessage(L"/nickserv SET PRIVATE ON");
					break;
				case 19:		// nickserv show nick to /LIST
					PostIrcMessage(L"/nickserv SET PRIVATE OFF");
					break;
				case 20:		// nickserv Hide e-mail from info
					PostIrcMessage(L"/nickserv SET HIDE EMAIL ON");
					break;
				case 21:		// nickserv Show e-mail in info
					PostIrcMessage(L"/nickserv SET HIDE EMAIL OFF");
					break;
				case 22:		// nickserv Set security for nick
					PostIrcMessage(L"/nickserv SET SECURE ON");
					break;
				case 23:		// nickserv Remove security for nick
					PostIrcMessage(L"/nickserv SET SECURE OFF");
					break;
				case 24:		// nickserv Link nick to current
					PostIrcMessage(L"/nickserv LINK %%question=\"%s\",\"%s\"",
						TranslateT("Please enter nick you want to link to your current nick"), TranslateT("Link another nick to current nick"));
					break;
				case 25:		// nickserv Unlink nick from current
					PostIrcMessage(L"/nickserv LINK %%question=\"%s\",\"%s\"",
						TranslateT("Please enter nick you want to unlink from your current nick"), TranslateT("Unlink another nick from current nick"));
					break;
				case 26:		// nickserv Set main nick
					PostIrcMessage(L"/nickserv LINK %%question=\"%s\",\"%s\"",
						TranslateT("Please enter nick you want to set as your main nick"), TranslateT("Set main nick"));
					break;
				case 27:		// nickserv list all linked nicks
					PostIrcMessage(L"/nickserv LISTLINKS");
					break;
				case 28:		// nickserv list all channels owned
					PostIrcMessage(L"/nickserv LISTCHANS");
					break;
				}
				break;

			case GC_USER_NICKLISTMENU:
				switch (gch->dwData) {
				case 1:
					PostIrcMessage(L"/MODE %s +o %s", p1, gch->ptszUID);
					break;
				case 2:
					PostIrcMessage(L"/MODE %s -o %s", p1, gch->ptszUID);
					break;
				case 3:
					PostIrcMessage(L"/MODE %s +v %s", p1, gch->ptszUID);
					break;
				case 4:
					PostIrcMessage(L"/MODE %s -v %s", p1, gch->ptszUID);
					break;
				case 5:
					PostIrcMessage(L"/KICK %s %s", p1, gch->ptszUID);
					break;
				case 6:
					PostIrcMessage(L"/KICK %s %s %%question=\"%s\",\"%s\",\"%s\"",
						p1, gch->ptszUID, TranslateT("Please enter the reason"), TranslateT("Kick"), TranslateT("Jerk"));
					break;
				case 7:
					DoUserhostWithReason(1, L"B" + (CMStringW)p1, true, L"%s", gch->ptszUID);
					break;
				case 8:
					DoUserhostWithReason(1, L"K" + (CMStringW)p1, true, L"%s", gch->ptszUID);
					break;
				case 9:
					DoUserhostWithReason(1, L"L" + (CMStringW)p1, true, L"%s", gch->ptszUID);
					break;
				case 10:
					PostIrcMessage(L"/WHOIS %s %s", gch->ptszUID, gch->ptszUID);
					break;
					//	case 11:
					//		DoUserhostWithReason(1, "I", true, "%s", gch->ptszUID );
					//		break;
					//	case 12:
					//		DoUserhostWithReason(1, "J", true, "%s", gch->ptszUID );
					//		break;
				case 13:
					PostIrcMessage(L"/DCC CHAT %s", gch->ptszUID);
					break;
				case 14:
					PostIrcMessage(L"/DCC SEND %s", gch->ptszUID);
					break;
				case 15:
					DoUserhostWithReason(1, L"I", true, L"%s", gch->ptszUID);
					break;
				case 16:
					PostIrcMessage(L"/MODE %s +h %s", p1, gch->ptszUID);
					break;
				case 17:
					PostIrcMessage(L"/MODE %s -h %s", p1, gch->ptszUID);
					break;
				case 18:
					PostIrcMessage(L"/MODE %s +q %s", p1, gch->ptszUID);
					break;
				case 19:
					PostIrcMessage(L"/MODE %s -q %s", p1, gch->ptszUID);
					break;
				case 20:
					PostIrcMessage(L"/MODE %s +a %s", p1, gch->ptszUID);
					break;
				case 21:
					PostIrcMessage(L"/MODE %s -a %s", p1, gch->ptszUID);
					break;
				case 22:
					PostIrcMessage(L"/NOTICE %s %%question=\"%s\",\"%s\"",
						gch->ptszUID, TranslateT("Please enter the notice text"), TranslateT("Send notice"));
					break;
				case 23:
					PostIrcMessage(L"/INVITE %s %%question=\"%s\",\"%s\"",
						gch->ptszUID, TranslateT("Please enter the channel name to invite to"), TranslateT("Invite to channel"));
					break;
				case 30:
					{
						PROTOSEARCHRESULT psr = { 0 };
						psr.cbSize = sizeof(psr);
						psr.flags = PSR_UNICODE;
						psr.id.w = gch->ptszUID;
						psr.nick.w = gch->ptszUID;

						ADDCONTACTSTRUCT acs = { 0 };
						acs.handleType = HANDLE_SEARCHRESULT;
						acs.szProto = m_szModuleName;
						acs.psr = &psr;
						CallService(MS_ADDCONTACT_SHOW, 0, (LPARAM)&acs);
					}
					break;
				case 31:	//slap
					PostIrcMessageWnd(p1, NULL, CMStringW(FORMAT, L"/slap %s", gch->ptszUID));
					break;
				case 32:  //nickserv info
					PostIrcMessageWnd(p1, NULL, CMStringW(FORMAT, L"/nickserv INFO %s ALL", gch->ptszUID));
					break;
				case 33:  //nickserv ghost
					PostIrcMessageWnd(p1, NULL, CMStringW(FORMAT, L"/nickserv GHOST %s", gch->ptszUID));
					break;
				}
				break;
			}
			mir_free(p1);
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

static gc_item logItems[] = {
		{ LPGENW("&Change your nickname"), 1, MENU_ITEM, FALSE },
		{ LPGENW("Channel &settings"), 2, MENU_ITEM, FALSE },
		{ L"", 0, MENU_SEPARATOR, FALSE },
		{ LPGENW("NickServ"), 0, MENU_NEWPOPUP, FALSE },
		{ LPGENW("Register nick"), 5, MENU_POPUPITEM, TRUE },
		{ LPGENW("Auth nick"), 6, MENU_POPUPITEM, FALSE },
		{ LPGENW("Delete nick"), 7, MENU_POPUPITEM, FALSE },
		{ LPGENW("Identify nick"), 8, MENU_POPUPITEM, FALSE },
		{ LPGENW("Remind password"), 9, MENU_POPUPITEM, FALSE },
		{ LPGENW("Set new password"), 10, MENU_POPUPITEM, TRUE },
		{ LPGENW("Set language"), 11, MENU_POPUPITEM, FALSE },
		{ LPGENW("Set homepage"), 12, MENU_POPUPITEM, FALSE },
		{ LPGENW("Set e-mail"), 13, MENU_POPUPITEM, FALSE },
		{ LPGENW("Set info"), 14, MENU_POPUPITEM, FALSE },
		{ L"", 0, MENU_POPUPSEPARATOR, FALSE },
		{ LPGENW("Hide e-mail from info"), 20, MENU_POPUPITEM, FALSE },
		{ LPGENW("Show e-mail in info"), 21, MENU_POPUPITEM, FALSE },
		{ L"", 0, MENU_POPUPSEPARATOR, FALSE },
		{ LPGENW("Set security for nick"), 22, MENU_POPUPITEM, FALSE },
		{ LPGENW("Remove security for nick"), 23, MENU_POPUPITEM, FALSE },
		{ L"", 0, MENU_POPUPSEPARATOR, FALSE },
		{ LPGENW("Link nick to current"), 24, MENU_POPUPITEM, FALSE },
		{ LPGENW("Unlink nick from current"), 25, MENU_POPUPITEM, FALSE },
		{ LPGENW("Set main nick"), 26, MENU_POPUPITEM, FALSE },
		{ LPGENW("List all your nicks"), 27, MENU_POPUPITEM, FALSE },
		{ LPGENW("List your channels"), 28, MENU_POPUPITEM, FALSE },
		{ L"", 0, MENU_POPUPSEPARATOR, FALSE },
		{ LPGENW("Kill unauthorized: off"), 15, MENU_POPUPITEM, FALSE },
		{ LPGENW("Kill unauthorized: on"), 16, MENU_POPUPITEM, FALSE },
		{ LPGENW("Kill unauthorized: quick"), 17, MENU_POPUPITEM, FALSE },
		{ L"", 0, MENU_POPUPSEPARATOR, FALSE },
		{ LPGENW("Hide nick from list"), 18, MENU_POPUPITEM, FALSE },
		{ LPGENW("Show nick to list"), 19, MENU_POPUPITEM, FALSE },
		{ LPGENW("Show the server &window"), 4, MENU_ITEM, FALSE },
		{ L"", 0, MENU_SEPARATOR, FALSE },
		{ LPGENW("&Leave the channel"), 3, MENU_ITEM, FALSE }
};

static gc_item nickItems[] = {
		{ LPGENW("&WhoIs info"), 10, MENU_ITEM, FALSE },       //0
		{ LPGENW("&Invite to channel"), 23, MENU_ITEM, FALSE },
		{ LPGENW("Send &notice"), 22, MENU_ITEM, FALSE },
		{ LPGENW("&Slap"), 31, MENU_ITEM, FALSE },
		{ LPGENW("Nickserv info"), 32, MENU_ITEM, FALSE },
		{ LPGENW("Nickserv kill ghost"), 33, MENU_ITEM, FALSE },      //5
		{ LPGENW("&Control"), 0, MENU_NEWPOPUP, FALSE },
		{ LPGENW("Give Owner"), 18, MENU_POPUPITEM, FALSE },      //7
		{ LPGENW("Take Owner"), 19, MENU_POPUPITEM, FALSE },
		{ LPGENW("Give Admin"), 20, MENU_POPUPITEM, FALSE },
		{ LPGENW("Take Admin"), 21, MENU_POPUPITEM, FALSE },      //10
		{ LPGENW("Give &Op"), 1, MENU_POPUPITEM, FALSE },
		{ LPGENW("Take O&p"), 2, MENU_POPUPITEM, FALSE },
		{ LPGENW("Give &Halfop"), 16, MENU_POPUPITEM, FALSE },
		{ LPGENW("Take H&alfop"), 17, MENU_POPUPITEM, FALSE },
		{ LPGENW("Give &Voice"), 3, MENU_POPUPITEM, FALSE },      //15
		{ LPGENW("Take V&oice"), 4, MENU_POPUPITEM, FALSE },
		{ L"", 0, MENU_POPUPSEPARATOR, FALSE },
		{ LPGENW("&Kick"), 5, MENU_POPUPITEM, FALSE },
		{ LPGENW("Ki&ck (reason)"), 6, MENU_POPUPITEM, FALSE },
		{ LPGENW("&Ban"), 7, MENU_POPUPITEM, FALSE },      //20
		{ LPGENW("Ban'&n kick"), 8, MENU_POPUPITEM, FALSE },
		{ LPGENW("Ban'n kick (&reason)"), 9, MENU_POPUPITEM, FALSE },
		{ LPGENW("&Direct Connection"), 0, MENU_NEWPOPUP, FALSE },
		{ LPGENW("Request &Chat"), 13, MENU_POPUPITEM, FALSE },
		{ LPGENW("Send &File"), 14, MENU_POPUPITEM, FALSE },      //25
		{ LPGENW("Add to &ignore list"), 15, MENU_ITEM, FALSE },
		{ L"", 12, MENU_SEPARATOR, FALSE },
		{ LPGENW("&Add User"), 30, MENU_ITEM, FALSE }
};

int __cdecl CIrcProto::GCMenuHook(WPARAM, LPARAM lParam)
{
	GCMENUITEMS *gcmi = (GCMENUITEMS*)lParam;
	if (gcmi) {
		if (!mir_strcmpi(gcmi->pszModule, m_szModuleName)) {
			if (gcmi->Type == MENU_ON_LOG) {
				if (mir_wstrcmpi(gcmi->pszID, SERVERWINDOW)) {
					gcmi->nItems = _countof(logItems);
					gcmi->Item = logItems;
				}
				else gcmi->nItems = 0;
			}

			if (gcmi->Type == MENU_ON_NICKLIST) {
				CONTACT user = { (wchar_t*)gcmi->pszUID, NULL, NULL, false, false, false };
				MCONTACT hContact = CList_FindContact(&user);

				gcmi->nItems = _countof(nickItems);
				gcmi->Item = nickItems;
				BOOL bIsInList = (hContact && db_get_b(hContact, "CList", "NotOnList", 0) == 0);
				gcmi->Item[gcmi->nItems - 1].bDisabled = bIsInList;

				unsigned long ulAdr = 0;
				if (m_manualHost)
					ulAdr = ConvertIPToInteger(m_mySpecifiedHostIP);
				else
					ulAdr = ConvertIPToInteger(m_IPFromServer ? m_myHost : m_myLocalHost);
				gcmi->Item[23].bDisabled = ulAdr == 0 ? TRUE : FALSE;		//DCC submenu

				wchar_t stzChanName[100];
				const wchar_t* temp = wcschr(gcmi->pszID, ' ');
				size_t len = min(((temp == NULL) ? mir_wstrlen(gcmi->pszID) : (int)(temp - gcmi->pszID + 1)), _countof(stzChanName) - 1);
				mir_wstrncpy(stzChanName, gcmi->pszID, len);
				stzChanName[len] = 0;
				CHANNELINFO *wi = (CHANNELINFO *)DoEvent(GC_EVENT_GETITEMDATA, stzChanName, NULL, NULL, NULL, NULL, NULL, false, false, 0);
				BOOL bServOwner = strchr(sUserModes.c_str(), 'q') == NULL ? FALSE : TRUE;
				BOOL bServAdmin = strchr(sUserModes.c_str(), 'a') == NULL ? FALSE : TRUE;
				BOOL bOwner = bServOwner ? ((wi->OwnMode >> 4) & 01) : FALSE;
				BOOL bAdmin = bServAdmin ? ((wi->OwnMode >> 3) & 01) : FALSE;
				BOOL bOp = strchr(sUserModes.c_str(), 'o') == NULL ? FALSE : ((wi->OwnMode >> 2) & 01);
				BOOL bHalfop = strchr(sUserModes.c_str(), 'h') == NULL ? FALSE : ((wi->OwnMode >> 1) & 01);

				BOOL bForceEnable = GetAsyncKeyState(VK_CONTROL);

				gcmi->Item[6].bDisabled /* "Control" submenu */ = !(bForceEnable || bHalfop || bOp || bAdmin || bOwner);
				gcmi->Item[7].uType = gcmi->Item[8].uType = /* +/- Owner */ bServOwner ? MENU_POPUPITEM : 0;
				gcmi->Item[9].uType = gcmi->Item[10].uType = /* +/- Admin */ bServAdmin ? MENU_POPUPITEM : 0;
				gcmi->Item[7].bDisabled = gcmi->Item[8].bDisabled = gcmi->Item[9].bDisabled = gcmi->Item[10].bDisabled = /* +/- Owner/Admin */
					!(bForceEnable || bOwner);
				gcmi->Item[11].bDisabled = gcmi->Item[12].bDisabled = gcmi->Item[13].bDisabled = gcmi->Item[14].bDisabled = /* +/- Op/hop */
					!(bForceEnable || bOp || bAdmin || bOwner);
			}
		}
	}

	return 0;
}

int __cdecl CIrcProto::OnPreShutdown(WPARAM, LPARAM)
{
	mir_cslock lock(cs);

	if (m_perform && IsConnected())
		if (DoPerform("Event: Disconnect"))
			Sleep(200);

	DisconnectAllDCCSessions(true);

	if (IsConnected())
		Disconnect();
	if (m_listDlg)
		m_listDlg->Close();
	if (m_nickDlg)
		m_nickDlg->Close();
	if (m_joinDlg)
		m_joinDlg->Close();
	return 0;
}

int __cdecl CIrcProto::OnMenuPreBuild(WPARAM hContact, LPARAM)
{
	DBVARIANT dbv;
	if (hContact == NULL)
		return 0;

	char *szProto = GetContactProto(hContact);
	if (szProto && !mir_strcmpi(szProto, m_szModuleName)) {
		bool bIsOnline = getWord(hContact, "Status", ID_STATUS_OFFLINE) != ID_STATUS_OFFLINE;

		// context menu for chatrooms
		if (getByte(hContact, "ChatRoom", 0) == GCW_CHATROOM)
			Menu_ShowItem(hUMenuChanSettings, true);

		// context menu for contact
		else if (!getWString(hContact, "Default", &dbv)) {
			Menu_ShowItem(hUMenuChanSettings, false);

			// for DCC contact
			BYTE bDcc = getByte(hContact, "DCC", 0);
			if (bDcc)
				Menu_ShowItem(hUMenuDisconnect, true);
			else {
				// for normal contact
				Menu_ShowItem(hUMenuWhois, IsConnected());

				bool bEnabled = true;
				if (bIsOnline) {
					DBVARIANT dbv3;
					if (!getString(hContact, "Host", &dbv3)) {
						if (dbv3.pszVal[0] == 0)
							bEnabled = false;
						db_free(&dbv3);
					}
				}
				Menu_ShowItem(hUMenuIgnore, bEnabled);
			}
			db_free(&dbv);
		}
	}

	return 0;
}

int __cdecl CIrcProto::OnDbSettingChanged(WPARAM hContact, LPARAM lParam)
{
	if (hContact == NULL || !IsConnected())
		return 0;

	DBCONTACTWRITESETTING* cws = (DBCONTACTWRITESETTING*)lParam;
	if (strcmp(cws->szModule, "CList"))
		return 0;

	if (cws->value.type != DBVT_DELETED && !(cws->value.type == DBVT_BYTE && cws->value.bVal == 0))
		return 0;

	if (!strcmp(cws->szSetting, "NotOnList")) {
		DBVARIANT dbv;
		if (!getWString(hContact, "Nick", &dbv)) {
			if (getByte("MirVerAutoRequest", 1))
				PostIrcMessage(L"/PRIVMSG %s \001VERSION\001", dbv.ptszVal);
			db_free(&dbv);
		}
	}
	return 0;
}
void __cdecl CIrcProto::ConnectServerThread(void*)
{
	Thread_SetName("IRC: ConnectServer");

	InterlockedIncrement((long *)&m_bConnectThreadRunning);
	InterlockedIncrement((long *)&m_bConnectRequested);
	while (!Miranda_Terminated() && m_bConnectRequested > 0) {
		while (m_bConnectRequested > 0)
			InterlockedDecrement((long *)&m_bConnectRequested);
		if (IsConnected()) {
			Sleep(200);
			Disconnect();
		}

		m_info.bNickFlag = false;
		int Temp = m_iStatus;
		m_iStatus = ID_STATUS_CONNECTING;
		nickflag = true;
		ProtoBroadcastAck(NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE)Temp, ID_STATUS_CONNECTING);
		Sleep(100);
		{
			mir_cslock lock(cs);
			Connect(si);
		}
		if (IsConnected()) {
			if (m_mySpecifiedHost[0])
				ForkThread(&CIrcProto::ResolveIPThread, new IPRESOLVE(m_mySpecifiedHost, IP_MANUAL));

			DoEvent(GC_EVENT_CHANGESESSIONAME, SERVERWINDOW, NULL, m_info.sNetwork.c_str(), NULL, NULL, NULL, FALSE, TRUE);
		}
		else {
			Temp = m_iDesiredStatus;
			m_iStatus = m_iDesiredStatus = ID_STATUS_OFFLINE;
			ProtoBroadcastAck(NULL, ACKTYPE_LOGIN, ACKRESULT_FAILED, NULL, LOGINERR_NONETWORK);
			ProtoBroadcastAck(NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE)Temp, ID_STATUS_OFFLINE);
			Sleep(100);
		}
	}

	InterlockedDecrement((long *)&m_bConnectThreadRunning);
}

void __cdecl CIrcProto::DisconnectServerThread(void*)
{
	Thread_SetName("IRC: DisconnectServer");

	mir_cslock lck(cs);
	if (IsConnected())
		Disconnect();
	return;
}

void CIrcProto::ConnectToServer(void)
{
	m_portCount = atoi(m_portStart);
	si.sServer = GetWord(m_serverName, 0);
	si.iPort = m_portCount;
	si.sNick = m_nick;
	si.sUserID = m_userID;
	si.sFullName = m_name;
	si.sPassword = m_password;
	si.bIdentServer = ((m_ident) ? (true) : (false));
	si.iIdentServerPort = _wtoi(m_identPort);
	si.sIdentServerType = m_identSystem;
	si.m_iSSL = m_iSSL;
	si.sNetwork = m_network;

	bPerformDone = false;
	bTempDisableCheck = false;
	bTempForceCheck = false;
	m_iTempCheckTime = 0;
	sChannelPrefixes = L"&#";
	sUserModes = "ov";
	sUserModePrefixes = L"@+";
	sChannelModes = "btnimklps";

	if (!m_bConnectThreadRunning)
		ForkThread(&CIrcProto::ConnectServerThread, 0);
	else if (m_bConnectRequested < 1)
		InterlockedIncrement((long *)&m_bConnectRequested);

	wchar_t szTemp[300];
	mir_snwprintf(szTemp, L"\033%s \002%s\002 (%S: %u)",
		TranslateT("Connecting to"), si.sNetwork.c_str(), si.sServer.c_str(), si.iPort);
	DoEvent(GC_EVENT_INFORMATION, SERVERWINDOW, NULL, szTemp, NULL, NULL, NULL, true, false);
}

void CIrcProto::DisconnectFromServer(void)
{
	if (m_perform && IsConnected())
		DoPerform("Event: Disconnect");

	GCDEST gcd = { m_szModuleName, 0, GC_EVENT_CONTROL };
	GCEVENT gce = { sizeof(gce), &gcd };
	CallChatEvent(SESSION_TERMINATE, &gce);
	ForkThread(&CIrcProto::DisconnectServerThread, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////
// GetMyAwayMsg - obtain the current away message

INT_PTR __cdecl CIrcProto::GetMyAwayMsg(WPARAM wParam, LPARAM lParam)
{
	if ((int)wParam != m_iStatus)
		return 0;

	const wchar_t* p = m_statusMessage.c_str();

	return (lParam & SGMA_UNICODE) ? (INT_PTR)mir_wstrdup(p) : (INT_PTR)mir_u2a(p);
}
