// SvrMain.cpp: implementation of the CSvrMain class.
//
//////////////////////////////////////////////////////////////////////

/*
 *  Copyright (C) 2002 Maesc software consultants. All Rights Reserved.
 *
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 *  USA.
 */

#include "SvrMain.h"

#include "winuser.h"
#include <windows.h>
#include <stdio.h>
#include <winsock.h>

#include "VNCKeyMap.h"
#include "Mouse.h"
#include "rfb.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSvrMain::CSvrMain(HWND hWnd)
{
  WORD    wVersionRequested;
  WSADATA wsaData;

  m_sock = 0;
  s_sock = 0;

  wVersionRequested = MAKEWORD(2, 0);
  if (WSAStartup(wVersionRequested, &wsaData) != 0)
  {
    MessageBox(NULL, TEXT("Socket init error"), TEXT("VNCsvr"), MB_OK);
	  PostQuitMessage(0);
  }
  m_hwnd = hWnd;
}


CSvrMain::~CSvrMain()
{
  CloseConnections();
  (void) WSACleanup();
}


int CSvrMain::Connect(char *host, int port)
{
  struct sockaddr_in thataddr;
	int res;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)  {
		MessageBox(NULL, TEXT("Error creating socket"), TEXT("VNCsvr"), MB_OK);
		PostQuitMessage(0);
    return -1;
	}
  m_ScrUpdate.m_sock = m_sock;
		
	// The host may be specified as a dotted address "a.b.c.d"
	// Try that first
	thataddr.sin_addr.s_addr = inet_addr(host);
	
	// If it wasn't one of those, do gethostbyname
	if (thataddr.sin_addr.s_addr == INADDR_NONE) {
		LPHOSTENT lphost;
		lphost = gethostbyname(host);
		
		if (lphost == NULL) { 
			MessageBox(NULL, TEXT("Failed to get server address.\n\rDid you type the host name correctly?"), TEXT("VNCsvr"), MB_OK);
      PostQuitMessage(0);
      return -1;
    };
		thataddr.sin_addr.s_addr = ((LPIN_ADDR) lphost->h_addr)->s_addr;
	};
	
	thataddr.sin_family = AF_INET;
	thataddr.sin_port = htons(port);
	res = connect(m_sock, (LPSOCKADDR) &thataddr, sizeof(thataddr));
  if (res == SOCKET_ERROR)  {
    MessageBox(NULL, TEXT("Failed to connect to client"), TEXT("VNCsvr"), MB_OK);
    PostQuitMessage(0);
    return -1;
  }
  return 0;
}


