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

// ScrUpdate.h: interface for the CScrUpdate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRUPDATE_H__353EC170_4455_4B67_B37C_16F0048E83F5__INCLUDED_)
#define AFX_SCRUPDATE_H__353EC170_4455_4B67_B37C_16F0048E83F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>

#include "rfb.h"

#define MAX_RESOLUTION_X  1280  
#define MAX_RESOLUTION_Y  1024
#define X_BLOCK_SIZE      64
#define Y_BLOCK_SIZE      32
#define MAX_BLOCKS_X     ((MAX_RESOLUTION_X+(X_BLOCK_SIZE-1))/X_BLOCK_SIZE)
#define MAX_BLOCKS_Y     ((MAX_RESOLUTION_Y+(Y_BLOCK_SIZE-1))/Y_BLOCK_SIZE)

class CScrUpdate  
{
public:
	void StopScan(void);
	void StartScan(void);
	void AddRectangle(int x0, int y0, int x1, int y1);
	void AddRectangle(RECT r);
	CScrUpdate();
	virtual ~CScrUpdate();

  int    m_ThreadRunning;
  SOCKET m_sock;
  BOOL   m_UpdateBlock[MAX_BLOCKS_X][MAX_BLOCKS_Y];
  CARD32 m_rfbEncoding;
  int    m_iScreenWidth;
  int    m_iScreenHeight;
	int    m_iBitsPerPixel;
  int    m_bCopyRectSupported;
  int    m_x_blocks;
  int    m_y_blocks;

private :
  HANDLE m_Thread;
};

#endif // !defined(AFX_SCRUPDATE_H__353EC170_4455_4B67_B37C_16F0048E83F5__INCLUDED_)
