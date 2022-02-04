// ScrUpdate.cpp: implementation of the CScrUpdate class.
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
#include "ScrUpdate.h"

#include "rfb.h"

#ifdef COLOR_24BIT
typedef CARD32 MAXPIXELTYPE;
#else
typedef CARD16 MAXPIXELTYPE;
#endif


/* Update thread */
DWORD WINAPI RectangleSend( LPVOID parent )
{
	/* Static makes it faster */
	char buffer[X_BLOCK_SIZE*Y_BLOCK_SIZE*sizeof(MAXPIXELTYPE) + sizeof(rfbFramebufferUpdateRectHeader) + sizeof(rfbFramebufferUpdateMsg)];
	static BYTE AllZero[X_BLOCK_SIZE * Y_BLOCK_SIZE * sizeof(MAXPIXELTYPE)] = {0};
	CScrUpdate                     *su = (CScrUpdate *) parent;
	rfbFramebufferUpdateMsg        *fbup = (rfbFramebufferUpdateMsg *) buffer;
	rfbFramebufferUpdateRectHeader *surh = (rfbFramebufferUpdateRectHeader*) (fbup + 1);
	BYTE                           *startpix = (BYTE *) (surh + 1);
	int                            x,y,j;
	BYTE                           *pix;
  int                            x_blocks = su->m_x_blocks;
  int                            y_blocks = su->m_y_blocks;
	HBITMAP                        hBMP, hBMPCmpCopy;
	HDC                            hDCDesktop, hDCCopy, hDCCmpCopy;
	RECT                           rc;
	HBITMAP                        hDIB, hDIBCmp;
  int                            BytesPerPixel, BytesPerLine;
  int                            MouseXsize, MouseYsize;
  POINT                          LastMousePos = {-1};
  BITMAPINFO                     bm = {0};
	
  MouseXsize = GetSystemMetrics(SM_CXCURSOR);
  MouseYsize = GetSystemMetrics(SM_CYCURSOR);
  if (MouseXsize * MouseYsize > X_BLOCK_SIZE * Y_BLOCK_SIZE) {
    MessageBox(NULL, TEXT("XBLOCK, YBLOCK too small for xursor"), TEXT("Recompile lamer"), MB_OK);
    return -1;
  }

  hDCDesktop = GetDC(NULL) ;
	hDCCopy    = CreateCompatibleDC(hDCDesktop);
	hDCCmpCopy = CreateCompatibleDC(hDCDesktop);	
  
	GetWindowRect(GetDesktopWindow(), &rc);
	
	hBMP        = CreateBitmap(rc.right, rc.bottom, 1, su->m_iBitsPerPixel, NULL);
	hBMPCmpCopy = CreateBitmap(X_BLOCK_SIZE, Y_BLOCK_SIZE, 1, su->m_iBitsPerPixel, NULL);
	
	SelectObject(hDCCopy, (HBITMAP) hBMP);
	SelectObject(hDCCmpCopy, (HBITMAP) hBMPCmpCopy);
	
  if (su->m_iBitsPerPixel > 16)
	  bm.bmiHeader.biBitCount = 32;
  else
	  bm.bmiHeader.biBitCount = su->m_iBitsPerPixel;

  BytesPerPixel = bm.bmiHeader.biBitCount / 8;
  BytesPerLine  = BytesPerPixel * X_BLOCK_SIZE;

	bm.bmiHeader.biHeight    = rc.bottom;
	bm.bmiHeader.biWidth     = rc.right;
	bm.bmiHeader.biPlanes    = 1;
	bm.bmiHeader.biSize      = sizeof(bm.bmiHeader);
	bm.bmiHeader.biSizeImage = ((((rc.right * bm.bmiHeader.biBitCount) + (bm.bmiHeader.biBitCount-1)) & ~(bm.bmiHeader.biBitCount-1)) >> 3) * rc.bottom;
	bm.bmiHeader.biXPelsPerMeter = 3780;
	bm.bmiHeader.biYPelsPerMeter = 3780;
	
	LPVOID pBuffer;
	hDIB = CreateDIBSection(hDCCopy, &bm, (bm.bmiHeader.biBitCount <= 8) ? DIB_PAL_COLORS : DIB_RGB_COLORS, (void**)&pBuffer, NULL, 0);
	
  bm.bmiHeader.biHeight    = Y_BLOCK_SIZE;
	bm.bmiHeader.biWidth     = X_BLOCK_SIZE;
	bm.bmiHeader.biSizeImage = ((((X_BLOCK_SIZE * bm.bmiHeader.biBitCount) + (bm.bmiHeader.biBitCount-1)) & ~(bm.bmiHeader.biBitCount-1)) >> 3) * Y_BLOCK_SIZE;
	
	LPVOID pCmpBuffer;
  hDIBCmp = CreateDIBSection(hDCCmpCopy, &bm, (bm.bmiHeader.biBitCount <= 8) ? DIB_PAL_COLORS : DIB_RGB_COLORS, (void**)&pCmpBuffer, NULL, 0);

  bm.bmiHeader.biHeight    = MouseYsize;
	bm.bmiHeader.biWidth     = MouseXsize;
	bm.bmiHeader.biSizeImage = ((((MouseXsize * bm.bmiHeader.biBitCount) + (bm.bmiHeader.biBitCount-1)) & ~(bm.bmiHeader.biBitCount-1)) >> 3) * MouseYsize;
	
	SelectObject(hDCCopy, hDIB);
	SelectObject(hDCCmpCopy, hDIBCmp);
	
	ReleaseDC(NULL, hDCDesktop);

  pix = startpix;
	
	while (su->m_ThreadRunning) 
	{
		for (y = 0; y < y_blocks; y++)
		{
      POINT Point;
      HCURSOR hCursor;

			Sleep(50);
      GetCursorPos(&Point);
      if (memcmp(&Point, &LastMousePos, sizeof(Point)) != 0) {
        hCursor = GetCursor();
        su->AddRectangle(LastMousePos.x, LastMousePos.y, LastMousePos.x + MouseXsize - 1, LastMousePos.y + MouseYsize - 1);
        DrawIconEx(hDCCopy, Point.x, Point.y, hCursor, 0, 0, 0, 0, DI_NORMAL);
        su->AddRectangle(Point.x, Point.y, Point.x + MouseXsize - 1, Point.y + MouseYsize - 1);
        LastMousePos = Point;
      }

			for (x = 0; x < x_blocks; x++)
			{
  			switch (su->m_rfbEncoding) {
					case rfbEncodingRaw:
					{
							int   yoffs = y * Y_BLOCK_SIZE;
							int   xoffs = x * X_BLOCK_SIZE;
              
							int   ylines, ystart;
              BYTE  *CmpBuf;

              ylines = Y_BLOCK_SIZE;
							
							hDCDesktop = GetDC(NULL);
              if (!su->m_UpdateBlock[x][y]) { /* Check if updating is required */
 							  CmpBuf = (BYTE *) pCmpBuffer;

							  BitBlt(hDCCmpCopy, 0, 0, X_BLOCK_SIZE, Y_BLOCK_SIZE, hDCDesktop, xoffs, yoffs, SRCCOPY);
							  BitBlt(hDCCmpCopy, 0, 0, X_BLOCK_SIZE, Y_BLOCK_SIZE, hDCCopy, xoffs, yoffs, SRCINVERT);
							
							  /* Check if something changed and find last line that changed at the same time */
							  while (ylines && memcmp(AllZero, CmpBuf, BytesPerLine) == 0) {
								  ylines--;
								  CmpBuf += BytesPerLine;
                }
              }
							
  					  if (ylines) {

                if (!su->m_UpdateBlock[x][y]) {
	 							  CmpBuf = (BYTE *) pCmpBuffer + (X_BLOCK_SIZE * (Y_BLOCK_SIZE - 1)) * BytesPerPixel;
  							  ystart = 0;
								  while (memcmp(AllZero, CmpBuf, BytesPerLine) == 0) 
								  {
								    ystart++;
								    CmpBuf -= BytesPerLine;
								  }
								  ylines -= ystart;
								  yoffs += ystart;
                }
                else
                  su->m_UpdateBlock[x][y] = FALSE;

                /* Bit blit to copy of screen for comparison with desktop later on */
                BitBlt(hDCCopy, xoffs, yoffs, X_BLOCK_SIZE, ylines, hDCDesktop, xoffs, yoffs, SRCCOPY);
                /* TODO : Only draw when inside bounding rectangle of mouse */
                DrawIconEx(hDCCopy, Point.x, Point.y, hCursor, 0, 0, 0, 0, DI_NORMAL);
                
                fbup->type     = rfbFramebufferUpdate;
								fbup->pad      = 0;
								fbup->nRects   = Swap16IfLE(1);
								surh->r.x      = Swap16IfLE(x * X_BLOCK_SIZE);
								surh->r.y      = Swap16IfLE(yoffs);
								surh->r.h      = Swap16IfLE(ylines);
               	surh->r.w      = Swap16IfLE(X_BLOCK_SIZE);
              	surh->encoding = Swap32IfLE(su->m_rfbEncoding);

								yoffs = rc.bottom - yoffs - ylines;
                if (yoffs < 0)
                {
                  yoffs = yoffs; /* Problem with Y_BLOCK_SIZE not being multiple of Y resolution */
                }
								for (j = ylines-1; j >=0; j--) {
#if 1 /* For 16 bit palette */
									memcpy(pix, (BYTE*) pBuffer + (xoffs + (j + yoffs) * rc.right) * BytesPerPixel, X_BLOCK_SIZE * BytesPerPixel);
									pix += BytesPerLine;
#else /* For 8 bit pallette, not supported by client, therefore not used */
                  for (int k = 0 ; k < X_BLOCK_SIZE; k++) {
                    *((BYTE*) pix) = *((BYTE*) pBuffer + (xoffs + k + (j + yoffs) * rc.right));
                    pix += 1;
                  }
#endif
								}
								(void)send(su->m_sock, buffer, (char *) pix - buffer, 0);
                pix = startpix;
								
							}
							ReleaseDC(NULL, hDCDesktop);
							break;
						}
					case rfbEncodingCopyRect:
					case rfbEncodingRRE:
					case rfbEncodingCoRRE:
					case rfbEncodingHextile:
					default:
						MessageBox(NULL, TEXT("Encoding not supported"), TEXT("Send Thread"), MB_OK);
						break;
					}
				}
		}
	}

  DeleteObject(hDIBCmp);
  DeleteObject(hDIB);
  DeleteObject(hBMP);
  DeleteObject(hBMPCmpCopy);
  DeleteDC(hDCCopy);
  DeleteDC(hDCCmpCopy);
	
	return 0;
}


