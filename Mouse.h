// Mouse.h: interface for the CMouse class.
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

#if !defined(AFX_MOUSE_H__75382596_0851_47AC_87B0_9A059C9FD0B2__INCLUDED_)
#define AFX_MOUSE_H__75382596_0851_47AC_87B0_9A059C9FD0B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMouse  
{
public:
	void Event(HWND hWnd, int x, int y, int ButtonMask);
	CMouse();
	virtual ~CMouse();

private:
	int m_iLastButtonMask;
	int m_iLastY;
	int m_iLastX;
};

#endif // !defined(AFX_MOUSE_H__75382596_0851_47AC_87B0_9A059C9FD0B2__INCLUDED_)
