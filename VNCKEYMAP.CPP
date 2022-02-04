//  Copyright (C) 1999 AT&T Laboratories Cambridge. All Rights Reserved.
//
//  This file is part of the VNC system.
//
//  The VNC system is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// If the source code for the VNC system is not available from the place 
// whence you received this file, check http://www.uk.research.att.com/vnc or contact
// the authors on vnc@uk.research.att.com for information on obtaining it.

// Changes made on original code :
//   This code originally just mapped between X keysyms and local Windows
//   virtual keycodes.  Now it actually does the local-end simulation of
//   key presses, to keep this code on one place!

#include "vncKeymap.h"

#include "keysymdef.h"

// Define the keymap structure
typedef struct vncKeymapping_struct {
	UINT wincode;
	UINT Xcode;
} vncKeymapping;

const UINT ignorekeymap[] = {
	XK_Scroll_Lock,
	XK_Caps_Lock,
	XK_Num_Lock
};

const vncKeymapping keymap[] = {
    {'`',		XK_dead_grave},
    {'´',		XK_dead_acute},
    {'~',		XK_dead_tilde},
    {'^',		XK_dead_circumflex},
    {VK_BACK,		XK_BackSpace},
    {VK_TAB,		XK_Tab},
    {VK_CLEAR,		XK_Clear},
    {VK_RETURN,		XK_Return},
    {VK_LSHIFT,		XK_Shift_L},
    {VK_RSHIFT,		XK_Shift_R},
    {VK_LCONTROL,	XK_Control_L},
    {VK_RCONTROL,	XK_Control_R},
    {VK_LMENU,		XK_Alt_L},
    {VK_RMENU,		XK_Alt_R},
    {VK_PAUSE,		XK_Pause},
    {VK_CAPITAL,	XK_Caps_Lock},
    {VK_ESCAPE,		XK_Escape},
    {VK_SPACE,		XK_space},
    {VK_PRIOR,		XK_Page_Up},
    {VK_NEXT,		XK_Page_Down},
    {VK_END,		XK_End},
    {VK_HOME,		XK_Home},
    {VK_LEFT,		XK_Left},
    {VK_UP,		XK_Up},
    {VK_RIGHT,		XK_Right},
    {VK_DOWN,		XK_Down},
    {VK_SELECT,		XK_Select},
    {VK_EXECUTE,	XK_Execute},
    {VK_SNAPSHOT,	XK_Print},
    {VK_INSERT,		XK_Insert},
    {VK_DELETE,		XK_Delete},
    {VK_HELP,		XK_Help},
    {VK_NUMPAD0,	XK_KP_0},
    {VK_NUMPAD1,	XK_KP_1},
    {VK_NUMPAD2,	XK_KP_2},
    {VK_NUMPAD3,	XK_KP_3},
    {VK_NUMPAD4,	XK_KP_4},
    {VK_NUMPAD5,	XK_KP_5},
    {VK_NUMPAD6,	XK_KP_6},
    {VK_NUMPAD7,	XK_KP_7},
    {VK_NUMPAD8,	XK_KP_8},
    {VK_NUMPAD9,	XK_KP_9},
    {VK_SPACE,		XK_KP_Space},
    {VK_TAB,		XK_KP_Tab},
    {VK_RETURN,		XK_KP_Enter},
    {VK_F1,		XK_KP_F1},
    {VK_F2,		XK_KP_F2},
    {VK_F3,		XK_KP_F3},
    {VK_F4,		XK_KP_F4},
    {VK_HOME,		XK_KP_Home},
    {VK_LEFT,		XK_KP_Left},
    {VK_UP,		XK_KP_Up},
    {VK_RIGHT,		XK_KP_Right},
    {VK_DOWN,		XK_KP_Down},
    {VK_PRIOR,		XK_KP_Prior},
    {VK_PRIOR,		XK_KP_Page_Up},
    {VK_NEXT,		XK_KP_Next},
    {VK_NEXT,		XK_KP_Page_Down},
    {VK_END,		XK_KP_End},
    {VK_INSERT,		XK_KP_Insert},
    {VK_DELETE,		XK_KP_Delete},
    {VK_MULTIPLY,	XK_KP_Multiply},
    {VK_ADD,		XK_KP_Add},
    {VK_SEPARATOR,	XK_KP_Separator},
    {VK_SUBTRACT,	XK_KP_Subtract},
    {VK_DECIMAL,	XK_KP_Decimal},
    {VK_DIVIDE,		XK_KP_Divide},
    {VK_F1,		XK_F1},
    {VK_F2,		XK_F2},
    {VK_F3,		XK_F3},
    {VK_F4,		XK_F4},
    {VK_F5,		XK_F5},
    {VK_F6,		XK_F6},
    {VK_F7,		XK_F7},
    {VK_F8,		XK_F8},
    {VK_F9,		XK_F9},
    {VK_F10,		XK_F10},
    {VK_F11,		XK_F11},
    {VK_F12,		XK_F12},
    {VK_F13,		XK_F13},
    {VK_F14,		XK_F14},
    {VK_F15,		XK_F15},
    {VK_F16,		XK_F16},
    {VK_F17,		XK_F17},
    {VK_F18,		XK_F18},
    {VK_F19,		XK_F19},
    {VK_F20,		XK_F20},
    {VK_F21,		XK_F21},
    {VK_F22,		XK_F22},
    {VK_F23,		XK_F23},
    {VK_F24,		XK_F24},
    {VK_NUMLOCK,	XK_Num_Lock},
    {VK_SCROLL,		XK_Scroll_Lock}
};

