// --------------------------------------------------
//　DrawSample Ver0.50
//	WinSystem.cpp			Programing By N.Sonoda
// --------------------------------------------------
// -------------------- IncludeFile ----------------
#include "WinSystem.h"
// ------------------------------------------------------------------------
//　関数名	:MainRegisterClass	メインウィンドウクラスを登録
//	引数	:hInstance			インスタンスハンドル(Windows環境で実行されているプログラムを一意に識別する)	
//	戻り値	:					失敗:0
// ------------------------------------------------------------------------
BOOL MainRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;					// ウィンドウクラスの情報格納用構造体変数

	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);					// 構造体のサイズを設定
	wcex.style = CS_HREDRAW | CS_VREDRAW;				// ウィンドウスタイル
	wcex.lpfnWndProc = (WNDPROC)WndProc;				// ウィンドウプロシージャの指定
	wcex.cbClsExtra = 0;								// 追加領域は使わない
	wcex.cbWndExtra = 0;								// 追加領域は使わない
	wcex.hInstance = hInstance;							// このインスタンスのハンドル
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);	// ラージアイコン
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// カーソルスタイル
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// 背景色 黒色
	wcex.lpszMenuName = NULL;									// 表示メニューのセレクト(メニューなし)
	wcex.lpszClassName = "MainWindowClass";					// ウィンドウクラス名
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);// スモールアイコン

	return RegisterClassEx(&wcex);		// ウインドウクラス登録
}
// ------------------------------------------------------------------------
//　関数名	:WinMain		アプリケーションエントリーポイント	
//	引数	:hInstance		インスタンスハンドル(Windows環境で実行されているプログラムを一意に識別する)	
//			:hPrevInstance	Windows95以降はNULL
//			:pCmdLine		プログラムに渡されたコマンドライン引数が格納された文字列のポインタ
//			:CmdShow		ウィンドウズの初期化方法
//	戻り値	:				Windowsへの終了コード		
// ------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		pCmdLine,
	int			CmdShow)
{
	HWND hWnd;				// メインウインドウのハンドル
	HWND hDeskWnd;			// ディスクトップのウインドウハンドル
	MSG	msg;				// メッセージキューから取得したメッセージ情報格納用構造体変数
	HMENU hMenu;			// メニューのハンドル
	RECT deskrc, rc;
	int Window_xPos, Window_yPos;
	BOOL bError_Flag = FALSE;
	HDC hDC;
	const char *pszMutexObjectName = "DrawSample";

	// ミューテックスオブジェクトの作成
	hMutex = CreateMutex(NULL,					// セキュリティ属性構造体へのポインタ
		FALSE,					// 作成したミューテックスを最初から所有するかの有無 
		pszMutexObjectName);	// ミューテックスオブジェクトの名前
	// 既にアプリケーションが実行されているかの確認
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "既に作動中なので、実行できません。", "確認", MB_OK);
		return FALSE;
	}

	g_hInst = hInstance;				// インスタンスハンドルの保存
	if (!MainRegisterClass(hInstance))	// メインウィンドウクラスの登録
		return FALSE;

	// ウィンドウの作成
	hWnd = CreateWindow("MainWindowClass",		// クラス名
		"DrawSample",			// タイトル
		WS_MINIMIZEBOX			// スタイル
		| WS_OVERLAPPED
		| WS_SYSMENU,
		CW_USEDEFAULT,			// ｘ座標(Windowsに任せる)
		CW_USEDEFAULT,			// ｙ座標(Windowsに任せる)ｘｙで左上角の座標
		Window_Width,			// 幅
		Window_Height,			// 高さ
		HWND_DESKTOP,			// 無し
		(HMENU)NULL,			// メニュー無し
		hInstance,				// このプログラムのインスタンスのハンドル
		NULL);					// 追加引数無し
	g_hWnd = hWnd;

	GetClientRect(hWnd, &rc);
	MoveWindow(hWnd, CW_USEDEFAULT, CW_USEDEFAULT, Window_Width + Window_Width - rc.right, Window_Height + Window_Height - rc.bottom, TRUE);

	hMenu = GetSystemMenu(hWnd, FALSE);			// 初期設定メニュー削除
	DeleteMenu(hMenu, 0, MF_BYPOSITION);
	DeleteMenu(hMenu, 1, MF_BYPOSITION);
	DeleteMenu(hMenu, 2, MF_BYPOSITION);
	DrawMenuBar(hWnd);

	// ウィンドウをディスクトップ上の中央に表示するため、座標の取得
	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
	GetWindowRect(hWnd, (LPRECT)&rc);
	Window_xPos = (deskrc.right - (rc.right - rc.left)) / 2;
	Window_yPos = (deskrc.bottom - (rc.bottom - rc.top)) / 2;

	if (hWnd != NULL)
	{
		// ポジションを移動する
		SetWindowPos(hWnd, HWND_TOP, Window_xPos, Window_yPos, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);
		// ウィンドウを表示
		ShowWindow(hWnd, SW_SHOWNORMAL);
	}
	else return FALSE;

	// メイン メッセージ ループ
	msg.message = WM_CREATE;
	while (msg.message != WM_QUIT){// メッセージループ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);			// キーボードメッセージを文字メッセージに変換してメッセージキューにポスト
			DispatchMessage(&msg);			// ウィンドウプロシージャにメッセージを送る
		}
		if (g_MainLoop && g_MainCreate_Flag)
		{
			if (g_Main_Time)
			{
				if (!IsIconic(hWnd))		// ウィンドウの最小化有無
				{
					// 画像更新フラグOFF
					if (g_UpdateWindow)
						InterlockedExchange(&g_UpdateWindow, 0);
					MainLoop(hWnd);			// ゲームメイン関数の呼び出し
					InterlockedDecrement(&g_Main_Time);
					// 画像の更新処理
					if (g_UpdateWindow)
					{
						// デバイスコンテキストハンドルの取得
						hDC = GetDC(hWnd);
						StretchDIBits(hDC, 0, 0, Window_Width, Window_Height, 0, 0, Window_Width, Window_Height,
							g_pMemoryImage_MainWindow, g_pMemoryImageInfo_MainWindow, DIB_RGB_COLORS, SRCCOPY);
						// デバイスコンテキストハンドルの開放
						ReleaseDC(hWnd, hDC);
					}
				}
			}
		}
		Sleep(1);
	}

	ReleaseMutex(hMutex);				// ミューテックスオブジェクト所有権の解放
	CloseHandle(hMutex);				// ミューテックスオブジェクト用ハンドルの開放

	return (int)msg.wParam;
}
// ------------------------------------------------------------------------
//　関数名	:WndProc		ウィンドウのプロシージャ	
//	引数	:hWnd			ウィンドウのハンドル
//			:Message		メッセージの種類
//			:wParam			メッセージの補助情報
//			:lParam			メッセージの補助情報
//	戻り値	:				メッセージを戻す		
// ------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	static DWORD threadID;							// スレッド用ID

	switch (uMessage)
	{
		case WM_CREATE:
		g_pBmp = new Bmp[Resource_Count];
		// ビットマップの読み込み
		for (int i = 0; i < Resource_Count; i++)
		{
			g_bBitMapLoadSuccess_Flag = LoadBitMap(hWnd, i);
			if (!g_bBitMapLoadSuccess_Flag) break;
		}
		if (!g_bBitMapLoadSuccess_Flag)
		{
			MessageBox(hWnd, "ビットマップファイルから読み込めませんでした。", "確認", MB_OK);
			PostQuitMessage(0);
			break;
		}
		// 画像処理関連の初期化
		DrawInit(hWnd);
		if (!g_hMainThread)		//スレッドの作成
		{
			g_hMainThread = CreateThread(
				0,											// セキュリティ属性(なし)
				0,											// レッドが持つスタックのサイズを、バイト単位で指定(デフォルト)
				(LPTHREAD_START_ROUTINE)TimerThread,		// スレッドの開始アドレスを指定
				(VOID *)hWnd,								// スレッドへの引数指定(アドレス)
				0,											// 制御フラグを指定(設定なし)
				&threadID);									// スレッドIDが格納されているポインタ指定 
		}
		// イベントオブジェクトを作成
		g_hMainEvent = CreateEvent(
			0,		// セキュリティ属性(設定なし)
			TRUE,	// 手動/自動リセットオブジェクト作成
			TRUE,	// イベントオブジェクトの初期状態指定(シグナル状態)
			0);		// イベントオブジェクトの名前
		SetEvent(g_hMainEvent);				// オブジェクトをシグナル状態に設定
		if (!g_hMaskThread){	//スレッドの作成
			g_hMaskThread = CreateThread(
				0,											// セキュリティ属性(なし)
				0,											// レッドが持つスタックのサイズを、バイト単位で指定(デフォルト)
				(LPTHREAD_START_ROUTINE)CreateMaskThread,	// スレッドの開始アドレスを指定
				(VOID *)NULL,								// スレッドへの引数指定(アドレス)
				0,											// 制御フラグを指定(設定なし)
				&threadID);									// スレッドIDが格納されているポインタ指定 
		}
		// イベントオブジェクトを作成
		g_hMaskEvent = CreateEvent(
			0,		// セキュリティ属性(設定なし)
			TRUE,	// 手動/自動リセットオブジェクト作成
			TRUE,	// イベントオブジェクトの初期状態指定(シグナル状態)
			0);		// イベントオブジェクトの名前
		SetEvent(g_hMaskEvent);				// オブジェクトをシグナル状態に設定
		CreateFont(hWnd);					// フォントの作成
		break;
		// ウインドウの破棄処理	
	case WM_CLOSE:
		Wm_DestroyProc(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}
// ------------------------------------------------------------------------
//　関数名	:Wm_DestroyProc		終了処理	
//	引数	:hWnd				ウィンドウのハンドル
//	戻り値	:					成功:1、失敗:0
// ------------------------------------------------------------------------
BOOL Wm_DestroyProc(HWND hWnd)
{
	if (g_hMainThread)
	{
		ResetEvent(g_hMainEvent);	// イベントオブジェクトを非シグナル状態に設定
		// 指定したｵﾌﾞｼﾞｪｸﾄがｼｸﾞﾅﾙ状態になるか、または、ﾀｲﾑｱｳﾄが発生するかまで待つ
		while (WaitForSingleObject(g_hMainThread, 0) == WAIT_TIMEOUT)
			Sleep(1);
		g_hMainThread = NULL;
	}

	// 画像関連データの開放
	if (g_pMemoryImageBuffer_MainWindow != NULL)
	{
		delete[] g_pMemoryImageBuffer_MainWindow;
		g_pMemoryImageBuffer_MainWindow = NULL;
	}
	if (g_DIBSection != NULL)
	{
		DeleteObject(g_DIBSection);
		g_DIBSection = NULL;
	}
	DELETEOBJECT(g_hFont);
	if (g_StandardFont.pBuffer != NULL)
	{
		delete[] g_StandardFont.pBuffer;
		delete[] g_StandardFont.pBuffer_Mask;
	}
	if (g_pBmp)
	{
		for (int i = 0; i < Resource_Count; i++)
		{
			delete[] g_pBmp[i].pBuffer;
			if (g_pBmp[i].pBuffer_Mask)
				delete[] g_pBmp[i].pBuffer_Mask;
		}
		delete[] g_pBmp;
	}
	g_MainLoop = 0;
	PostQuitMessage(0);

	return TRUE;
}
// ------------------------------------------------------------------------
//　関数名	:TimerThread		メインループの時間を一定に保つ	
//	引数	:hWnd				ウィンドウのハンドル
//	戻り値	:					なし
// ------------------------------------------------------------------------
void TimerThread(HWND hWnd)
{
	static DWORD OldTime, NowTime;
	static DWORD Timer = 30;

	while (TRUE)
	{
		if (WaitForSingleObject(g_hMainEvent, 0) == WAIT_TIMEOUT){
			break;
		}
		// Timeを入手する
		if (g_Main_Time == 0)
		{
			NowTime = timeGetTime();
			if (NowTime - OldTime >= Timer)
			{
				InterlockedIncrement(&g_Main_Time);
				OldTime = NowTime;
			}
		}
		Sleep(1);
	}
}
// ------------------------------------------------------------------------
//　関数名	:LoadBitMap			ビットマップの読み込み
//	引数	:hWnd				ウィンドウのハンドル
//			:BmpNum				ロードするビットマップ番号		
//	戻り値	:					成功:1、失敗:0
// ------------------------------------------------------------------------
BOOL LoadBitMap(HWND hWnd, UINT BmpNum)
{
	LPBITMAPINFO pBitMap;
	HANDLE hResource;
	HRSRC hRsrc;

	// リソースのハンドル取得
	hRsrc = FindResource(g_hInst, g_szResourceID[BmpNum], RT_BITMAP);
	if (hRsrc == NULL)
	{
		MessageBox(hWnd, "リソースがありません。", "確認", MB_OK);
		return FALSE;
	}
	// リソースをメモリーにロードする
	hResource = LoadResource(g_hInst, hRsrc);
	if (hRsrc == NULL)
	{
		MessageBox(hWnd, "リソースをロードできません。", "確認", MB_OK);
		return FALSE;
	}

	// メモリ内の指定されたリソースをロック
	pBitMap = (LPBITMAPINFO)LockResource(hResource);

	// 24BitColorで作成
	g_pBmp[BmpNum].pBuffer = new BYTE[sizeof(BITMAPINFO) + pBitMap->bmiHeader.biWidth * pBitMap->bmiHeader.biHeight * sizeof(COLOR_RGB)];

	g_pBmp[BmpNum].pBmpInfo = (LPBITMAPINFO)g_pBmp[BmpNum].pBuffer;
	g_pBmp[BmpNum].pImage = (LPBYTE)(g_pBmp[BmpNum].pBuffer + sizeof(BITMAPINFOHEADER));
	LPBYTE pBitMapImageBackup = g_pBmp[BmpNum].pImage;
	// ヘッダー情報のコピー
	CopyMemory(g_pBmp[BmpNum].pBmpInfo, (char *)pBitMap, sizeof(BITMAPINFOHEADER));

	LPBYTE pPos;		// 読み込むポジションを保存

	if (pBitMap->bmiHeader.biBitCount == 16)		// 2,16,256色,24BitColor以外の場合
	{
		MessageBox(hWnd, "リソースを16BitColor以外で、登録してください。", "確認", MB_OK);
		return FALSE;
	}
	// 色数に応じてデータをコピー
	if (pBitMap->bmiHeader.biBitCount != 24)
	{
		// 論理パレットを展開
		// 読み込むポジションを決定
		pPos = (LPBYTE)pBitMap + sizeof(BITMAPINFOHEADER) + ((1 << g_pBmp[BmpNum].pBmpInfo->bmiHeader.biBitCount) * sizeof(RGBQUAD));
		// ビットマップデータのコピー
		if (pBitMap->bmiHeader.biBitCount == 1)		// 2色
		{
			for (LONG i = 0; i < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight; i++)
			{
				short BitPosition = 7;		// ビット列の場所を指定(7:最上位ビット)
				LPBYTE lpPosBackup = pPos;
				for (LONG j = 0; j < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth; j++)
				{
					BYTE Data = *pPos;
					// ビット列のシフト
					Data = Data << (7 - BitPosition);
					Data = Data >> 7;
					BitPosition--;
					if (BitPosition < 0)
					{
						BitPosition = 7;
						pPos++;
					}
					*(pBitMapImageBackup++) = pBitMap->bmiColors[Data].rgbBlue;
					*(pBitMapImageBackup++) = pBitMap->bmiColors[Data].rgbGreen;
					*(pBitMapImageBackup++) = pBitMap->bmiColors[Data].rgbRed;
				}
				pPos = lpPosBackup + (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth / 32) * sizeof(DWORD) + sizeof(DWORD);
			}
		}
		if (pBitMap->bmiHeader.biBitCount == 4)		// 16色
		{
			BOOL bHighOrLow = 1;//(0:Low,1:High)
			BYTE DataLow = *pPos;
			BYTE DataHigh = *pPos;
			BYTE Data;
			for (LONG i = 0; i < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight; i++)
			{
				for (LONG j = 0; j < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth; j++)
				{
					DataLow = *pPos;
					DataHigh = *pPos;
					// ビット列をHigtとLowに分ける
					DataHigh = (DataHigh >> 4);
					DataLow &= 0x0F;
					if (bHighOrLow)
					{
						Data = DataHigh;
						bHighOrLow = 0;
					}
					else
					{
						Data = DataLow;
						pPos++;
						bHighOrLow = 1;
					}
					*(pBitMapImageBackup++) = pBitMap->bmiColors[Data].rgbBlue;
					*(pBitMapImageBackup++) = pBitMap->bmiColors[Data].rgbGreen;
					*(pBitMapImageBackup++) = pBitMap->bmiColors[Data].rgbRed;
				}
				if (((g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 8) % 2) == 1)
				{
					pPos += (3 - (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 8) / 2);
					if (bHighOrLow)
						bHighOrLow = 0;
					else
					{
						pPos++;
						bHighOrLow = 1;
					}
				}
				else
				{
					if ((g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 8) != 0)
						pPos += (4 - (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 8) / 2);
				}
			}
		}
		if (pBitMap->bmiHeader.biBitCount == 8)		// 256色
		{
			for (LONG i = 0; i < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight; i++)
			{
				for (LONG j = 0; j < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth; j++)
				{
					CopyMemory(pBitMapImageBackup, &(pBitMap->bmiColors[*pPos++].rgbBlue), sizeof(COLOR_RGB));
					pBitMapImageBackup += sizeof(COLOR_RGB);
				}
				if (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 4)
					pPos += 4 - g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 4;
			}
		}
		// 色数を24BitColorに変更
		g_pBmp[BmpNum].pBmpInfo->bmiHeader.biBitCount = 24;
	}
	else
	{
		// 24BitColorなのでビットマップデータをそのまま展開
		// ビットマップデータのコピー
		// 読み込むポジションを決定
		pPos = (LPBYTE)pBitMap + sizeof(BITMAPINFOHEADER);
		for (LONG i = 0; i < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight; i++)
		{
			CopyMemory(pBitMapImageBackup, pPos, sizeof(COLOR_RGB) * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth);
			pPos += sizeof(COLOR_RGB) * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth + g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 4;
			pBitMapImageBackup += sizeof(COLOR_RGB) * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth;
		}

	}
	// マスク
	// 24BitColorで作成
	g_pBmp[BmpNum].pBuffer_Mask = new BYTE[sizeof(BITMAPINFO) + g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight * sizeof(COLOR_RGB)];

	g_pBmp[BmpNum].pBmpInfo_Mask = (LPBITMAPINFO)g_pBmp[BmpNum].pBuffer_Mask;
	g_pBmp[BmpNum].pImage_Mask = (LPBYTE)(g_pBmp[BmpNum].pBuffer_Mask + sizeof(BITMAPINFOHEADER));
	// ヘッダー情報のコピー
	CopyMemory(g_pBmp[BmpNum].pBmpInfo_Mask, (char *)g_pBmp[BmpNum].pBmpInfo, sizeof(BITMAPINFOHEADER));
	// イメージ情報を白(0XFF)で埋める
	FillMemory(g_pBmp[BmpNum].pImage_Mask, g_pBmp[BmpNum].pBmpInfo_Mask->bmiHeader.biWidth * g_pBmp[BmpNum].pBmpInfo_Mask->bmiHeader.biHeight * sizeof(COLOR_RGB), 0xFF);

	return TRUE;
}
// ------------------------------------------------------------------------
//　関数名	:DrawInit			画像処理関連の初期化	
//	引数	:hWnd				ウィンドウのハンドル
//	戻り値	:					なし
// ------------------------------------------------------------------------
void DrawInit(HWND hWnd)
{
	HDC hDC;									// ＤＣの割付変数
	// メインウインドウ
	if (g_pMemoryImageBuffer_MainWindow != NULL)
	{
		delete[] g_pMemoryImageBuffer_MainWindow;
		g_pMemoryImageBuffer_MainWindow = NULL;
	}
	// 24BitColorで作成
	g_pMemoryImageBuffer_MainWindow = new BYTE[sizeof(BITMAPINFO) + Window_Width * Window_Height * sizeof(COLOR_RGB)];

	g_pMemoryImageInfo_MainWindow = (LPBITMAPINFO)g_pMemoryImageBuffer_MainWindow;
	g_pMemoryImage_MainWindow = (BYTE*)(g_pMemoryImageBuffer_MainWindow + sizeof(BITMAPINFOHEADER));

	g_pMemoryImageInfo_MainWindow->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	// 構造体のサイズ
	g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth = Window_Width;				// 幅
	g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight = Window_Height;			// 高さ
	g_pMemoryImageInfo_MainWindow->bmiHeader.biPlanes = 1;						// プレーン数
	g_pMemoryImageInfo_MainWindow->bmiHeader.biBitCount = 24;					// 色数[1,4,8,24](24BitColor)
	g_pMemoryImageInfo_MainWindow->bmiHeader.biCompression = 0;					// 圧縮方式(なし)
	g_pMemoryImageInfo_MainWindow->bmiHeader.biSizeImage = 0;					// ビットマップサイズ(なし)
	g_pMemoryImageInfo_MainWindow->bmiHeader.biXPelsPerMeter = 0;				// 水平解像度
	g_pMemoryImageInfo_MainWindow->bmiHeader.biYPelsPerMeter = 0;				// 垂直解像度
	g_pMemoryImageInfo_MainWindow->bmiHeader.biClrUsed = 0;						// 色数
	g_pMemoryImageInfo_MainWindow->bmiHeader.biClrImportant = 0;					// 重要な色数
	hDC = GetDC(hWnd);
	// 文字描画領域DIBセクションの作成
	g_DIBSection = CreateDIBSection(hDC,								// デバイスコンテキストハンドル
		g_pMemoryImageInfo_MainWindow,	// 大元画面のBITMAPINFO構造体情報
		DIB_RGB_COLORS,					// カラー指定
		(void**)&g_pMemoryImage_MainWindow,	// DIBのビットマップバッファのアドレスを入れるポインタ(この変数は開放してはいけない！)
		NULL,							// ファイルマッピングを使う時に使用。通常はNULL
		0);								// ファイルマッピングを使う時に使用。通常は0
	ReleaseDC(hWnd, hDC);
}
// ------------------------------------------------------------------------
//　関数名	:DrawBitMap			ビットマップ関連の描画
//	引数	:hWnd				ウィンドウのハンドル
//　　　　　:X_Position			描画するX座標					
//　　　　　:Y_Position			描画するY座標
//　　　　　:X_DataPosition		データがあるX座標
//　　　　　:Y_DataPosition		データがあるY座標
//　　　　　:Unit_Width			幅
//　　　　　:Unit_Height		高さ
//			:BmpNum				ビットマップ番号
//			:OperationCode		描画方法
//	戻り値	:					なし
// ------------------------------------------------------------------------
void DrawBitMap(HWND hWnd, int X_Position, int Y_Position, int X_DataPosition, int Y_DataPosition
	, int Unit_Width, int Unit_Height, UINT BmpNum, short OperationCode)
{
	LONG ReceivePosition, SendPosition;

	// 情報に誤りがある
	if (BmpNum < 0 || Resource_Count <= BmpNum)
		return;
	// ビットマップの表示する大きさは、ビットマップデータ内か確認する
	if (!(X_DataPosition + Unit_Width <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth
		&& Y_DataPosition + Unit_Height <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight))
		return;

	// ビットマップの格納は、Ｙ座標は上下逆なので、修正する
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - Unit_Height;
	Y_DataPosition = g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight - Unit_Height - Y_DataPosition;

	// メモリの範囲内かの有無
	if (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth > X_Position && 0 < X_Position + Unit_Width
		&& g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight > Y_Position && 0 < Y_Position + Unit_Height)
	{
		if (X_Position + Unit_Width > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth)
		{
			// 右にある
			// 幅を求める
			Unit_Width = g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth - X_Position;
		}
		else if (X_Position <= 0)			// 左にある
		{
			// 幅を求める
			Unit_Width = Unit_Width + X_Position;
			// データ位置を求める
			X_DataPosition = X_DataPosition + abs(X_Position);
			X_Position = 0;
		}
		if (g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight <= Y_Position + Unit_Height)
		{
			// 下にある
			// 高さを求める
			Unit_Height = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position;
		}
		else if (Y_Position <= 0)
		{
			// 高さを求める
			Unit_Height = Y_Position + Unit_Height;
			// データ位置を求める
			Y_DataPosition = Y_DataPosition + abs(Y_Position);
			Y_Position = 0; // 上にある
		}
		// メモリー中の座標計算
		ReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);
		SendPosition = X_DataPosition + Y_DataPosition * (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth);
	}
	else
		return;

	if (!OperationCode)	// そのまま描画
	{
		for (int i = 0; i < Unit_Height; i++)
		{
			CopyMemory(g_pMemoryImage_MainWindow + (ReceivePosition + i * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				g_pBmp[BmpNum].pImage + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				Unit_Width * sizeof(COLOR_RGB));
		}
	}
	else					// 背景と重ね合わせて描画
	{
		int Loop = (Unit_Width * sizeof(COLOR_RGB)) / sizeof(__int64);
		DWORD* pLoop = (DWORD*)&Loop;
		int Counter = (Unit_Width * sizeof(COLOR_RGB)) % sizeof(__int64);
		DWORD* pCounter = (DWORD*)&Counter;

		for (int i = 0; i < Unit_Height; i++)
		{
			LPBYTE pMemoryImage = g_pMemoryImage_MainWindow + (ReceivePosition + i * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth) * sizeof(COLOR_RGB);
			LPBYTE pMaskImage = g_pBmp[BmpNum].pImage_Mask + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo_Mask->bmiHeader.biWidth) * sizeof(COLOR_RGB);
			LPBYTE pOrImage = g_pBmp[BmpNum].pImage + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth) * sizeof(COLOR_RGB);
			DWORD Width = (DWORD)(Unit_Width * sizeof(COLOR_RGB));
			DWORD* pWidth = &Width;
			__asm
			{
					MOV EAX, DWORD PTR pLoop
					XOR EBX, EBX
					MOV EBX, [EAX]

					MOV EAX, DWORD PTR pCounter
					XOR ECX, ECX
					MOV ECX, [EAX]

					MOV	EDX, DWORD PTR pMemoryImage
					MOV	EDI, DWORD PTR pMaskImage
					MOV	ESI, DWORD PTR pOrImage

					CMP	BX, 0H
					JZ	LP2
				LP1 :
					MOVQ MM0, [EDX]
					MOVQ MM1, [EDI]
					PAND MM0, MM1
					MOVQ MM1, [ESI]
					POR MM0, MM1
					MOVQ[EDX], MM0

					ADD EDX, 8H
					ADD EDI, 8H
					ADD ESI, 8H

					DEC	BX
					JNZ	LP1
					CMP	CL, 0H
					JZ	END
				LP2 :
					MOV	AL, [EDX]
					AND AL, [EDI]
					OR	AL, [ESI]
					MOV[EDX], AL
					INC EDX
					INC EDI
					INC ESI
					DEC	CL
					CMP	CL, 0H
					JNZ	LP2
				END : EMMS
			}
		}
	}

	// 画像更新フラグON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//　関数名	:DrawBitMap			ビットマップ関連の描画