int CSvrMain::Negotiate()
/* Could use some better error handling and be more robust agains receiving smaller 
   packets than expected....
 */
{
  char   str[80];
  CARD32 authScheme;
  int    len;
  rfbServerInitMsg si;
  rfbSetPixelFormatMsg spf;
	int                  SendPallette = FALSE;

  /* Communicate protocol */
  sprintf(str,rfbProtocolVersionFormat, rfbProtocolMajorVersion, rfbProtocolMinorVersion);
  if (send(m_sock, str, strlen(str), 0) <= 0) {
    MessageBox(NULL, TEXT("Error sending ID"), TEXT("VNCsvr"), MB_OK);
    return -1;
  }

  if (recv(m_sock, str, sizeof(str)-1, 0) <= 0) {
    MessageBox(NULL, TEXT("Error receiving ID"), TEXT("VNCsvr"), MB_OK);
    return -1;
  }

  /* Communicate required authentication... none */
  authScheme = Swap32IfLE(rfbNoAuth);
  (void) send(m_sock, (char *) &authScheme, sizeof(CARD32), 0);

  /* Receive init msg ... a '0' */
  if ((len = recv(m_sock, str, 1, 0)) <= 0) {
    MessageBox(NULL, TEXT("Error receiving ID"), TEXT("VNCsvr"), MB_OK);
    return -1;
  }

  /* Send system information */
  si.framebufferWidth    = Swap16IfLE(GetSystemMetrics(SM_CXSCREEN));
  si.framebufferHeight   = Swap16IfLE(GetSystemMetrics(SM_CYSCREEN));
  si.nameLength          = Swap32IfLE(0); /* No name to follow */
	si.format.bitsPerPixel = (m_ScrUpdate.m_iBitsPerPixel == 24) ? 32 : m_ScrUpdate.m_iBitsPerPixel;
  si.format.depth        = m_ScrUpdate.m_iBitsPerPixel;	/* 8 to 32 */
  si.format.bigEndian    = 0;		/* True if multi-byte pixels are interpreted
				   as big endian, or if single-bit-per-pixel
				   has most significant bit of the byte
				   corresponding to first (leftmost) pixel. Of
				   course this is meaningless for 8 bits/pix */
	si.format.trueColour   = (m_ScrUpdate.m_iBitsPerPixel > 8) ? 1 : 0;		/* If false then we need a "colour map" to
				   convert pixels to RGB.  If true, xxxMax and
				   xxxShift specify bits used for red, green
				   and blue */
  if (m_ScrUpdate.m_iBitsPerPixel == 24) {
		si.format.redMax     = Swap16IfLE(0xFF);	  /* maximum red value */
		si.format.greenMax   = Swap16IfLE(0xFF);		/* similar for green */
		si.format.blueMax    = Swap16IfLE(0xFF);		/* and blue */
		si.format.redShift   = 16; 		/* number of shifts needed to get the red */
		si.format.greenShift = 8;	  	/* similar for green */
		si.format.blueShift  = 0;	  	/* and blue */
	}
	else
		if (m_ScrUpdate.m_iBitsPerPixel == 16) {
			si.format.redMax     = Swap16IfLE(0x1F);	  /* maximum red value */
			si.format.greenMax   = Swap16IfLE(0x1F);		/* similar for green */
			si.format.blueMax    = Swap16IfLE(0x1F);		/* and blue */
#ifdef UNDER_CE
			si.format.redShift   = 0; 		/* number of shifts needed to get the red */
			si.format.greenShift = 5;	  	/* similar for green */
			si.format.blueShift  = 10;		/* and blue */
#else /* 'regular' Windows */
      si.format.redShift   = 10; 		/* number of shifts needed to get the red */
			si.format.greenShift = 5;	  	/* similar for green */
			si.format.blueShift  = 0;	  	/* and blue */
#endif
    }
		else { /* Assume 8 bit, does not work... */
			/* Add pallette colors */
			SendPallette = TRUE;
      si.format.redMax     = Swap16IfLE(0x3);	  /* maximum red value */
			si.format.greenMax   = Swap16IfLE(0x3);		/* similar for green */
			si.format.blueMax    = Swap16IfLE(0x3);		/* and blue */
			si.format.redShift   = 4; 		/* number of shifts needed to get the red */
			si.format.greenShift = 0;	  	/* similar for green */
			si.format.blueShift  = 2;		/* and blue */
      si.format.trueColour = 1;
		}
  (void) send(m_sock, (char *) &si, sizeof(si), 0);

  /* Receive system info from client */
  if (recv(m_sock, (char *) &spf, sizeof(rfbSetPixelFormatMsg), 0) != sizeof(rfbSetPixelFormatMsg)) {
    return -1;
  }

  if (spf.format.bitsPerPixel != si.format.bitsPerPixel ||
    spf.format.depth != si.format.depth) {
    MessageBox(NULL, TEXT("Client requested unsupported format"), TEXT("Vnc server"), MB_OK);
    return -1;
  }

  /* receive supported encoding types from client... ignore them... this assumes that whatever is received
     with this call is exactly one of those packets. It is theoretically possible that the received data
     also includes the full screen update request. This would mean that the 'Run' function does not receive
     it and the client screen stays blank....
   */
  (void) recv(m_sock, (char *) str, sizeof(str), 0);

  return 0;
}

