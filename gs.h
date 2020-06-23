//---------------------------------------------------------------------------
// File:	gs.h
// Author:	Tony Saveski, t_saveski@yahoo.com
// Notes:	Playstation2 GS Convenience Routines
//---------------------------------------------------------------------------
#ifndef GS_H
#define GS_H

#include <tamtypes.h>
#include "regs.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void gs_set_imr(void);
extern void gs_set_crtc(u8 int_mode, u8 ntsc_pal, u8 field_mode);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
// CSR Register
//---------------------------------------------------------------------------
#define CSR			((vu64 *)(csr))
#define GS_SET_CSR(SIGNAL,FINISH,HSINT,VSINT,EDWINT,FLUSH,RESET,NFIELD,FIELD,FIFO,REV,ID) \
	*CSR = \
	((u64)(SIGNAL)	<< 0)		| \
	((u64)(FINISH)	<< 1)		| \
	((u64)(HSINT)	<< 2)		| \
	((u64)(VSINT)	<< 3)		| \
	((u64)(EDWINT)	<< 4)		| \
	((u64)(FLUSH)	<< 8)		| \
	((u64)(RESET)	<< 9)		| \
	((u64)(NFIELD)	<< 12)		| \
	((u64)(FIELD)	<< 13)		| \
	((u64)(FIFO)		<< 14)		| \
	((u64)(REV)		<< 16)		| \
	((u64)(ID)		<< 24)

#define GS_RESET() \
	*CSR = ((u64)(1)	<< 9)

//---------------------------------------------------------------------------
// PMODE Register
//---------------------------------------------------------------------------
#define PMODE		((volatile u64 *)(pmode))
#define GS_SET_PMODE(EN1,EN2,MMOD,AMOD,SLBG,ALP) \
	*PMODE = \
	((u64)(EN1) 	<< 0) 	| \
	((u64)(EN2) 	<< 1) 	| \
	((u64)(001)	<< 2) 	| \
	((u64)(MMOD)	<< 5) 	| \
	((u64)(AMOD) << 6) 	| \
	((u64)(SLBG) << 7) 	| \
	((u64)(ALP) 	<< 8)

//---------------------------------------------------------------------------
// SMODE2 Register
//---------------------------------------------------------------------------
#define SMODE2		((vu64 *)(smode2))
#define GS_SET_SMODE2(INT,FFMD,DPMS) \
	*SMODE2 = \
	((u64)(INT)	<< 0)	| \
	((u64)(FFMD)	<< 1)	| \
	((u64)(DPMS)	<< 2)

//---------------------------------------------------------------------------
// DISPFP1 Register
//---------------------------------------------------------------------------
#define DISPFB1		((vu64 *)(dispfb1))
#define GS_SET_DISPFB1(FBP,FBW,PSM,DBX,DBY) \
	*DISPFB1 = \
	((u64)(FBP)	<< 0)	| \
	((u64)(FBW)	<< 9)	| \
	((u64)(PSM)	<< 15)	| \
	((u64)(DBX)	<< 32)	| \
	((u64)(DBY)	<< 43)

//---------------------------------------------------------------------------
// DISPLAY1 Register
//---------------------------------------------------------------------------
#define DISPLAY1	((vu64 *)(display1))
#define GS_SET_DISPLAY1(DX,DY,MAGH,MAGV,DW,DH) \
	*DISPLAY1 = \
	((u64)(DX)	<< 0)	| \
	((u64)(DY)	<< 12)	| \
	((u64)(MAGH)	<< 23)	| \
	((u64)(MAGV)	<< 27)	| \
	((u64)(DW)	<< 32)	| \
	((u64)(DH)	<< 44)

//---------------------------------------------------------------------------
// DISPFP2 Register
//---------------------------------------------------------------------------
#define DISPFB2		((vu64 *)(dispfb2))
#define GS_SET_DISPFB2(FBP,FBW,PSM,DBX,DBY) \
	*DISPFB2 = \
	((u64)(FBP)	<< 0)	| \
	((u64)(FBW)	<< 9)	| \
	((u64)(PSM)	<< 15)	| \
	((u64)(DBX)	<< 32)	| \
	((u64)(DBY)	<< 43)