//	引数	:hWnd				ウィンドウのハンドル
//　　　　　:X_Position			描画するX座標					
//　　　　　:Y_Position			描画するY座標
//　　　　　:X_DataPosition		データがあるX座標
//　　　　　:Y_DataPosition		データがあるY座標
//　　　　　:Unit_Width			幅
//　　　　　:Unit_Height		高さ
//			:BmpNum				ビットマップ番号
//			:OperationCode		描画方法
//			:Alpha				アルファー値
//	戻り値	:					なし
// ------------------------------------------------------------------------
void DrawBitMap(HWND hWnd, int X_Position, int Y_Position, int X_DataPosition, int Y_DataPosition
	, int Unit_Width, int Unit_Height, UINT BmpNum, short OperationCode, float Alpha)
{
	LONG ReceivePosition, SendPosition;

	// 情報に誤りがある
	if (BmpNum < 0 || Resource_Count <= BmpNum)
		return;
	// ビットマップの表示する大きさは、ビットマップデータ内か確認する
	if (!(X_DataPosition + Unit_Width <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth
		&& Y_DataPosition + Unit_Height <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight))
		return;

	// ビットマップの格納は、Ｙ座標は上下逆なので、修正する
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - Unit_Height;
	Y_DataPosition = g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight - Unit_Height - Y_DataPosition;

	// メモリの範囲内かの有無
	if (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth > X_Position && 0 < X_Position + Unit_Width
		&& g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight > Y_Position && 0 < Y_Position + Unit_Height)
	{
		if (X_Position + Unit_Width > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth)
		{
			// 右にある
			// 幅を求める
			Unit_Width = g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth - X_Position;
		}
		else if (X_Position <= 0)		// 左にある
		{
			// 幅を求める
			Unit_Width = Unit_Width + X_Position;
			// データ位置を求める
			X_DataPosition = X_DataPosition + abs(X_Position);
			X_Position = 0;
		}
		if (g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight <= Y_Position + Unit_Height)
		{
			// 下にある
			// 高さを求める
			Unit_Height = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position;
		}
		else if (Y_Position <= 0)
		{
			// 高さを求める
			Unit_Height = Y_Position + Unit_Height;
			// データ位置を求める
			Y_DataPosition = Y_DataPosition + abs(Y_Position);
			Y_Position = 0; // 上にある
		}
		// メモリー中の座標計算
		ReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);
		SendPosition = X_DataPosition + Y_DataPosition * (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth);
	}
	else
		return;

	if (!OperationCode)	// そのまま描画
	{
		for (int i = 0; i < Unit_Height; i++)
		{
			CopyMemory(g_pMemoryImage_MainWindow + (ReceivePosition + i * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				g_pBmp[BmpNum].pImage + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				Unit_Width * sizeof(COLOR_RGB));
		}
	}
	else					// 背景と重ね合わせて描画
	{
		if (Alpha < 0.0f || Alpha > 1.0f)
			Alpha = 1.0f;
		WORD AlphaValue = (WORD)(0x100 * (Alpha / 1.0f));
		WORD AlphaSubValue = 0x100 - AlphaValue;
		DWORD* pAlphaValue = (DWORD*)&AlphaValue;
		DWORD* pAlphaSubValue = (DWORD*)&AlphaSubValue;

		__int64 pAlphaImage = ((__int64)((AlphaValue << 16) | AlphaValue) << 32) | (AlphaValue << 16) | AlphaValue;
		__int64 pAlphaBG = ((__int64)((AlphaSubValue << 16) | AlphaSubValue) << 32) | (AlphaSubValue << 16) | AlphaSubValue;
		__int64 pReverse = 0xFFFFFFFFFFFFFFFF;
		int Loop = (Unit_Width * sizeof(COLOR_RGB)) / sizeof(__int64);
		DWORD* pLoop = (DWORD*)&Loop;
		int Counter = (Unit_Width * sizeof(COLOR_RGB)) % sizeof(__int64);
		DWORD* pCounter = (DWORD*)&Counter;

		for (int i = 0; i < Unit_Height; i++)
		{
			LPBYTE lpMemoryImage = g_pMemoryImage_MainWindow + (ReceivePosition + i * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth) * sizeof(COLOR_RGB);
			LPBYTE lpMaskImage = g_pBmp[BmpNum].pImage_Mask + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo_Mask->bmiHeader.biWidth) * sizeof(COLOR_RGB);
			LPBYTE lpOrImage = g_pBmp[BmpNum].pImage + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth) * sizeof(COLOR_RGB);
			__asm
			{
					MOV EAX, DWORD PTR pLoop
					XOR EBX, EBX
					MOV EBX, [EAX]

					MOV EAX, DWORD PTR pCounter
					XOR ECX, ECX
					MOV ECX, [EAX]

					MOV	EDX, DWORD PTR lpMemoryImage
					MOV	EDI, DWORD PTR lpMaskImage
					MOV	ESI, DWORD PTR lpOrImage

					CMP	BX, 0H
					JZ	LP2
				LP1 :
					MOVQ MM6, pAlphaBG

					PXOR MM7, MM7
					MOVQ MM0, [EDX]
					MOVQ MM1, MM0

					PUNPCKLBW MM0, MM7
					PUNPCKHBW MM1, MM7
					PMULLW MM0, MM6
					PSRLW MM0, 8H
					PMULLW MM1, MM6
					PSRLW MM1, 8H
					PACKUSWB MM0, MM1

					MOVQ MM1, [EDI]
					PXOR MM1, pReverse
					PAND MM0, MM1

					MOVQ MM1, [EDX]
					MOVQ MM2, [EDI]
					PAND MM1, MM2
					POR MM0, MM1

					MOVQ MM6, pAlphaImage
					MOVQ MM1, [ESI]
					MOVQ MM2, MM1

					PUNPCKLBW MM1, MM7
					PUNPCKHBW MM2, MM7
					PMULLW MM1, MM6
					PSRLW MM1, 8H
					PMULLW MM2, MM6
					PSRLW MM2, 8H
					PACKUSWB MM1, MM2

					PADDB MM0, MM1
					MOVQ[EDX], MM0

					ADD EDX, 8H
					ADD EDI, 8H
					ADD ESI, 8H

					DEC	BX
					JNZ	LP1
					CMP	CL, 0H
					JZ	END
				LP2 :
					XOR EAX, EAX
					MOV	AL, [EDX]
					MOV	EBX, DWORD PTR pAlphaSubValue
					MOV BX, [EBX]
					IMUL AX, BX
					ROR AX, 8H
					MOV	BL, [EDI]
					XOR BL, 0FFH
					AND AL, BL
					MOV CH, AL

					XOR EAX, EAX
					MOV	AL, [EDX]
					MOV	BL, [EDI]
					AND AL, BL
					OR CH, AL

					XOR EAX, EAX
					MOV	AL, [ESI]
					MOV	EBX, DWORD PTR pAlphaValue
					MOV BX, [EBX]
					IMUL AX, BX
					ROR AX, 8H
					ADD AL, CH

					MOV[EDX], AL
					INC EDX
					INC EDI
					INC ESI
					DEC	CL
					CMP	CL, 0H
					JNZ	LP2
				END : EMMS
			}
		}
	}

	// 画像更新フラグON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//　関数名	:CreateMaskThread	重ね合わせ用イメージ画像作成	
