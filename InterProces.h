#ifndef _RFB_INTERPROCESS_H_
#define _RFB_INTERPROCESS_H_

#define RFB_CLASSNAME    "RFB"
#define RFB_WINDOWSNAME  "RFB Server"
#define RFB_UPDATERECT   (WM_APP - 1)

#define RFB_ENCODE_XY(x,y) ((x) << 16 | (y))
#define RFB_DECODE_X(xy)   ((xy) >> 16)
#define RFB_DECODE_Y(xy)   ((xy) & 0xFFFF)

#endif