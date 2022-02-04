// SvrMain.h: interface for the CSvrMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SVRMAIN_H__6F940AA8_5C0C_4D90_85D9_96C6757DE3A8__INCLUDED_)
#define AFX_SVRMAIN_H__6F940AA8_5C0C_4D90_85D9_96C6757DE3A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>

#include "ScrUpdate.h"

class CSvrMain  
{
public:
  CSvrMain(HWND hWnd);
	virtual ~CSvrMain();
  
	int  WaitForIncomingConnection(int post);
	int  Run(HWND hWnd);
	int  Negotiate(void);
	int  Connect(char *host, int port);
  void CloseConnections(void);

	CScrUpdate m_ScrUpdate;

private:
  HWND       m_hwnd;
  SOCKET     m_sock, s_sock;
};

#endif // !defined(AFX_SVRMAIN_H__6F940AA8_5C0C_4D90_85D9_96C6757DE3A8__INCLUDED_)
