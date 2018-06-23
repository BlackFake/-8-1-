// --------------------------------------------------
//　DrawSample Ver0.50
//	WinSystem.h				Programing By N.Sonoda
// --------------------------------------------------
#ifndef __WINSYSTEM_H__
#define __WINSYSTEM_H__
// -------------------- Define宣言 ------------------
#define Moji_Width					16							// 基本文字の幅(高速描画用)
#define Moji_Height					24							// 基本文字の高さ(高速描画用)
#define Moji_Count					95							// 基本文字の作成数(高速描画用)
#define DELETEOBJECT(x)	if(x) {DeleteObject(x);x=NULL;}			// オブジェクト開放マクロ
// -------------------- IncludeFile -----------------
#include "Global.h"
#include "resource.h"
// --------------- グローバル変数宣言 ---------------
volatile HANDLE g_hMainEvent;									// イベント用ハンドル(メイン用)
HANDLE g_hMainThread;											// スレッド用ハンドル(メイン用)
volatile HANDLE g_hMaskEvent;									// イベント用ハンドル(マスクイメージ作成用)
HANDLE g_hMaskThread;											// スレッド用ハンドル(マスクイメージ作成用)
HANDLE hMutex = NULL;											// ミューテックス用ハンドル
DWORD g_Time = 0;												// 再描画までの時間
LONG g_Main_Time = 1;											// メインループの時間が、設定時間を満たしているかの有無
LONG g_MainLoop = 1;											// WM_DESTROY実行後(終了処理)、メインループを実行しないためのフラグ
LONG g_UpdateWindow = 0;										// WM_PAINTの呼び出しの確認フラグ
HINSTANCE g_hInst;												// インスタンス保存用
BOOL g_MainCreate_Flag = FALSE;									// メインウインドウの初期設定有無
// メインウィンドウ関連
LPBITMAPINFO g_pMemoryImageInfo_MainWindow;						// ビットマップ情報が格納されている先頭アドレス
LPBYTE g_pMemoryImage_MainWindow;								// ビットマップイメージデータが格納されている先頭アドレス(メインウインドウ)
LPBYTE g_pMemoryImageBuffer_MainWindow = NULL;					// 上記２つのデータを格納(メインウインドウ)
BOOL g_bBitMapLoadSuccess_Flag;									// ビットマップの読み込み成功/失敗
typedef struct tgaRGB
{
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
}COLOR_RGB;
HBITMAP g_DIBSection = NULL;		// DIBSectionを使って描画を行うビットマップ領域
typedef struct _tagBMP_
{
	LPBITMAPINFO	pBmpInfo;		// ビットマップ情報が格納されている先頭アドレス
	LPBYTE			pImage;			// ビットマップイメージデータが格納されている先頭アドレス
	LPBYTE			pBuffer;		// 上記２つのデータを格納
	LPBITMAPINFO	pBmpInfo_Mask;	// ビットマップ情報が格納されている先頭アドレス(マスクパターン)
	LPBYTE			pImage_Mask;	// ビットマップイメージデータが格納されている先頭アドレス(マスクパターン)
	LPBYTE			pBuffer_Mask;	// 上記２つのデータを格納(マスクパターン)
}Bmp;
Bmp g_StandardFont;												// 標準フォント
Bmp* g_pBmp = NULL;												// ビットマップ情報保存用
// フォント関連
HFONT g_hFont = NULL;											// フォントスタイル保存用
HWND g_hWnd = NULL;
// ----------------- ユーザー変更OK -----------------
#define Resource_Count				1							// 読み込むリソースの数
int g_iFontHeight = 16;											// フォントの高さ
int g_iFontWidth = 12;											// 平均文字幅
static char* g_szResourceID[] = 								// リソースの名称を登録
{
	"IDB_BITMAP1"
};
// -------------------- 関数宣言 --------------------
BOOL MainRegisterClass(HINSTANCE);								// メインウインドウクラスを登録
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// ウインドウのプロシージャ
BOOL Wm_DestroyProc(HWND);										// 終了処理
void TimerThread(HWND);											// メインループの時間を一定に保つ
BOOL LoadBitMap(HWND, UINT);										// ビットマップの読み込み
void DrawInit(HWND);											// 画像処理関連の初期化化
void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short);		// ビットマップ関連の描画
void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short, float);	// ビットマップ関連の描画
void CreateMaskThread();										// 重ね合わせ用イメージ画像作成
void ClearScreen(HWND);											// 画面を黒色で塗りつぶす
void CreateFont(HWND);											// フォントの作成
void DrawText(HWND, int, int, const char*);						// 文字列の描画(基本文字)
void DrawText(HWND, int, int, const char*, COLORREF);				// 文字列の描画
void DrawPoint(HWND, int, int, COLORREF);							// 点の描画
void DrawLine(HWND, int, int, int, int, COLORREF);					// 線の描画
void CreateDIB(HWND);
// --------------------------------------------------
#endif	/* __WINSYSTEM_H__ */