DWORD WINAPI ScanScreen(LPVOID parent)
{
  return 0;
}


CScrUpdate::CScrUpdate()
{
  m_iScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
  m_iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
  m_iBitsPerPixel = max(GetDeviceCaps(NULL, BITSPIXEL), 16); /* CE does not seem to support 8 bit DIB's */
#ifndef UNDER_CE
  m_iBitsPerPixel = 16; /* For some reason there is no problem under CE... fix later */
#endif

  
  m_x_blocks = (m_iScreenWidth + X_BLOCK_SIZE - 1) / X_BLOCK_SIZE;
  m_y_blocks = (m_iScreenHeight + Y_BLOCK_SIZE - 1) / Y_BLOCK_SIZE;

  if (m_iScreenWidth % X_BLOCK_SIZE ||
      m_iScreenHeight % Y_BLOCK_SIZE)
    MessageBox(NULL, TEXT("This is not gonna work....\nblock sizes need to be multiple\nof X&Y resolution.\nPrepare for GPF...."), TEXT("VNCSvr"), MB_OK);
  
  m_rfbEncoding = rfbEncodingRaw;
  m_bCopyRectSupported = TRUE;
}


CScrUpdate::~CScrUpdate()
{
  StopScan();
}


void CScrUpdate::AddRectangle(RECT r)
{
  AddRectangle(r.left, r.top, r.right, r.bottom);
}


void CScrUpdate::AddRectangle(int x0, int y0, int x1, int y1)
{
  /* Update the update array using all 4 points of the rectangle */
  for (int y = y0; y < y1; y += Y_BLOCK_SIZE)
    for (int x = x0; x < x1; x += X_BLOCK_SIZE)
      m_UpdateBlock[x/X_BLOCK_SIZE][y/Y_BLOCK_SIZE] = true;
}


void CScrUpdate::StartScan()
{
  /* Initially no blocks need to be send */
  for (int y = 0; y < MAX_BLOCKS_Y; y++)
    for (int x = 0 ; x < MAX_BLOCKS_X; x++)
      m_UpdateBlock[x][y] = false;

  /* Create thread that is gonna send rectangles to the client */
  m_ThreadRunning = TRUE;
  m_Thread = CreateThread(NULL, 0, RectangleSend, this, 0, NULL ); 
}


void CScrUpdate::StopScan()
{
  BOOL  res;
  DWORD ExitCode;
  
  if (m_ThreadRunning) {
    m_ThreadRunning = FALSE;
    
    WaitForSingleObject(m_Thread, INFINITE);
    res = GetExitCodeThread(m_Thread, &ExitCode); 
  }

}
