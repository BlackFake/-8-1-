// --------------------------------------------------
//　DrawSample Ver0.50
//	Global.h			Programing By N.Sonoda
// --------------------------------------------------
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
// -------------------- Define宣言 ------------------
#define Window_Width				640							// ウインドウ幅
#define Window_Height				480							// ウインドウ高さ
// -------------------- IncludeFile -----------------
#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// -------------------- LibraryFile -----------------
#pragma comment(lib, "winmm.lib")
// --------------- グローバル変数宣言 ---------------
extern DWORD g_dwTime;											// 再描画までの時間
extern HWND g_hWnd;
// -------------------- 構造体宣言 ------------------
// -------------------- 関数宣言 --------------------
// ---------- WinSystem ----------
extern void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short);// ビットマップ関連の描画
extern void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short, float);// ビットマップ関連の描画
extern void ClearScreen(HWND);									// 画面を黒色で塗りつぶす
extern void DrawText(HWND, int, int, const char*);				// 文字列の描画(基本文字)
extern void DrawText(HWND, int, int, const char*, COLORREF);	// 文字列の描画
extern void DrawPoint(HWND, int, int, COLORREF);				// 点の描画
extern void DrawLine(HWND, int, int, int, int, COLORREF);		// 線の描画
// ---------- Main ----------
extern void MainLoop(HWND);										// メインプログラム
// --------------------------------------------------
#endif	/* __GLOBAL_H__ */