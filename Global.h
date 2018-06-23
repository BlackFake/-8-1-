// --------------------------------------------------
//�@DrawSample Ver0.50
//	Global.h			Programing By N.Sonoda
// --------------------------------------------------
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
// -------------------- Define�錾 ------------------
#define Window_Width				640							// �E�C���h�E��
#define Window_Height				480							// �E�C���h�E����
// -------------------- IncludeFile -----------------
#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// -------------------- LibraryFile -----------------
#pragma comment(lib, "winmm.lib")
// --------------- �O���[�o���ϐ��錾 ---------------
extern DWORD g_dwTime;											// �ĕ`��܂ł̎���
extern HWND g_hWnd;
// -------------------- �\���̐錾 ------------------
// -------------------- �֐��錾 --------------------
// ---------- WinSystem ----------
extern void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short);// �r�b�g�}�b�v�֘A�̕`��
extern void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short, float);// �r�b�g�}�b�v�֘A�̕`��
extern void ClearScreen(HWND);									// ��ʂ����F�œh��Ԃ�
extern void DrawText(HWND, int, int, const char*);				// ������̕`��(��{����)
extern void DrawText(HWND, int, int, const char*, COLORREF);	// ������̕`��
extern void DrawPoint(HWND, int, int, COLORREF);				// �_�̕`��
extern void DrawLine(HWND, int, int, int, int, COLORREF);		// ���̕`��
// ---------- Main ----------
extern void MainLoop(HWND);										// ���C���v���O����
// --------------------------------------------------
#endif	/* __GLOBAL_H__ */