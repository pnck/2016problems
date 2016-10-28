// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

typedef enum _UNWIND_OP_CODES {
	UWOP_PUSH_NONVOL = 0, /* info == register number */
	UWOP_ALLOC_LARGE,     /* no info, alloc size in next 2 slots */
	UWOP_ALLOC_SMALL,     /* info == size of allocation / 8 - 1 */
	UWOP_SET_FPREG,       /* no info, FP = RSP + UNWIND_INFO.FPRegOffset*16 */
	UWOP_SAVE_NONVOL,     /* info == register number, offset in next slot */
	UWOP_SAVE_NONVOL_FAR, /* info == register number, offset in next 2 slots */
	UWOP_SAVE_XMM128,     /* info == XMM reg number, offset in next slot */
	UWOP_SAVE_XMM128_FAR, /* info == XMM reg number, offset in next 2 slots */
	UWOP_PUSH_MACHFRAME   /* info == 0: no error-code, 1: error-code */
} UNWIND_CODE_OPS;

typedef union _UNWIND_CODE {
	struct {
		BYTE CodeOffset;
		BYTE UnwindOp : 4;
		BYTE OpInfo : 4;
	};
	USHORT FrameOffset;
} UNWIND_CODE, *PUNWIND_CODE;


typedef struct _UNWIND_INFO {
	BYTE Version : 3;
	BYTE Flags : 5;
	BYTE SizeOfProlog;
	BYTE CountOfCodes;
	BYTE FrameRegister : 4;
	BYTE FrameOffset : 4;
	UNWIND_CODE UnwindCode[1];
	/*  UNWIND_CODE MoreUnwindCode[((CountOfCodes + 1) & ~1) - 1];
	*   union {
	*       OPTIONAL ULONG ExceptionHandler;
	*       OPTIONAL ULONG FunctionEntry;
	*   };
	*   OPTIONAL ULONG ExceptionData[]; */
} UNWIND_INFO, *PUNWIND_INFO;


// TODO:  在此处引用程序需要的其他头文件