const VkScanKeyMap[] = {0x0020, 0x0131, 0x01DE, 0x0133, 0x0134, 0x0135, 0x0137, 
											  0x00DE, 0x0139, 0x0130, 0x0138, 0x01BB, 0x00BC, 0x00BD, 
												0x00BE, 0x00BF, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 
												0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x01BA, 0x00BA, 
												0x01BC, 0x00BB, 0x01BE, 0x01BF, 0x0132, 0x0141, 0x0142, 
												0x0143, 0x0144, 0x0145, 0x0146, 0x0147, 0x0148, 0x0149, 
												0x014A, 0x014B, 0x014C, 0x014D, 0x014E, 0x014F, 0x0150, 
												0x0151, 0x0152, 0x0153, 0x0154, 0x0155, 0x0156, 0x0157, 
												0x0158, 0x0159, 0x015A, 0x00DB, 0x00DC, 0x00DD, 0x0136, 
												0x01BD, 0x00C0, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 
												0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 
												0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 
												0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 
												0x01DB, 0x01DC, 0x01DD, 0x01C0, 0x0208};

SHORT VkScanKey(char t)
{
	if (t<32 || t>127)
		return -1;
	return VkScanKeyMap[t-32];
}


vncKeymap::vncKeymap()
{
};

void
KeybdEvent(BYTE keycode, DWORD flags)
{
	// Send the desired keyboard event
	keybd_event(keycode, MapVirtualKey(keycode, 0), flags, 0);
}

void
SetShiftState(BYTE key, BOOL down)
{
	BOOL keystate = (GetAsyncKeyState(key) & 0x8000) != 0;

	// This routine sets the specified key to the desired value (up or down)
	if ((keystate && down) || ((!keystate) && (!down)))
		return;

	// Now send a key event to set the key to the new value
	KeybdEvent(key, down ? 0 : KEYEVENTF_KEYUP);
	keystate = (GetAsyncKeyState(key) & 0x8000) != 0;

}

void
vncKeymap::DoXkeysym(CARD32 keysym, BOOL keydown)
{
	int i;

	// Cull out some particular keysyms
	for (i=0; i < (sizeof(ignorekeymap) / sizeof(UINT)); i++)
		if (ignorekeymap[i] == keysym)
			return;

	// First just try to map the virtual keycode using our lookup-table
	for (i = 0; i < (sizeof(keymap) / sizeof(vncKeymapping)); i++)
	{
		if (keymap[i].Xcode == keysym)
		{
			UINT virtcode = keymap[i].wincode;

			// Now simulate the keyboard event
			KeybdEvent((unsigned char) (virtcode & 255), keydown ? 0 : KEYEVENTF_KEYUP);
			return;
		}
	}

	// Otherwise, get the OS to tell us how to simulate it
	SHORT keyval = VkScanKey((char) (keysym & 255)); //VkKeyScan((char) (keysym & 255));

	//keyval = (GetKeyState(keyval) << 8) | keyval;
	
	// Retrieve the keycode and shift state
	BYTE keycode = LOBYTE(keyval);
	BYTE keymask = HIBYTE(keyval);
	if (keycode == -1)
		return;

	BOOL lshift;
	BOOL ctrl;
	BOOL alt;

	// Correct the keymask shift state to cope with the capslock state
	// Changed to 'Async' by qsf 27/4/99.
	BOOL capslock = (GetAsyncKeyState(VK_CAPITAL) & 0x8000) != 0;
	keymask = capslock ? keymask ^ 1 : keymask;

	// No, so the modifier keys come in only one flavour

	// Toggle shift to the required position
	lshift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

	SetShiftState(VK_SHIFT, keymask & 1);

	// But only toggle Ctrl & Alt if they aught to be down
	ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
	if (!ctrl) SetShiftState(VK_CONTROL, keymask & 2);
	alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
	if (!alt) SetShiftState(VK_MENU, keymask & 4);

	// Now send the desired keycode
	KeybdEvent((unsigned char) (keycode & 255), keydown ? 0 : KEYEVENTF_KEYUP);

	// No, so just reset the single versions
	SetShiftState(VK_SHIFT, lshift);
	SetShiftState(VK_CONTROL, ctrl);
	SetShiftState(VK_MENU, alt);

}

void
vncKeymap::ClearShiftKeys()
{
	// Otherwise, we can't distinguish the keys anyway...

	// Clear the shift key states
	SetShiftState(VK_SHIFT, FALSE);
	SetShiftState(VK_CONTROL, FALSE);
	SetShiftState(VK_MENU, FALSE);
}


