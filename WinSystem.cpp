// --------------------------------------------------
//�@DrawSample Ver0.50
//	WinSystem.cpp			Programing By N.Sonoda
// --------------------------------------------------
// -------------------- IncludeFile ----------------
#include "WinSystem.h"
// ------------------------------------------------------------------------
//�@�֐���	:MainRegisterClass	���C���E�B���h�E�N���X��o�^
//	����	:hInstance			�C���X�^���X�n���h��(Windows���Ŏ��s����Ă���v���O��������ӂɎ��ʂ���)	
//	�߂�l	:					���s:0
// ------------------------------------------------------------------------
BOOL MainRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;					// �E�B���h�E�N���X�̏��i�[�p�\���̕ϐ�

	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);					// �\���̂̃T�C�Y��ݒ�
	wcex.style = CS_HREDRAW | CS_VREDRAW;				// �E�B���h�E�X�^�C��
	wcex.lpfnWndProc = (WNDPROC)WndProc;				// �E�B���h�E�v���V�[�W���̎w��
	wcex.cbClsExtra = 0;								// �ǉ��̈�͎g��Ȃ�
	wcex.cbWndExtra = 0;								// �ǉ��̈�͎g��Ȃ�
	wcex.hInstance = hInstance;							// ���̃C���X�^���X�̃n���h��
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);	// ���[�W�A�C�R��
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// �J�[�\���X�^�C��
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// �w�i�F ���F
	wcex.lpszMenuName = NULL;									// �\�����j���[�̃Z���N�g(���j���[�Ȃ�)
	wcex.lpszClassName = "MainWindowClass";					// �E�B���h�E�N���X��
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);// �X���[���A�C�R��

	return RegisterClassEx(&wcex);		// �E�C���h�E�N���X�o�^
}
// ------------------------------------------------------------------------
//�@�֐���	:WinMain		�A�v���P�[�V�����G���g���[�|�C���g	
//	����	:hInstance		�C���X�^���X�n���h��(Windows���Ŏ��s����Ă���v���O��������ӂɎ��ʂ���)	
//			:hPrevInstance	Windows95�ȍ~��NULL
//			:pCmdLine		�v���O�����ɓn���ꂽ�R�}���h���C���������i�[���ꂽ������̃|�C���^
//			:CmdShow		�E�B���h�E�Y�̏��������@
//	�߂�l	:				Windows�ւ̏I���R�[�h		
// ------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		pCmdLine,
	int			CmdShow)
{
	HWND hWnd;				// ���C���E�C���h�E�̃n���h��
	HWND hDeskWnd;			// �f�B�X�N�g�b�v�̃E�C���h�E�n���h��
	MSG	msg;				// ���b�Z�[�W�L���[����擾�������b�Z�[�W���i�[�p�\���̕ϐ�
	HMENU hMenu;			// ���j���[�̃n���h��
	RECT deskrc, rc;
	int Window_xPos, Window_yPos;
	BOOL bError_Flag = FALSE;
	HDC hDC;
	const char *pszMutexObjectName = "DrawSample";

	// �~���[�e�b�N�X�I�u�W�F�N�g�̍쐬
	hMutex = CreateMutex(NULL,					// �Z�L�����e�B�����\���̂ւ̃|�C���^
		FALSE,					// �쐬�����~���[�e�b�N�X���ŏ����珊�L���邩�̗L�� 
		pszMutexObjectName);	// �~���[�e�b�N�X�I�u�W�F�N�g�̖��O
	// ���ɃA�v���P�[�V���������s����Ă��邩�̊m�F
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "���ɍ쓮���Ȃ̂ŁA���s�ł��܂���B", "�m�F", MB_OK);
		return FALSE;
	}

	g_hInst = hInstance;				// �C���X�^���X�n���h���̕ۑ�
	if (!MainRegisterClass(hInstance))	// ���C���E�B���h�E�N���X�̓o�^
		return FALSE;

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow("MainWindowClass",		// �N���X��
		"DrawSample",			// �^�C�g��
		WS_MINIMIZEBOX			// �X�^�C��
		| WS_OVERLAPPED
		| WS_SYSMENU,
		CW_USEDEFAULT,			// �����W(Windows�ɔC����)
		CW_USEDEFAULT,			// �����W(Windows�ɔC����)�����ō���p�̍��W
		Window_Width,			// ��
		Window_Height,			// ����
		HWND_DESKTOP,			// ����
		(HMENU)NULL,			// ���j���[����
		hInstance,				// ���̃v���O�����̃C���X�^���X�̃n���h��
		NULL);					// �ǉ���������
	g_hWnd = hWnd;

	GetClientRect(hWnd, &rc);
	MoveWindow(hWnd, CW_USEDEFAULT, CW_USEDEFAULT, Window_Width + Window_Width - rc.right, Window_Height + Window_Height - rc.bottom, TRUE);

	hMenu = GetSystemMenu(hWnd, FALSE);			// �����ݒ胁�j���[�폜
	DeleteMenu(hMenu, 0, MF_BYPOSITION);
	DeleteMenu(hMenu, 1, MF_BYPOSITION);
	DeleteMenu(hMenu, 2, MF_BYPOSITION);
	DrawMenuBar(hWnd);

	// �E�B���h�E���f�B�X�N�g�b�v��̒����ɕ\�����邽�߁A���W�̎擾
	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
	GetWindowRect(hWnd, (LPRECT)&rc);
	Window_xPos = (deskrc.right - (rc.right - rc.left)) / 2;
	Window_yPos = (deskrc.bottom - (rc.bottom - rc.top)) / 2;

	if (hWnd != NULL)
	{
		// �|�W�V�������ړ�����
		SetWindowPos(hWnd, HWND_TOP, Window_xPos, Window_yPos, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);
		// �E�B���h�E��\��
		ShowWindow(hWnd, SW_SHOWNORMAL);
	}
	else return FALSE;

	// ���C�� ���b�Z�[�W ���[�v
	msg.message = WM_CREATE;
	while (msg.message != WM_QUIT){// ���b�Z�[�W���[�v
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);			// �L�[�{�[�h���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ����ă��b�Z�[�W�L���[�Ƀ|�X�g
			DispatchMessage(&msg);			// �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}
		if (g_MainLoop && g_MainCreate_Flag)
		{
			if (g_Main_Time)
			{
				if (!IsIconic(hWnd))		// �E�B���h�E�̍ŏ����L��
				{
					// �摜�X�V�t���OOFF
					if (g_UpdateWindow)
						InterlockedExchange(&g_UpdateWindow, 0);
					MainLoop(hWnd);			// �Q�[�����C���֐��̌Ăяo��
					InterlockedDecrement(&g_Main_Time);
					// �摜�̍X�V����
					if (g_UpdateWindow)
					{
						// �f�o�C�X�R���e�L�X�g�n���h���̎擾
						hDC = GetDC(hWnd);
						StretchDIBits(hDC, 0, 0, Window_Width, Window_Height, 0, 0, Window_Width, Window_Height,
							g_pMemoryImage_MainWindow, g_pMemoryImageInfo_MainWindow, DIB_RGB_COLORS, SRCCOPY);
						// �f�o�C�X�R���e�L�X�g�n���h���̊J��
						ReleaseDC(hWnd, hDC);
					}
				}
			}
		}
		Sleep(1);
	}

	ReleaseMutex(hMutex);				// �~���[�e�b�N�X�I�u�W�F�N�g���L���̉��
	CloseHandle(hMutex);				// �~���[�e�b�N�X�I�u�W�F�N�g�p�n���h���̊J��

	return (int)msg.wParam;
}
// ------------------------------------------------------------------------
//�@�֐���	:WndProc		�E�B���h�E�̃v���V�[�W��	
//	����	:hWnd			�E�B���h�E�̃n���h��
//			:Message		���b�Z�[�W�̎��
//			:wParam			���b�Z�[�W�̕⏕���
//			:lParam			���b�Z�[�W�̕⏕���
//	�߂�l	:				���b�Z�[�W��߂�		
// ------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	static DWORD threadID;							// �X���b�h�pID

	switch (uMessage)
	{
		case WM_CREATE:
		g_pBmp = new Bmp[Resource_Count];
		// �r�b�g�}�b�v�̓ǂݍ���
		for (int i = 0; i < Resource_Count; i++)
		{
			g_bBitMapLoadSuccess_Flag = LoadBitMap(hWnd, i);
			if (!g_bBitMapLoadSuccess_Flag) break;
		}
		if (!g_bBitMapLoadSuccess_Flag)
		{
			MessageBox(hWnd, "�r�b�g�}�b�v�t�@�C������ǂݍ��߂܂���ł����B", "�m�F", MB_OK);
			PostQuitMessage(0);
			break;
		}
		// �摜�����֘A�̏�����
		DrawInit(hWnd);
		if (!g_hMainThread)		//�X���b�h�̍쐬
		{
			g_hMainThread = CreateThread(
				0,											// �Z�L�����e�B����(�Ȃ�)
				0,											// ���b�h�����X�^�b�N�̃T�C�Y���A�o�C�g�P�ʂŎw��(�f�t�H���g)
				(LPTHREAD_START_ROUTINE)TimerThread,		// �X���b�h�̊J�n�A�h���X���w��
				(VOID *)hWnd,								// �X���b�h�ւ̈����w��(�A�h���X)
				0,											// ����t���O���w��(�ݒ�Ȃ�)
				&threadID);									// �X���b�hID���i�[����Ă���|�C���^�w�� 
		}
		// �C�x���g�I�u�W�F�N�g���쐬
		g_hMainEvent = CreateEvent(
			0,		// �Z�L�����e�B����(�ݒ�Ȃ�)
			TRUE,	// �蓮/�������Z�b�g�I�u�W�F�N�g�쐬
			TRUE,	// �C�x���g�I�u�W�F�N�g�̏�����Ԏw��(�V�O�i�����)
			0);		// �C�x���g�I�u�W�F�N�g�̖��O
		SetEvent(g_hMainEvent);				// �I�u�W�F�N�g���V�O�i����Ԃɐݒ�
		if (!g_hMaskThread){	//�X���b�h�̍쐬
			g_hMaskThread = CreateThread(
				0,											// �Z�L�����e�B����(�Ȃ�)
				0,											// ���b�h�����X�^�b�N�̃T�C�Y���A�o�C�g�P�ʂŎw��(�f�t�H���g)
				(LPTHREAD_START_ROUTINE)CreateMaskThread,	// �X���b�h�̊J�n�A�h���X���w��
				(VOID *)NULL,								// �X���b�h�ւ̈����w��(�A�h���X)
				0,											// ����t���O���w��(�ݒ�Ȃ�)
				&threadID);									// �X���b�hID���i�[����Ă���|�C���^�w�� 
		}
		// �C�x���g�I�u�W�F�N�g���쐬
		g_hMaskEvent = CreateEvent(
			0,		// �Z�L�����e�B����(�ݒ�Ȃ�)
			TRUE,	// �蓮/�������Z�b�g�I�u�W�F�N�g�쐬
			TRUE,	// �C�x���g�I�u�W�F�N�g�̏�����Ԏw��(�V�O�i�����)
			0);		// �C�x���g�I�u�W�F�N�g�̖��O
		SetEvent(g_hMaskEvent);				// �I�u�W�F�N�g���V�O�i����Ԃɐݒ�
		CreateFont(hWnd);					// �t�H���g�̍쐬
		break;
		// �E�C���h�E�̔j������	
	case WM_CLOSE:
		Wm_DestroyProc(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	return 0;
}
// ------------------------------------------------------------------------
//�@�֐���	:Wm_DestroyProc		�I������	
//	����	:hWnd				�E�B���h�E�̃n���h��
//	�߂�l	:					����:1�A���s:0
// ------------------------------------------------------------------------
BOOL Wm_DestroyProc(HWND hWnd)
{
	if (g_hMainThread)
	{
		ResetEvent(g_hMainEvent);	// �C�x���g�I�u�W�F�N�g���V�O�i����Ԃɐݒ�
		// �w�肵����޼ު�Ă�����ُ�ԂɂȂ邩�A�܂��́A��ѱ�Ă��������邩�܂ő҂�
		while (WaitForSingleObject(g_hMainThread, 0) == WAIT_TIMEOUT)
			Sleep(1);
		g_hMainThread = NULL;
	}

	// �摜�֘A�f�[�^�̊J��
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
//�@�֐���	:TimerThread		���C�����[�v�̎��Ԃ����ɕۂ�	
//	����	:hWnd				�E�B���h�E�̃n���h��
//	�߂�l	:					�Ȃ�
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
		// Time����肷��
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
//�@�֐���	:LoadBitMap			�r�b�g�}�b�v�̓ǂݍ���
//	����	:hWnd				�E�B���h�E�̃n���h��
//			:BmpNum				���[�h����r�b�g�}�b�v�ԍ�		
//	�߂�l	:					����:1�A���s:0
// ------------------------------------------------------------------------
BOOL LoadBitMap(HWND hWnd, UINT BmpNum)
{
	LPBITMAPINFO pBitMap;
	HANDLE hResource;
	HRSRC hRsrc;

	// ���\�[�X�̃n���h���擾
	hRsrc = FindResource(g_hInst, g_szResourceID[BmpNum], RT_BITMAP);
	if (hRsrc == NULL)
	{
		MessageBox(hWnd, "���\�[�X������܂���B", "�m�F", MB_OK);
		return FALSE;
	}
	// ���\�[�X���������[�Ƀ��[�h����
	hResource = LoadResource(g_hInst, hRsrc);
	if (hRsrc == NULL)
	{
		MessageBox(hWnd, "���\�[�X�����[�h�ł��܂���B", "�m�F", MB_OK);
		return FALSE;
	}

	// ���������̎w�肳�ꂽ���\�[�X�����b�N
	pBitMap = (LPBITMAPINFO)LockResource(hResource);

	// 24BitColor�ō쐬
	g_pBmp[BmpNum].pBuffer = new BYTE[sizeof(BITMAPINFO) + pBitMap->bmiHeader.biWidth * pBitMap->bmiHeader.biHeight * sizeof(COLOR_RGB)];

	g_pBmp[BmpNum].pBmpInfo = (LPBITMAPINFO)g_pBmp[BmpNum].pBuffer;
	g_pBmp[BmpNum].pImage = (LPBYTE)(g_pBmp[BmpNum].pBuffer + sizeof(BITMAPINFOHEADER));
	LPBYTE pBitMapImageBackup = g_pBmp[BmpNum].pImage;
	// �w�b�_�[���̃R�s�[
	CopyMemory(g_pBmp[BmpNum].pBmpInfo, (char *)pBitMap, sizeof(BITMAPINFOHEADER));

	LPBYTE pPos;		// �ǂݍ��ރ|�W�V������ۑ�

	if (pBitMap->bmiHeader.biBitCount == 16)		// 2,16,256�F,24BitColor�ȊO�̏ꍇ
	{
		MessageBox(hWnd, "���\�[�X��16BitColor�ȊO�ŁA�o�^���Ă��������B", "�m�F", MB_OK);
		return FALSE;
	}
	// �F���ɉ����ăf�[�^���R�s�[
	if (pBitMap->bmiHeader.biBitCount != 24)
	{
		// �_���p���b�g��W�J
		// �ǂݍ��ރ|�W�V����������
		pPos = (LPBYTE)pBitMap + sizeof(BITMAPINFOHEADER) + ((1 << g_pBmp[BmpNum].pBmpInfo->bmiHeader.biBitCount) * sizeof(RGBQUAD));
		// �r�b�g�}�b�v�f�[�^�̃R�s�[
		if (pBitMap->bmiHeader.biBitCount == 1)		// 2�F
		{
			for (LONG i = 0; i < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight; i++)
			{
				short BitPosition = 7;		// �r�b�g��̏ꏊ���w��(7:�ŏ�ʃr�b�g)
				LPBYTE lpPosBackup = pPos;
				for (LONG j = 0; j < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth; j++)
				{
					BYTE Data = *pPos;
					// �r�b�g��̃V�t�g
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
		if (pBitMap->bmiHeader.biBitCount == 4)		// 16�F
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
					// �r�b�g���Higt��Low�ɕ�����
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
		if (pBitMap->bmiHeader.biBitCount == 8)		// 256�F
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
		// �F����24BitColor�ɕύX
		g_pBmp[BmpNum].pBmpInfo->bmiHeader.biBitCount = 24;
	}
	else
	{
		// 24BitColor�Ȃ̂Ńr�b�g�}�b�v�f�[�^�����̂܂ܓW�J
		// �r�b�g�}�b�v�f�[�^�̃R�s�[
		// �ǂݍ��ރ|�W�V����������
		pPos = (LPBYTE)pBitMap + sizeof(BITMAPINFOHEADER);
		for (LONG i = 0; i < g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight; i++)
		{
			CopyMemory(pBitMapImageBackup, pPos, sizeof(COLOR_RGB) * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth);
			pPos += sizeof(COLOR_RGB) * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth + g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth % 4;
			pBitMapImageBackup += sizeof(COLOR_RGB) * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth;
		}

	}
	// �}�X�N
	// 24BitColor�ō쐬
	g_pBmp[BmpNum].pBuffer_Mask = new BYTE[sizeof(BITMAPINFO) + g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight * sizeof(COLOR_RGB)];

	g_pBmp[BmpNum].pBmpInfo_Mask = (LPBITMAPINFO)g_pBmp[BmpNum].pBuffer_Mask;
	g_pBmp[BmpNum].pImage_Mask = (LPBYTE)(g_pBmp[BmpNum].pBuffer_Mask + sizeof(BITMAPINFOHEADER));
	// �w�b�_�[���̃R�s�[
	CopyMemory(g_pBmp[BmpNum].pBmpInfo_Mask, (char *)g_pBmp[BmpNum].pBmpInfo, sizeof(BITMAPINFOHEADER));
	// �C���[�W����(0XFF)�Ŗ��߂�
	FillMemory(g_pBmp[BmpNum].pImage_Mask, g_pBmp[BmpNum].pBmpInfo_Mask->bmiHeader.biWidth * g_pBmp[BmpNum].pBmpInfo_Mask->bmiHeader.biHeight * sizeof(COLOR_RGB), 0xFF);

	return TRUE;
}
// ------------------------------------------------------------------------
//�@�֐���	:DrawInit			�摜�����֘A�̏�����	
//	����	:hWnd				�E�B���h�E�̃n���h��
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void DrawInit(HWND hWnd)
{
	HDC hDC;									// �c�b�̊��t�ϐ�
	// ���C���E�C���h�E
	if (g_pMemoryImageBuffer_MainWindow != NULL)
	{
		delete[] g_pMemoryImageBuffer_MainWindow;
		g_pMemoryImageBuffer_MainWindow = NULL;
	}
	// 24BitColor�ō쐬
	g_pMemoryImageBuffer_MainWindow = new BYTE[sizeof(BITMAPINFO) + Window_Width * Window_Height * sizeof(COLOR_RGB)];

	g_pMemoryImageInfo_MainWindow = (LPBITMAPINFO)g_pMemoryImageBuffer_MainWindow;
	g_pMemoryImage_MainWindow = (BYTE*)(g_pMemoryImageBuffer_MainWindow + sizeof(BITMAPINFOHEADER));

	g_pMemoryImageInfo_MainWindow->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	// �\���̂̃T�C�Y
	g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth = Window_Width;				// ��
	g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight = Window_Height;			// ����
	g_pMemoryImageInfo_MainWindow->bmiHeader.biPlanes = 1;						// �v���[����
	g_pMemoryImageInfo_MainWindow->bmiHeader.biBitCount = 24;					// �F��[1,4,8,24](24BitColor)
	g_pMemoryImageInfo_MainWindow->bmiHeader.biCompression = 0;					// ���k����(�Ȃ�)
	g_pMemoryImageInfo_MainWindow->bmiHeader.biSizeImage = 0;					// �r�b�g�}�b�v�T�C�Y(�Ȃ�)
	g_pMemoryImageInfo_MainWindow->bmiHeader.biXPelsPerMeter = 0;				// �����𑜓x
	g_pMemoryImageInfo_MainWindow->bmiHeader.biYPelsPerMeter = 0;				// �����𑜓x
	g_pMemoryImageInfo_MainWindow->bmiHeader.biClrUsed = 0;						// �F��
	g_pMemoryImageInfo_MainWindow->bmiHeader.biClrImportant = 0;					// �d�v�ȐF��
	hDC = GetDC(hWnd);
	// �����`��̈�DIB�Z�N�V�����̍쐬
	g_DIBSection = CreateDIBSection(hDC,								// �f�o�C�X�R���e�L�X�g�n���h��
		g_pMemoryImageInfo_MainWindow,	// �匳��ʂ�BITMAPINFO�\���̏��
		DIB_RGB_COLORS,					// �J���[�w��
		(void**)&g_pMemoryImage_MainWindow,	// DIB�̃r�b�g�}�b�v�o�b�t�@�̃A�h���X������|�C���^(���̕ϐ��͊J�����Ă͂����Ȃ��I)
		NULL,							// �t�@�C���}�b�s���O���g�����Ɏg�p�B�ʏ��NULL
		0);								// �t�@�C���}�b�s���O���g�����Ɏg�p�B�ʏ��0
	ReleaseDC(hWnd, hDC);
}
// ------------------------------------------------------------------------
//�@�֐���	:DrawBitMap			�r�b�g�}�b�v�֘A�̕`��
//	����	:hWnd				�E�B���h�E�̃n���h��
//�@�@�@�@�@:X_Position			�`�悷��X���W					
//�@�@�@�@�@:Y_Position			�`�悷��Y���W
//�@�@�@�@�@:X_DataPosition		�f�[�^������X���W
//�@�@�@�@�@:Y_DataPosition		�f�[�^������Y���W
//�@�@�@�@�@:Unit_Width			��
//�@�@�@�@�@:Unit_Height		����
//			:BmpNum				�r�b�g�}�b�v�ԍ�
//			:OperationCode		�`����@
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void DrawBitMap(HWND hWnd, int X_Position, int Y_Position, int X_DataPosition, int Y_DataPosition
	, int Unit_Width, int Unit_Height, UINT BmpNum, short OperationCode)
{
	LONG ReceivePosition, SendPosition;

	// ���Ɍ�肪����
	if (BmpNum < 0 || Resource_Count <= BmpNum)
		return;
	// �r�b�g�}�b�v�̕\������傫���́A�r�b�g�}�b�v�f�[�^�����m�F����
	if (!(X_DataPosition + Unit_Width <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth
		&& Y_DataPosition + Unit_Height <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight))
		return;

	// �r�b�g�}�b�v�̊i�[�́A�x���W�͏㉺�t�Ȃ̂ŁA�C������
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - Unit_Height;
	Y_DataPosition = g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight - Unit_Height - Y_DataPosition;

	// �������͈͓̔����̗L��
	if (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth > X_Position && 0 < X_Position + Unit_Width
		&& g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight > Y_Position && 0 < Y_Position + Unit_Height)
	{
		if (X_Position + Unit_Width > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth)
		{
			// �E�ɂ���
			// �������߂�
			Unit_Width = g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth - X_Position;
		}
		else if (X_Position <= 0)			// ���ɂ���
		{
			// �������߂�
			Unit_Width = Unit_Width + X_Position;
			// �f�[�^�ʒu�����߂�
			X_DataPosition = X_DataPosition + abs(X_Position);
			X_Position = 0;
		}
		if (g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight <= Y_Position + Unit_Height)
		{
			// ���ɂ���
			// ���������߂�
			Unit_Height = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position;
		}
		else if (Y_Position <= 0)
		{
			// ���������߂�
			Unit_Height = Y_Position + Unit_Height;
			// �f�[�^�ʒu�����߂�
			Y_DataPosition = Y_DataPosition + abs(Y_Position);
			Y_Position = 0; // ��ɂ���
		}
		// �������[���̍��W�v�Z
		ReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);
		SendPosition = X_DataPosition + Y_DataPosition * (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth);
	}
	else
		return;

	if (!OperationCode)	// ���̂܂ܕ`��
	{
		for (int i = 0; i < Unit_Height; i++)
		{
			CopyMemory(g_pMemoryImage_MainWindow + (ReceivePosition + i * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				g_pBmp[BmpNum].pImage + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				Unit_Width * sizeof(COLOR_RGB));
		}
	}
	else					// �w�i�Əd�ˍ��킹�ĕ`��
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

	// �摜�X�V�t���OON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//�@�֐���	:DrawBitMap			�r�b�g�}�b�v�֘A�̕`��
//	����	:hWnd				�E�B���h�E�̃n���h��
//�@�@�@�@�@:X_Position			�`�悷��X���W					
//�@�@�@�@�@:Y_Position			�`�悷��Y���W
//�@�@�@�@�@:X_DataPosition		�f�[�^������X���W
//�@�@�@�@�@:Y_DataPosition		�f�[�^������Y���W
//�@�@�@�@�@:Unit_Width			��
//�@�@�@�@�@:Unit_Height		����
//			:BmpNum				�r�b�g�}�b�v�ԍ�
//			:OperationCode		�`����@
//			:Alpha				�A���t�@�[�l
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void DrawBitMap(HWND hWnd, int X_Position, int Y_Position, int X_DataPosition, int Y_DataPosition
	, int Unit_Width, int Unit_Height, UINT BmpNum, short OperationCode, float Alpha)
{
	LONG ReceivePosition, SendPosition;

	// ���Ɍ�肪����
	if (BmpNum < 0 || Resource_Count <= BmpNum)
		return;
	// �r�b�g�}�b�v�̕\������傫���́A�r�b�g�}�b�v�f�[�^�����m�F����
	if (!(X_DataPosition + Unit_Width <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth
		&& Y_DataPosition + Unit_Height <= g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight))
		return;

	// �r�b�g�}�b�v�̊i�[�́A�x���W�͏㉺�t�Ȃ̂ŁA�C������
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - Unit_Height;
	Y_DataPosition = g_pBmp[BmpNum].pBmpInfo->bmiHeader.biHeight - Unit_Height - Y_DataPosition;

	// �������͈͓̔����̗L��
	if (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth > X_Position && 0 < X_Position + Unit_Width
		&& g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight > Y_Position && 0 < Y_Position + Unit_Height)
	{
		if (X_Position + Unit_Width > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth)
		{
			// �E�ɂ���
			// �������߂�
			Unit_Width = g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth - X_Position;
		}
		else if (X_Position <= 0)		// ���ɂ���
		{
			// �������߂�
			Unit_Width = Unit_Width + X_Position;
			// �f�[�^�ʒu�����߂�
			X_DataPosition = X_DataPosition + abs(X_Position);
			X_Position = 0;
		}
		if (g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight <= Y_Position + Unit_Height)
		{
			// ���ɂ���
			// ���������߂�
			Unit_Height = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position;
		}
		else if (Y_Position <= 0)
		{
			// ���������߂�
			Unit_Height = Y_Position + Unit_Height;
			// �f�[�^�ʒu�����߂�
			Y_DataPosition = Y_DataPosition + abs(Y_Position);
			Y_Position = 0; // ��ɂ���
		}
		// �������[���̍��W�v�Z
		ReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);
		SendPosition = X_DataPosition + Y_DataPosition * (g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth);
	}
	else
		return;

	if (!OperationCode)	// ���̂܂ܕ`��
	{
		for (int i = 0; i < Unit_Height; i++)
		{
			CopyMemory(g_pMemoryImage_MainWindow + (ReceivePosition + i * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				g_pBmp[BmpNum].pImage + (SendPosition + i * g_pBmp[BmpNum].pBmpInfo->bmiHeader.biWidth) * sizeof(COLOR_RGB),
				Unit_Width * sizeof(COLOR_RGB));
		}
	}
	else					// �w�i�Əd�ˍ��킹�ĕ`��
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

	// �摜�X�V�t���OON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//�@�֐���	:CreateMaskThread	�d�ˍ��킹�p�C���[�W�摜�쐬	
//	����	:					�Ȃ�
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void CreateMaskThread()
{
	for (int i = 0; i < Resource_Count; i++)
	{
		// �}�X�N�C���[�W�쐬
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
	ResetEvent(g_hMaskEvent);	// �C�x���g�I�u�W�F�N�g���V�O�i����Ԃɐݒ�

	g_MainCreate_Flag = TRUE;	// �����ݒ�I��
	g_hMaskThread = NULL;
}
// ------------------------------------------------------------------------
//�@�֐���	:ClearScreen		��ʂ����F�œh��Ԃ�
//	����	:hWnd				�E�B���h�E�̃n���h��
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void ClearScreen(HWND hWnd)
{
	// �������[��Ԃ���(0x00)�Ŗ��߂�
	FillMemory(g_pMemoryImage_MainWindow,
		g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight * g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth * sizeof(COLOR_RGB),
		0x00);

	// �摜�X�V�t���OON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//�@�֐���	:CreateFont			�t�H���g�̍쐬
//	����	:					�Ȃ�
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void CreateFont(HWND hWnd)
{
	// �ϐ��̐錾
	HDC hDC, hFontDC;						// �c�b�̊��t�ϐ�
	HGDIOBJ hGDIObj;						// �c�b�J���p�|�C���^
	HBITMAP hFontImage = NULL;				// �t�H���g�C���[�W�ۑ��p

	g_StandardFont.pBuffer = new BYTE[sizeof(BITMAPINFO) + sizeof(COLOR_RGB) * Moji_Width * Moji_Height * Moji_Count];
	g_StandardFont.pBmpInfo = (LPBITMAPINFO)(g_StandardFont.pBuffer + sizeof(BITMAPFILEHEADER));
	g_StandardFont.pImage = g_StandardFont.pBuffer + sizeof(BITMAPINFOHEADER);

	g_StandardFont.pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);// �\���̂̃T�C�Y
	g_StandardFont.pBmpInfo->bmiHeader.biWidth = Moji_Width * Moji_Count;// ��
	g_StandardFont.pBmpInfo->bmiHeader.biHeight = Moji_Height;			// ����
	g_StandardFont.pBmpInfo->bmiHeader.biPlanes = 1;						// �v���[����
	g_StandardFont.pBmpInfo->bmiHeader.biBitCount = 24;					// �F��[1,4,8,24](24BitColor)
	g_StandardFont.pBmpInfo->bmiHeader.biCompression = 0;					// ���k����(�Ȃ�)
	g_StandardFont.pBmpInfo->bmiHeader.biSizeImage = 0;					// �r�b�g�}�b�v�T�C�Y(�Ȃ�)
	g_StandardFont.pBmpInfo->bmiHeader.biXPelsPerMeter = 0;				// �����𑜓x
	g_StandardFont.pBmpInfo->bmiHeader.biYPelsPerMeter = 0;				// �����𑜓x
	g_StandardFont.pBmpInfo->bmiHeader.biClrUsed = 0;	 					// �F��
	g_StandardFont.pBmpInfo->bmiHeader.biClrImportant = 0;				// �d�v�ȐF��

	// �_���t�H���g�̍쐬
	g_hFont = CreateFont(Moji_Height,		// �t�H���g�̍���
		Moji_Width,			// ���ϕ�����
		0,					// ������������̊p�x
		0,					// �x�[�X���C���̊p�x
		FW_NORMAL,			// �t�H���g�̑���
		FALSE,				// �Α̂ɂ��邩�ǂ����̗L��
		FALSE,				// ������t���邩�ǂ����̗L��
		FALSE,				// ����������t���邩�ǂ����̗L��
		SHIFTJIS_CHARSET,	// �����Z�b�g�̎��ʎq
		OUT_DEFAULT_PRECIS,	// �o�͐��x
		OUT_DEFAULT_PRECIS,	// �N���b�s���O���x
		PROOF_QUALITY,		// �o�͕i��
		VARIABLE_PITCH,		// �s�b�`�ƃt�@�~��
		"�l�r ����");		// �t�H���g��

	hDC = GetDC(hWnd);						// �c�b�̃n���h���擾

	hFontImage = CreateCompatibleBitmap(hDC, Moji_Width * Moji_Count, Moji_Height);
	hFontDC = CreateCompatibleDC(hDC);
	hGDIObj = SelectObject(hFontDC, hFontImage);
	RECT rc;
	rc.top = 0; rc.bottom = Moji_Height, rc.left = 0; rc.right = Moji_Width * Moji_Count;
	FillRect(hFontDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	// �I�u�W�F�N�g�̑I��(�쐬�����t�H���g�X�^�C���𗘗p)
	SelectObject(hFontDC, g_hFont);
	// �w�i�F�̐ݒ�(���F)
	SetBkColor(hFontDC, COLORREF(0x00000000));
	// �e�L�X�g�̐F��ݒ�(���F)
	SetTextColor(hFontDC, COLORREF(0x00FFFFFF));
	// ���ݑI������Ă���t�H���g���g���āA�������`��
	for (int i = 0; i < Moji_Count; i++)
	{
		char buffer[2];
		wsprintf(buffer, "%c", 0x20 + i);
		TextOut(hFontDC, i * Moji_Width, 0, buffer, 1);
	}
	// �r�b�g�}�b�v�̃r�b�g���擾���A�w�肳�ꂽ�`���Ńo�b�t�@�ɃR�s�[
	GetDIBits(hDC, hFontImage, 0, g_StandardFont.pBmpInfo->bmiHeader.biHeight, g_StandardFont.pImage, (LPBITMAPINFO)g_StandardFont.pBmpInfo, DIB_RGB_COLORS);

	SelectObject(hFontDC, hGDIObj);
	DeleteObject(hFontDC);
	DELETEOBJECT(hFontImage);
	DELETEOBJECT(g_hFont);
	ReleaseDC(hWnd, hDC);

	// �t�H���g�̃}�X�N�p�^�[���쐬
	g_StandardFont.pBuffer_Mask = new BYTE[sizeof(BITMAPINFO) + sizeof(COLOR_RGB) * Moji_Width * Moji_Height * Moji_Count];
	g_StandardFont.pBmpInfo_Mask = (LPBITMAPINFO)(g_StandardFont.pBuffer_Mask + sizeof(BITMAPFILEHEADER));
	g_StandardFont.pImage_Mask = g_StandardFont.pBuffer_Mask + sizeof(BITMAPINFOHEADER);
	// �r�b�g�}�b�v���̃R�s�[
	CopyMemory(g_StandardFont.pBmpInfo_Mask, g_StandardFont.pBmpInfo, sizeof(BITMAPINFOHEADER));
	FillMemory(g_StandardFont.pImage_Mask, g_StandardFont.pBmpInfo_Mask->bmiHeader.biWidth * g_StandardFont.pBmpInfo_Mask->bmiHeader.biHeight * sizeof(COLOR_RGB), 0xFF);
	// �}�X�N�C���[�W�쐬
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

	// �_���t�H���g�̍쐬
	g_hFont = CreateFont(g_iFontHeight,		// �t�H���g�̍���
		g_iFontWidth,		// ���ϕ�����
		0,					// ������������̊p�x
		0,					// �x�[�X���C���̊p�x
		FW_NORMAL,			// �t�H���g�̑���
		FALSE,				// �Α̂ɂ��邩�ǂ����̗L��
		FALSE,				// ������t���邩�ǂ����̗L��
		FALSE,				// ����������t���邩�ǂ����̗L��
		SHIFTJIS_CHARSET,	// �����Z�b�g�̎��ʎq
		OUT_DEFAULT_PRECIS,	// �o�͐��x
		OUT_DEFAULT_PRECIS,	// �N���b�s���O���x
		PROOF_QUALITY,		// �o�͕i��
		VARIABLE_PITCH,		// �s�b�`�ƃt�@�~��
		"�l�r ����");		// �t�H���g��
}
// ------------------------------------------------------------------------
//�@�֐���	:DrawText			������̕`��(JIS�R�[�h�\�̂�)
//	����	:hWnd				�E�B���h�E�̃n���h��
//�@�@�@�@�@:X_Position			�`�悷��X���W					
//�@�@�@�@�@:Y_Position			�`�悷��Y���W
//�@�@�@�@�@:pString			�`�悷�镶����ւ̃|�C���^���w��
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void DrawText(HWND hWnd, int X_Position, int Y_Position, const char *pString)
{
	long ReceivePosition, SendPosition;
	int Unit_Width = Moji_Width;
	int Unit_Height = Moji_Height;
	int X_DataPosition = 0, Y_DataPosition = 0;

	if (!strlen(pString))
		return;

	// �r�b�g�}�b�v�̊i�[�́A�x���W�͏㉺�t�Ȃ̂ŁA�C������
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - Unit_Height;
	Y_DataPosition = g_StandardFont.pBmpInfo->bmiHeader.biHeight - Unit_Height - Y_DataPosition;

	while (*pString)
	{
		if (0x20 <= *pString && *pString <= 0x7E)
		{
			X_DataPosition = (*pString - 0x20) * Moji_Width;
			Unit_Width = Moji_Width;
			// �������͈͓̔����̗L��
			if (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth > X_Position && 0 < X_Position + Unit_Width
				&& g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight > Y_Position && 0 < Y_Position + Unit_Height)
			{
				if (X_Position + Unit_Width > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth)
				{
					// �E�ɂ���
					// �������߂�
					Unit_Width = g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth - X_Position;
				}
				else if (X_Position <= 0)			// ���ɂ���
				{
					// �������߂�
					Unit_Width = Unit_Width + X_Position;
					// �f�[�^�ʒu�����߂�
					X_DataPosition = X_DataPosition + abs(X_Position);
					X_Position = 0;
				}
				if (g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight <= Y_Position + Unit_Height)
				{
					// ���ɂ���
					// ���������߂�
					Unit_Height = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position;
				}
				else if (Y_Position <= 0)
				{
					// ���������߂�
					Unit_Height = Y_Position + Unit_Height;
					// �f�[�^�ʒu�����߂�
					Y_DataPosition = Y_DataPosition + abs(Y_Position);
					Y_Position = 0; // ��ɂ���
				}
				// �������[���̍��W�v�Z
				ReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);
				SendPosition = X_DataPosition + Y_DataPosition * (g_StandardFont.pBmpInfo->bmiHeader.biWidth);
			}
			else
				return;
			// �w�i�Əd�ˍ��킹�ĕ`��
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
	// �摜�X�V�t���OON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//�@�֐���	:DrawText			������̕`��
//	����	:hWnd				�E�B���h�E�̃n���h��
//�@�@�@�@�@:X_Position			�`�悷��X���W					
//�@�@�@�@�@:Y_Position			�`�悷��Y���W
//�@�@�@�@�@:pString			�`�悷�镶����ւ̃|�C���^���w��
//			:Color				�����̐F
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void DrawText(HWND hWnd, int X_Position, int Y_Position, const char *pString, COLORREF Color)
{
	HDC hDC, hMemDC;								// �c�b�̊��t�ϐ�
	HGDIOBJ hGDIObj;							// �c�b�J���p�|�C���^

	// �f�o�C�X�R���e�L�X�g�n���h���̎擾
	hDC = GetDC(hWnd);
	// �������f�o�C�X�R���e�L�X�g�n���h���̍쐬
	hMemDC = CreateCompatibleDC(hDC);
	// DIB�Z�N�V�������f�o�C�X�R���e�L�X�g�n���h���ɑI��
	hGDIObj = (HBITMAP)SelectObject(hMemDC, g_DIBSection);
	// �t�H���g���f�o�C�X�R���e�L�X�g�n���h���ɑI��
	// �I�u�W�F�N�g�̑I��
	SelectObject(hMemDC,			// �c�b�̃n���h�����w��
		g_hFont);					// �I�u�W�F�N�g�̃n���h�����w��
	// �F�̐ݒ�
	SetTextColor(hMemDC, Color);
	// �w�i�𓧖���
	SetBkMode(hMemDC, TRANSPARENT);
	// ������̕`��
	TextOut(hMemDC, X_Position, Y_Position, pString, lstrlen(pString));
	SelectObject(hMemDC, hGDIObj);				// �I�u�W�F�N�g�̑I��
	// �f�o�C�X�R���e�L�X�g�n���h���̊J��
	DeleteDC(hMemDC);							// �I�u�W�F�N�g�̊J��
	ReleaseDC(hWnd, hDC);						// �c�b�����
	// �摜�X�V�t���OON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//�@�֐���	:DrawPoint			�_�̕`��
//	����	:hWnd				�E�C���h�E�̃n���h��
//�@�@�@�@�@:X_Position			�`�悷��X���W					
//�@�@�@�@�@:Y_Position			�`�悷��Y���W
//			:Color				�F
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void DrawPoint(HWND hWnd, int X_Position, int Y_Position, COLORREF Color)
{
	long lReceivePosition;

	// �`����W���E�C���h�E���W�ɓ����Ă��邩�̊m�F
	if (X_Position < 0 || X_Position > g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth
		|| Y_Position < 0 || Y_Position > g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight)
		return;

	// �r�b�g�}�b�v�̊i�[�́A�x���W�͏㉺�t�Ȃ̂ŁA�C������
	Y_Position = g_pMemoryImageInfo_MainWindow->bmiHeader.biHeight - Y_Position - 1;

	// �������[���̍��W�v�Z
	lReceivePosition = X_Position + Y_Position * (g_pMemoryImageInfo_MainWindow->bmiHeader.biWidth);

	// �������͈͓̔����̗L��
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

	// �摜�X�V�t���OON
	if (!g_UpdateWindow)
		InterlockedExchange(&g_UpdateWindow, 1);
}
// ------------------------------------------------------------------------
//�@�֐���	:DrawLine			���̕`��
//	����	:hWnd				�E�C���h�E�̃n���h��
//�@�@�@�@�@:X_StartPosition	�`����J�n����X���W					
//�@�@�@�@�@:Y_StartPosition	�`����J�n����Y���W
//�@�@�@�@�@:X_EndPosition		�`����I������X���W					
//�@�@�@�@�@:Y_EndPosition		�`����I������Y���W
//�@�@�@�@�@:Color				�F
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void DrawLine(HWND hWnd, int X_StartPosition, int Y_StartPosition, int X_EndPosition, int Y_EndPosition, COLORREF Color)
{
	float Katamuki = 0.0f;				// �X���ۑ��p
	int X_Size, Y_Size, Y_Pos;			// X���W�̑��Βl,Y���W�̑��Βl,Y�ؕ�
	int X1_Clip = 0, Y1_Clip = 0;		// �N���b�s���O�����p
	int X2_Clip = Window_Width - 1;		// �N���b�s���O�����p
	int Y2_Clip = Window_Height - 1;	// �N���b�s���O�����p
	BYTE Clip1 = 0x00, Clip2 = 0x00;	// �N���b�s���O�����p
	int x, y, i, j = 0;

	// ���Βl�����߂�
	X_Size = X_EndPosition - X_StartPosition;
	Y_Size = Y_EndPosition - Y_StartPosition;

	// �X�������߂�
	if (X_Size == 0)		// �X����������̏���
		X_Size = -1;		// ��������
	else if (Y_Size != 0)	// �X���𕁒ʂɋ��߂�
		Katamuki = (float)Y_Size / (float)X_Size;
	else
		Katamuki = 0.0f;	// �X��������؂�Ȃ��̂�

	if ((X_StartPosition > X_EndPosition && -1.0f <= Katamuki && Katamuki <= 1.0f)
		|| ((Y_StartPosition > Y_EndPosition && Katamuki < -1.0f) || (Y_StartPosition > Y_EndPosition && Katamuki > 1.0f)))
	{
		// �Q�̍��W����������
		j = X_StartPosition;
		X_StartPosition = X_EndPosition;
		X_EndPosition = j;
		j = Y_StartPosition;
		Y_StartPosition = Y_EndPosition;
		Y_EndPosition = j;
	}

	// Y�ؕЂ����߂�
	if (X_Size != -1)
		Y_Pos = (int)((float)Y_StartPosition - Katamuki * (float)X_StartPosition + 0.5f);

	// �_�̍��W�ʒu�m�F
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

	// �N���b�s���O����
	if ((Clip1 & Clip2) != 0x00)		// ��ʊO
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

	// ��������
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