int CSvrMain::Run(HWND hWnd)
{
  
  int len;
  char buffer[128];
  CMouse Mouse;
  rfbFramebufferUpdateRequestMsg *fur = (rfbFramebufferUpdateRequestMsg *) buffer;

  do {
    do {
      fd_set fdread;
      timeval tv;

      tv.tv_sec = 10;

      FD_ZERO(&fdread);
      FD_SET(m_sock, &fdread);
      
      int sret = select(0, &fdread, 0, 0, &tv);
      switch (sret)
      {
        case 0: // timeout
          break;
        case SOCKET_ERROR:
          len = SOCKET_ERROR;
          break;
        default:  // Data is available
          len = recv(m_sock, buffer, 1, 0); /* Read cmd */
          break;
      }
      
      if (len == 0 || len == SOCKET_ERROR) {
        //res = GetLastError();
        //if (res == WSAECONNABORTED)
          return WSAECONNABORTED;
      }
    } while (len <= 0);
    if (len <= 0)
      break;
    switch (buffer[0]) {
      case rfbFramebufferUpdateRequest :

        if (recv(m_sock, buffer + 1, sizeof(rfbFramebufferUpdateRequestMsg)-1, 0) != (sizeof(rfbFramebufferUpdateRequestMsg)-1)) {
          return WSAECONNABORTED;
        }
        if (fur->incremental == 0) {
          fur->x = Swap16IfLE(fur->x);
          fur->y = Swap16IfLE(fur->y);
          fur->w = Swap16IfLE(fur->w);
          fur->h = Swap16IfLE(fur->h);

          /* Send full frame.... */
          m_ScrUpdate.AddRectangle(fur->x, fur->y, fur->x + fur->w - 1, fur->y + fur->h - 1);
        }
        break;

      case rfbEnableExtensionRequest :
        recv(m_sock, buffer + 1, sizeof(rfbEnableExtensionRequestMsg)-1, 0);
        break;
      case rfbFramebufferUpdate :
        recv(m_sock, buffer + 1, sizeof(rfbFramebufferUpdateMsg)-1, 0);
        break;
      case rfbKeyEvent:
			{
				rfbClientToServerMsg msg;
        if (recv(m_sock, (char *) &msg + 1, sizeof(rfbKeyEventMsg)-1, 0) == sizeof(rfbKeyEventMsg)-1) 
				{
					msg.ke.key = Swap32IfLE(msg.ke.key);

					vncKeymap vnckeys;				
					// Get the keymapper to do the work
					vnckeys.DoXkeysym(msg.ke.key, msg.ke.down);

				}
				break;
			}
        
      case rfbPointerEvent :
				{
					rfbClientToServerMsg msg;

					if (recv(m_sock, (char *) &msg + 1, sizeof(rfbPointerEventMsg)-1, 0) != sizeof(rfbPointerEventMsg)-1) {
						return WSAECONNABORTED; /* Dirty */
					}

          Mouse.Event(hWnd, (Swap16IfLE(msg.pe.x) * 65535) / (m_ScrUpdate.m_iScreenWidth),
                      (Swap16IfLE(msg.pe.y) * 65535) / (m_ScrUpdate.m_iScreenHeight), 
                      msg.pe.buttonMask);
        }
        break;
      
      default :
        len = len;
        break;
    }
  } while (1);

  return 0;
}


int CSvrMain::WaitForIncomingConnection(int port)
{
  struct sockaddr_in addr;

	// Create a listening socket
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  s_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!s_sock) 
    return -1;
    
  if (bind(s_sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR ||
    listen(s_sock, 5) == SOCKET_ERROR) {
    closesocket(s_sock);
    s_sock = 0;
		return -1;
	}

  m_sock = accept(s_sock, NULL, NULL);
  if (m_sock == SOCKET_ERROR) {
    closesocket(s_sock);
    s_sock = 0;
    return -1;
  }
  m_ScrUpdate.m_sock = m_sock;
	

  return 0;
}

void CSvrMain::CloseConnections(void)
{
    if (m_sock)
	  closesocket(m_sock);
  if (s_sock)
    closesocket(s_sock);
}