//	引数	:					なし
//	戻り値	:					なし
// ------------------------------------------------------------------------
void CreateMaskThread()
{
	for (int i = 0; i < Resource_Count; i++)
	{
		// マスクイメージ作成
		LPBYTE pImage = g_pBmp[i].pImage;
		LPBYTE pMaskImage = g_pBmp[i].pImage_Mask;
		DWORD Width = (DWORD)(g_pBmp[i].pBmpInfo->bmiHeader.biWidth * g_pBmp[i].pBmpInfo->bmiHeader.biHeight);
		DWORD* pWidth = &Width;
		__asm
		{
				MOV ECX, DWORD PTR pWidth
				MOV EBX, [ECX]
				MOV	EDX, DWORD PTR pImage
				MOV	EDI, DWORD PTR pMaskImage
				MOV	CL, 8H
			LP1:
				MOV	EAX, [EDX]
				SHL	EAX, CL
				SHR	EAX, CL
				CMP EAX, 0H
				JZ	MVP
				MOV	EAX, [EDI]
				AND	EAX, 0FF000000H
				MOV[EDI], EAX
			MVP :
				ADD	EDX, 3H
				ADD	EDI, 3H
				DEC	EBX
				JNZ	LP1
		}
	}
	ResetEvent(g_hMaskEvent);	// イベントオブジェクトを非シグナル状態に設定

	g_MainCreate_Flag = TRUE;	// 初期設定終了
	g_hMaskThread = NULL;
}
// ------------------------------------------------------------------------
//　関数名	:ClearScreen		画面を黒色で塗りつぶす
//	引数	:hWnd				ウィンドウのハンドル
//	戻り値	:					なし
// ------------------------------------------------------------------------
void ClearScreen(HWND hWnd)
{
	// メモリー空間を黒(0x00)で埋める
	FillMemory(g_pMemoryImage_MainWindow,
		g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth * sizeof(COLOR_RGB),
		0x00);

	// 画像更新フラグON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//　関数名	:CreateFont			フォントの作成
//	引数	:					なし
//	戻り値	:					なし
// ------------------------------------------------------------------------
void CreateFont(HWND hWnd)
{
	// 変数の宣言
	HDC hDC, hFontDC;						// ＤＣの割付変数
	HGDIOBJ hGDIObj;						// ＤＣ開放用ポインタ
	HBITMAP hFontImage = NULL;				// フォントイメージ保存用

	g_StandardFont.pBuffer = new BYTE[sizeof(BITMAPINFO) + sizeof(COLOR_RGB) * Moji_Width * Moji_Height * Moji_Count];
	g_StandardFont.pBmpInfo = (LPBITMAPINFO)(g_StandardFont.pBuffer + sizeof(BITMAPFILEHEADER));
	g_StandardFont.pImage = g_StandardFont.pBuffer + sizeof(BITMAPINFOHEADER);

	g_StandardFont.pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);// 構造体のサイズ
	g_StandardFont.pBmpInfo->bmiHeader.biWidth = Moji_Width * Moji_Count;// 幅
	g_StandardFont.pBmpInfo->bmiHeader.biHeight = Moji_Height;			// 高さ
	g_StandardFont.pBmpInfo->bmiHeader.biPlanes = 1;						// プレーン数
	g_StandardFont.pBmpInfo->bmiHeader.biBitCount = 24;					// 色数[1,4,8,24](24BitColor)
	g_StandardFont.pBmpInfo->bmiHeader.biCompression = 0;					// 圧縮方式(なし)
	g_StandardFont.pBmpInfo->bmiHeader.biSizeImage = 0;					// ビットマップサイズ(なし)
	g_StandardFont.pBmpInfo->bmiHeader.biXPelsPerMeter = 0;				// 水平解像度
	g_StandardFont.pBmpInfo->bmiHeader.biYPelsPerMeter = 0;				// 垂直解像度
	g_StandardFont.pBmpInfo->bmiHeader.biClrUsed = 0;	 					// 色数
	g_StandardFont.pBmpInfo->bmiHeader.biClrImportant = 0;				// 重要な色数

	// 論理フォントの作成
	g_hFont = CreateFont(Moji_Height,		// フォントの高さ
		Moji_Width,			// 平均文字幅
		0,					// 文字送り方向の角度
		0,					// ベースラインの角度
		FW_NORMAL,			// フォントの太さ
		FALSE,				// 斜体にするかどうかの有無
		FALSE,				// 下線を付けるかどうかの有無
		FALSE,				// 取り消し線を付けるかどうかの有無
		SHIFTJIS_CHARSET,	// 文字セットの識別子
		OUT_DEFAULT_PRECIS,	// 出力精度
		OUT_DEFAULT_PRECIS,	// クリッピング精度
		PROOF_QUALITY,		// 出力品質
		VARIABLE_PITCH,		// ピッチとファミリ
		"ＭＳ 明朝");		// フォント名

	hDC = GetDC(hWnd);						// ＤＣのハンドル取得

	hFontImage = CreateCompatibleBitmap(hDC, Moji_Width * Moji_Count, Moji_Height);
	hFontDC = CreateCompatibleDC(hDC);
	hGDIObj = SelectObject(hFontDC, hFontImage);
	RECT rc;
	rc.top = 0; rc.bottom = Moji_Height, rc.left = 0; rc.right = Moji_Width * Moji_Count;
	FillRect(hFontDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	// オブジェクトの選択(作成したフォントスタイルを利用)
	SelectObject(hFontDC, g_hFont);
	// 背景色の設定(黒色)
	SetBkColor(hFontDC, COLORREF(0x00000000));
	// テキストの色を設定(白色)
	SetTextColor(hFontDC, COLORREF(0x00FFFFFF));
	// 現在選択されているフォントを使って、文字列を描画
	for (int i = 0; i < Moji_Count; i++)
	{
		char buffer[2];
		wsprintf(buffer, "%c", 0x20 + i);
		TextOut(hFontDC, i * Moji_Width, 0, buffer, 1);
	}
	// ビットマップのビットを取得し、指定された形式でバッファにコピー
	GetDIBits(hDC, hFontImage, 0, g_StandardFont.pBmpInfo->bmiHeader.biHeight, g_StandardFont.pImage, (LPBITMAPINFO)g_StandardFont.pBmpInfo, DIB_RGB_COLORS);

	SelectObject(hFontDC, hGDIObj);
	DeleteObject(hFontDC);
	DELETEOBJECT(hFontImage);
	DELETEOBJECT(g_hFont);
	ReleaseDC(hWnd, hDC);

	// フォントのマスクパターン作成
	g_StandardFont.pBuffer_Mask = new BYTE[sizeof(BITMAPINFO) + sizeof(COLOR_RGB) * Moji_Width * Moji_Height * Moji_Count];
	g_StandardFont.pBmpInfo_Mask = (LPBITMAPINFO)(g_StandardFont.pBuffer_Mask + sizeof(BITMAPFILEHEADER));
	g_StandardFont.pImage_Mask = g_StandardFont.pBuffer_Mask + sizeof(BITMAPINFOHEADER);
	// ビットマップ情報のコピー
	CopyMemory(g_StandardFont.pBmpInfo_Mask, g_StandardFont.pBmpInfo, sizeof(BITMAPINFOHEADER));
	FillMemory(g_StandardFont.pImage_Mask, g_StandardFont.pBmpInfo_Mask->bmiHeader.biWidth * g_StandardFont.pBmpInfo_Mask->bmiHeader.biHeight * sizeof(COLOR_RGB), 0xFF);
	// マスクイメージ作成
	LPBYTE pImage = g_StandardFont.pImage;
	LPBYTE pMaskImage = g_StandardFont.pImage_Mask;
	DWORD Width = (DWORD)(g_StandardFont.pBmpInfo->bmiHeader.biWidth * g_StandardFont.pBmpInfo->bmiHeader.biHeight);
	DWORD* pWidth = &Width;
	__asm
	{
			MOV ECX, DWORD PTR pWidth
			MOV EBX, [ECX]
			MOV	EDX, DWORD PTR pImage
			MOV	EDI, DWORD PTR pMaskImage
			MOV	CL, 8H
		LP1:
			MOV	EAX, [EDX]
			SHL	EAX, CL
			SHR	EAX, CL
			CMP EAX, 0H
			JZ	MVP
			MOV	EAX, [EDI]
			AND	EAX, 0FF000000H
			MOV[EDI], EAX
		MVP : 
			ADD	EDX, 3H
			ADD	EDI, 3H
			DEC	EBX
			JNZ	LP1
	}

	// 論理フォントの作成
	g_hFont = CreateFont(g_iFontHeight,		// フォントの高さ
		g_iFontWidth,		// 平均文字幅
		0,					// 文字送り方向の角度
		0,					// ベースラインの角度
		FW_NORMAL,			// フォントの太さ
		FALSE,				// 斜体にするかどうかの有無
		FALSE,				// 下線を付けるかどうかの有無
		FALSE,				// 取り消し線を付けるかどうかの有無
		SHIFTJIS_CHARSET,	// 文字セットの識別子
		OUT_DEFAULT_PRECIS,	// 出力精度
		OUT_DEFAULT_PRECIS,	// クリッピング精度
		PROOF_QUALITY,		// 出力品質
		VARIABLE_PITCH,		// ピッチとファミリ
		"ＭＳ 明朝");		// フォント名
}
// ------------------------------------------------------------------------
//　関数名	:DrawText			文字列の描画(JISコード表のみ)
//	引数	:hWnd				ウィンドウのハンドル
//　　　　　:X_Position			描画するX座標					
//　　　　　:Y_Position			描画するY座標
//　　　　　:pString			描画する文字列へのポインタを指定
//	戻り値	:					なし
// ------------------------------------------------------------------------
void DrawText(HWND hWnd, int X_Position, int Y_Position, const char *pString)
{
	long ReceivePosition, SendPosition;
	int Unit_Width = Moji_Width;
	int Unit_Height = Moji_Height;
	int X_DataPosition = 0, Y_DataPosition = 0;

	if (!strlen(pString))
		return;

	// ビットマップの格納は、Ｙ座標は上下逆なので、修正する
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - Unit_Height;
	Y_DataPosition = g_StandardFont.pBmpInfo->bmiHeader.biHeight - Unit_Height - Y_DataPosition;

	while (*pString)
	{
		if (0x20 <= *pString && *pString <= 0x7E)
		{
			X_DataPosition = (*pString - 0x20) * Moji_Width;
			Unit_Width = Moji_Width;
			// メモリの範囲内かの有無
			if (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth > X_Position && 0 < X_Position + Unit_Width
				&& g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight > Y_Position && 0 < Y_Position + Unit_Height)
			{
				if (X_Position + Unit_Width > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth)
				{
					// 右にある
					// 幅を求める
					Unit_Width = g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth - X_Position;
				}
				else if (X_Position <= 0)			// 左にある
				{
					// 幅を求める
					Unit_Width = Unit_Width + X_Position;
					// データ位置を求める
					X_DataPosition = X_DataPosition + abs(X_Position);
					X_Position = 0;
				}
				if (g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight <= Y_Position + Unit_Height)
				{
					// 下にある
					// 高さを求める
					Unit_Height = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position;
				}
				else if (Y_Position <= 0)
				{
					// 高さを求める
					Unit_Height = Y_Position + Unit_Height;
					// データ位置を求める
					Y_DataPosition = Y_DataPosition + abs(Y_Position);
					Y_Position = 0; // 上にある
				}
				// メモリー中の座標計算
				ReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);
				SendPosition = X_DataPosition + Y_DataPosition * (g_StandardFont.pBmpInfo->bmiHeader.biWidth);
			}
			else
				return;
			// 背景と重ね合わせて描画
			for (int i = 0; i < Unit_Height; i++)
			{
				LPBYTE lpMemoryImage = g_pMemoryImage_MainWindow + (ReceivePosition + i * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth) * sizeof(COLOR_RGB);
				LPBYTE lpMaskImage = g_StandardFont.pImage_Mask + (SendPosition + i * g_StandardFont.pBmpInfo_Mask->bmiHeader.biWidth) * sizeof(COLOR_RGB);
				LPBYTE lpOrImage = g_StandardFont.pImage + (SendPosition + i * g_StandardFont.pBmpInfo->bmiHeader.biWidth) * sizeof(COLOR_RGB);
				DWORD dwWidth = (DWORD)(Unit_Width * sizeof(COLOR_RGB));
				DWORD* lpWidth = &dwWidth;
				__asm
				{
						MOV ECX, DWORD PTR lpWidth
						MOV EAX, [ECX]
						AND	EAX, 000007FFFH
						PUSH EAX
						MOV	CL, 2H
						SHR	AX, CL
						XOR	EBX, EBX
						MOV	BX, AX
						POP	EAX
						MOV	CL, 6H
						SHL	AL, CL
						SHR	AL, CL
						MOV	CL, AL
						MOV	EDX, DWORD PTR lpMemoryImage
						MOV	EDI, DWORD PTR lpMaskImage
						MOV	ESI, DWORD PTR lpOrImage
						CMP	BX, 0H
						JZ	SFT
					LP1 : 
						MOV	EAX, [EDX]
						AND EAX, [EDI]
						OR	EAX, [ESI]
						MOV[EDX], EAX
						ADD EDX, 4H
						ADD EDI, 4H
						ADD ESI, 4H
						DEC	BX
						JNZ	LP1
					SFT : 
						CMP	CL, 0H
						JZ	END
					LP2 :
						MOV	AL, [EDX]
						MOV	BL, [EDX]
						AND AL, [EDI]
						OR	AL, [ESI]
						MOV[EDX], AL
						INC EDX
						INC EDI
						INC ESI
						DEC	CL
						CMP	CL, 0H
						JNZ	LP2
					END :
				}
			}
			X_Position += Moji_Width;
		}
		pString++;
	}
	// 画像更新フラグON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//　関数名	:DrawText			文字列の描画
//	引数	:hWnd				ウィンドウのハンドル
//　　　　　:X_Position			描画するX座標					
//　　　　　:Y_Position			描画するY座標
//　　　　　:pString			描画する文字列へのポインタを指定
//			:Color				文字の色
//	戻り値	:					なし
// ------------------------------------------------------------------------
void DrawText(HWND hWnd, int X_Position, int Y_Position, const char *pString, COLORREF Color)
{
	HDC hDC, hMemDC;								// ＤＣの割付変数
	HGDIOBJ hGDIObj;							// ＤＣ開放用ポインタ

	// デバイスコンテキストハンドルの取得
	hDC = GetDC(hWnd);
	// メモリデバイスコンテキストハンドルの作成
	hMemDC = CreateCompatibleDC(hDC);
	// DIBセクションをデバイスコンテキストハンドルに選択
	hGDIObj = (HBITMAP)SelectObject(hMemDC, g_DIBSection);
	// フォントをデバイスコンテキストハンドルに選択
	// オブジェクトの選択
	SelectObject(hMemDC,			// ＤＣのハンドルを指定
		g_hFont);					// オブジェクトのハンドルを指定
	// 色の設定
	SetTextColor(hMemDC, Color);
	// 背景を透明化
	SetBkMode(hMemDC, TRANSPARENT);
	// 文字列の描画
	TextOut(hMemDC, X_Position, Y_Position, pString, lstrlen(pString));
	SelectObject(hMemDC, hGDIObj);				// オブジェクトの選択
	// デバイスコンテキストハンドルの開放
	DeleteDC(hMemDC);							// オブジェクトの開放
	ReleaseDC(hWnd, hDC);						// ＤＣを解放
	// 画像更新フラグON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//　関数名	:DrawPoint			点の描画
//	引数	:hWnd				ウインドウのハンドル
//　　　　　:X_Position			描画するX座標					
//　　　　　:Y_Position			描画するY座標
//			:Color				色
//	戻り値	:					なし
// ------------------------------------------------------------------------
void DrawPoint(HWND hWnd, int X_Position, int Y_Position, COLORREF Color)
{
	long lReceivePosition;

	// 描画座標がウインドウ座標に入っているかの確認
	if (X_Position < 0 || X_Position > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth
		|| Y_Position < 0 || Y_Position > g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight)
		return;

	// ビットマップの格納は、Ｙ座標は上下逆なので、修正する
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - 1;

	// メモリー中の座標計算
	lReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);

	// メモリの範囲内かの有無
	if (lReceivePosition < 0 || lReceivePosition >= g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth * g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight)
		return;

	COLOR_RGB *pColor;
	COLOR_RGB Color_RGB;
	pColor = (COLOR_RGB *)&Color;
	Color_RGB.rgbRed = pColor->rgbBlue;
	Color_RGB.rgbGreen = pColor->rgbGreen;
	Color_RGB.rgbBlue = pColor->rgbRed;

	CopyMemory(g_pMemoryImage_MainWindow + lReceivePosition * sizeof(COLOR_RGB),
		&Color_RGB, sizeof(COLOR_RGB));

	// 画像更新フラグON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//　関数名	:DrawLine			線の描画
//	引数	:hWnd				ウインドウのハンドル
//　　　　　:X_StartPosition	描画を開始するX座標					
//　　　　　:Y_StartPosition	描画を開始するY座標
//　　　　　:X_EndPosition		描画を終了するX座標					
//　　　　　:Y_EndPosition		描画を終了するY座標
//　　　　　:Color				色
//	戻り値	:					なし
// ------------------------------------------------------------------------
void DrawLine(HWND hWnd, int X_StartPosition, int Y_StartPosition, int X_EndPosition, int Y_EndPosition, COLORREF Color)
{
	float Katamuki = 0.0f;				// 傾き保存用
	int X_Size, Y_Size, Y_Pos;			// X座標の相対値,Y座標の相対値,Y切片
	int X1_Clip = 0, Y1_Clip = 0;		// クリッピング処理用
	int X2_Clip = Window_Width - 1;		// クリッピング処理用
	int Y2_Clip = Window_Height - 1;	// クリッピング処理用
	BYTE Clip1 = 0x00, Clip2 = 0x00;	// クリッピング処理用
	int x, y, i, j = 0;

	// 相対値を求める
	X_Size = X_EndPosition - X_StartPosition;
	Y_Size = Y_EndPosition - Y_StartPosition;

	// 傾きを求める
	if (X_Size == 0)		// 傾きが無限大の処理
		X_Size = -1;		// 垂直処理
	else if (Y_Size != 0)	// 傾きを普通に求める
		Katamuki = (float)Y_Size / (float)X_Size;
	else
		Katamuki = 0.0f;	// 傾きが割り切れないので

	if ((X_StartPosition > X_EndPosition && -1.0f <= Katamuki && Katamuki <= 1.0f)
		|| ((Y_StartPosition > Y_EndPosition && Katamuki < -1.0f) || (Y_StartPosition > Y_EndPosition && Katamuki > 1.0f)))
	{
		// ２つの座標を交換する
		j = X_StartPosition;
		X_StartPosition = X_EndPosition;
		X_EndPosition = j;
		j = Y_StartPosition;
		Y_StartPosition = Y_EndPosition;
		Y_EndPosition = j;
	}

	// Y切片を求める
	if (X_Size != -1)
		Y_Pos = (int)((float)Y_StartPosition - Katamuki * (float)X_StartPosition + 0.5f);

	// 点の座標位置確認
	if (X_StartPosition < X1_Clip)
		Clip1 |= 0x01;
	if (X_StartPosition > X2_Clip)
		Clip1 |= 0x02;
	if (Y_StartPosition < Y1_Clip)
		Clip1 |= 0x04;
	if (Y_StartPosition > Y2_Clip)
		Clip1 |= 0x08;
	if (X_EndPosition < X1_Clip)
		Clip2 |= 0x01;
	if (X_EndPosition > X2_Clip)
		Clip2 |= 0x02;
	if (Y_EndPosition < Y1_Clip)
		Clip2 |= 0x04;
	if (Y_EndPosition > Y2_Clip)
		Clip2 |= 0x08;

	// クリッピング処理
	if ((Clip1 & Clip2) != 0x00)		// 画面外
		return;
	else {
		if (Clip1 != 0x00 || Clip2 != 0x00)
		{
			if (Clip1 == 0x00)
			{
				if (Clip2 & 0x01)
				{
					X_EndPosition = X1_Clip;
					Y_EndPosition = (int)(Katamuki * (float)X_EndPosition + 0.5f) + Y_Pos;
				}
				else if (Clip2 & 0x02)
				{
					X_EndPosition = X2_Clip;
					Y_EndPosition = (int)(Katamuki * (float)X_EndPosition + 0.5f) + Y_Pos;
				}
				else if (Clip2 & 0x04)
				{
					Y_EndPosition = Y1_Clip;
					if (X_Size != -1)
						X_EndPosition = (int)((float)(Y_EndPosition - Y_Pos) / Katamuki + 0.5f);
				}
				else if (Clip2 & 0x08)
				{
					Y_EndPosition = Y2_Clip;
					if (X_Size != -1)
						X_EndPosition = (int)((float)(Y_EndPosition - Y_Pos) / Katamuki + 0.5f);
				}
			}
			else
			{
				if (Clip1 & 0x01)
				{
					X_StartPosition = X1_Clip;
					Y_StartPosition = (int)(Katamuki * (float)X_StartPosition + 0.5f) + Y_Pos;
				}
				else if (Clip1 & 0x02)
				{
					X_StartPosition = X2_Clip;
					Y_StartPosition = (int)(Katamuki * (float)X_StartPosition + 0.5f) + Y_Pos;
				}
				else if (Clip1 & 0x04)
				{
					Y_StartPosition = Y1_Clip;
					if (X_Size != -1)
						X_StartPosition = (int)((float)(Y_StartPosition - Y_Pos) / Katamuki + 0.5f);
				}
				else if (Clip1 & 0x08)
				{
					Y_StartPosition = Y2_Clip;
					if (X_Size != -1)
						X_StartPosition = (int)((float)(Y_StartPosition - Y_Pos) / Katamuki + 0.5f);
				}
			}
		}
	}

	// 線を引く
	if (X_Size != -1)
	{
		if (Katamuki >= 0.0f && Katamuki <= 1.0f)
		{
			for (i = X_StartPosition; i <= X_EndPosition; i++)
			{
				y = (int)(Katamuki * (float)i + 0.5f) + Y_Pos;
				DrawPoint(hWnd, i, y, Color);
			}
		}
		else if (Katamuki > 1.0f)
		{
			for (i = Y_StartPosition; i <= Y_EndPosition; i++)
			{
				x = (int)((float)(i - Y_Pos) / Katamuki + 0.5f);
				DrawPoint(hWnd, x, i, Color);
			}
		}
		else if (Katamuki < 0.0 && Katamuki >= -1.0f)
		{
			for (i = X_StartPosition; i <= X_EndPosition; i++)
			{
				y = (int)(Katamuki * (float)i + 0.5f) + Y_Pos;
				DrawPoint(hWnd, i, y, Color);
			}
		}
		else if (Katamuki < -1.0f)
		{
			for (i = Y_StartPosition; i <= Y_EndPosition; i++)
			{
				x = (int)((float)(i - Y_Pos) / Katamuki + 0.5f);
				DrawPoint(hWnd, x, i, Color);
			}
		}
	}
	else {
		if (Y_StartPosition < Y_EndPosition)
			for (i = Y_StartPosition; i <= Y_EndPosition; i++)
				DrawPoint(hWnd, X_StartPosition, i, Color);
		else
			for (i = Y_EndPosition; i <= Y_StartPosition; i++)
				DrawPoint(hWnd, X_StartPosition, i, Color);
	}
}
