//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_


//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void PrintDebugProc(int no, const char *fmt,...);
#endif