//---------------------------------------------------------------------------
// DISPLAY2 Register
//---------------------------------------------------------------------------
#define DISPLAY2	((vu64 *)(display2))
#define GS_SET_DISPLAY2(DX,DY,MAGH,MAGV,DW,DH) \
	*DISPLAY2 = \
	((u64)(DX)	<< 0)	| \
	((u64)(DY)	<< 12)	| \
	((u64)(MAGH)	<< 23)	| \
	((u64)(MAGV)	<< 27)	| \
	((u64)(DW)	<< 32)	| \
	((u64)(DH)	<< 44)

//---------------------------------------------------------------------------
// BGCOLOR Register
//---------------------------------------------------------------------------
#define BGCOLOR		((vu64 *)(bgcolor))
#define GS_SET_BGCOLOR(R,G,B) \
	*BGCOLOR = \
	((u64)(R)	<< 0)		| \
	((u64)(G)	<< 8)		| \
	((u64)(B)	<< 16)

//---------------------------------------------------------------------------
// FRAME_x Register
//---------------------------------------------------------------------------
#define GS_FRAME(FBP,FBW,PSM,FBMSK) \
	(((u64)(FBP)		<< 0)		| \
	 ((u64)(FBW)		<< 16)		| \
	 ((u64)(PSM)		<< 24)		| \
	 ((u64)(FBMSK)	<< 32))

//---------------------------------------------------------------------------
// PRIM Register
//---------------------------------------------------------------------------
#define PRIM_POINT			0
#define PRIM_LINE			1
#define PRIM_LINE_STRIP		2
#define PRIM_TRI			3
#define PRIM_TRI_STRIP		4
#define PRIM_TRI_FAN		5
#define PRIM_SPRITE			6

#define GS_PRIM(PRI,IIP,TME,FGE,ABE,AA1,FST,CTXT,FIX) \
	(((u64)(PRI)		<< 0)		| \
	 ((u64)(IIP)		<< 3)		| \
	 ((u64)(TME)		<< 4)		| \
	 ((u64)(FGE)		<< 5)		| \
	 ((u64)(ABE)		<< 6)		| \
	 ((u64)(AA1)		<< 7)		| \
	 ((u64)(FST)		<< 8)		| \
	 ((u64)(CTXT)	<< 9)		| \
	 ((u64)(FIX)		<< 10))

//---------------------------------------------------------------------------
// RGBAQ Register
//---------------------------------------------------------------------------
#define GS_RGBAQ(R,G,B,A,Q) \
	(((u64)(R)		<< 0)		| \
	 ((u64)(G)		<< 8)		| \
	 ((u64)(B)		<< 16)		| \
	 ((u64)(A)		<< 24)		| \
	 ((u64)(Q)		<< 32))

//---------------------------------------------------------------------------
// SCISSOR_x Register
//---------------------------------------------------------------------------
#define GS_SCISSOR(X0,X1,Y0,Y1) \
	(((u64)(X0)		<< 0)		| \
	 ((u64)(X1)		<< 16)		| \
	 ((u64)(Y0)		<< 32)		| \
	 ((u64)(Y1)		<< 48))

//---------------------------------------------------------------------------
// XYZ2 Register
//---------------------------------------------------------------------------
#define GS_XYZ2(X,Y,Z)	\
	(((u64)(X)		<< 0)		| \
	 ((u64)(Y)		<< 16)		| \
	 ((u64)(Z)		<< 32))

//---------------------------------------------------------------------------
// XYOFFSET_x Register
//---------------------------------------------------------------------------
#define GS_XYOFFSET(OFX,OFY)	\
	(((u64)(OFX)		<< 0)		| \
	 ((u64)(OFY)		<< 32))

//---------------------------------------------------------------------------
// ZBUF_x Register
//---------------------------------------------------------------------------
#define GS_ZBUF(ZBP,PSM,ZMSK)	\
	(((u64)(ZBP)		<< 0)		| \
	 ((u64)(PSM)		<< 24)		| \
	 ((u64)(ZMSK)	<< 32))

#endif // GS_H

