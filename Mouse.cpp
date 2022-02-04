// Mouse.cpp: implementation of the CMouse class.
//
//////////////////////////////////////////////////////////////////////

/*
 *  Copyright (C) 2002 Maesc software consultants.  All Rights Reserved.
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
#include "Mouse.h"

#include "rfb.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMouse::CMouse()
{
  m_iLastX          = -1;
  m_iLastY          = -1;
  m_iLastButtonMask = 0;
}

CMouse::~CMouse()
{

}

void CMouse::Event(HWND hWnd, int x, int y, int ButtonMask)
{
  DWORD flags = MOUSEEVENTF_ABSOLUTE;
  int update;

	if (x != m_iLastX || y != m_iLastY) {
		flags |= MOUSEEVENTF_MOVE;
		m_iLastX = x;
		m_iLastY = y;
		update = TRUE;
	}

	if ( (ButtonMask & rfbButton1Mask) != (m_iLastButtonMask & rfbButton1Mask) ) {
		flags |= (ButtonMask & rfbButton1Mask) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
 		update = TRUE;
	}

	if ( (ButtonMask & rfbButton2Mask) != (m_iLastButtonMask & rfbButton2Mask) )
	{
		flags |= (ButtonMask & rfbButton2Mask) 
				? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
		update = TRUE;
	}

	if ( (ButtonMask & rfbButton3Mask) != (m_iLastButtonMask & rfbButton3Mask) )
	{
		flags |= (ButtonMask & rfbButton3Mask) 
				? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
		update = TRUE;
	}

	if (update) {
		// Do the pointer event
		mouse_event(flags, (DWORD) x, (DWORD) y, 0, 0);
		m_iLastButtonMask = ButtonMask;
	}
}
