// CEVncSvr.cpp : Defines the entry point for the application.
//

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
#include "stdafx.h"

#include "rfb.h"
#include "SvrMain.h"
#include "InterProces.h"


CSvrMain svr(NULL);
HWND     hWnd;


LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
  switch (uMsg) 
  { 
    case WM_CREATE: 
      // Initialize the window. 
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    case WM_PAINT:
      break;

    /* The following message is a back door, it allows applications you write to
       tell the VNC server which rectangle was updated. Makes life a little bit
       easier for the server.
    */
    case RFB_UPDATERECT : 
      svr.m_ScrUpdate.AddRectangle(RFB_DECODE_X(wParam), RFB_DECODE_Y(wParam), RFB_DECODE_X(lParam), RFB_DECODE_Y(lParam));
      break;

    default: 
      return DefWindowProc(hWnd, uMsg, wParam, lParam); 
  } 
  return 0; 
} 


DWORD WINAPI MainThread( LPVOID parent )
{
  while (1) {
    while (svr.WaitForIncomingConnection(RFB_PORT_OFFSET) == 0) {

      if (svr.Negotiate() != 0) {
        svr.CloseConnections();
        continue;
      }
      svr.m_ScrUpdate.StartScan();

      (void) svr.Run(hWnd);
      svr.CloseConnections();
      svr.m_ScrUpdate.StopScan();
    }
  }
  return 0;
}


int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
  WNDCLASS wc;
  MSG      msg;
  HANDLE   Thread;

  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC)MessageHandler;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = NULL;
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.lpszClassName = TEXT(RFB_CLASSNAME);
  wc.lpszMenuName = NULL;

  RegisterClass(&wc);
  
  hWnd = CreateWindowEx(
#ifdef UNDER_CE    
    WS_EX_NOACTIVATE,
#else
    WS_EX_DLGMODALFRAME, /* Not too concerned about what it does here under regular windows */
#endif
    TEXT(RFB_CLASSNAME), TEXT(RFB_WINDOWSNAME), 0, 0, 0, 20, 20, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, SW_HIDE);

  Thread = CreateThread(NULL, 0, MainThread, 0, 0, NULL ); 

  while (GetMessage(&msg, hWnd, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

	return 0;
}

