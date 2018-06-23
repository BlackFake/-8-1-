// --------------------------------------------------
//�@DrawSample Ver0.50
//	WinSystem.h				Programing By N.Sonoda
// --------------------------------------------------
#ifndef __WINSYSTEM_H__
#define __WINSYSTEM_H__
// -------------------- Define�錾 ------------------
#define Moji_Width					16							// ��{�����̕�(�����`��p)
#define Moji_Height					24							// ��{�����̍���(�����`��p)
#define Moji_Count					95							// ��{�����̍쐬��(�����`��p)
#define DELETEOBJECT(x)	if(x) {DeleteObject(x);x=NULL;}			// �I�u�W�F�N�g�J���}�N��
// -------------------- IncludeFile -----------------
#include "Global.h"
#include "resource.h"
// --------------- �O���[�o���ϐ��錾 ---------------
volatile HANDLE g_hMainEvent;									// �C�x���g�p�n���h��(���C���p)
HANDLE g_hMainThread;											// �X���b�h�p�n���h��(���C���p)
volatile HANDLE g_hMaskEvent;									// �C�x���g�p�n���h��(�}�X�N�C���[�W�쐬�p)
HANDLE g_hMaskThread;											// �X���b�h�p�n���h��(�}�X�N�C���[�W�쐬�p)
HANDLE hMutex = NULL;											// �~���[�e�b�N�X�p�n���h��
DWORD g_Time = 0;												// �ĕ`��܂ł̎���
LONG g_Main_Time = 1;											// ���C�����[�v�̎��Ԃ��A�ݒ莞�Ԃ𖞂����Ă��邩�̗L��
LONG g_MainLoop = 1;											// WM_DESTROY���s��(�I������)�A���C�����[�v�����s���Ȃ����߂̃t���O
LONG g_UpdateWindow = 0;										// WM_PAINT�̌Ăяo���̊m�F�t���O
HINSTANCE g_hInst;												// �C���X�^���X�ۑ��p
BOOL g_MainCreate_Flag = FALSE;									// ���C���E�C���h�E�̏����ݒ�L��
// ���C���E�B���h�E�֘A
LPBITMAPINFO g_pMemoryImageInfo_MainWindow;						// �r�b�g�}�b�v��񂪊i�[����Ă���擪�A�h���X
LPBYTE g_pMemoryImage_MainWindow;								// �r�b�g�}�b�v�C���[�W�f�[�^���i�[����Ă���擪�A�h���X(���C���E�C���h�E)
LPBYTE g_pMemoryImageBuffer_MainWindow = NULL;					// ��L�Q�̃f�[�^���i�[(���C���E�C���h�E)
BOOL g_bBitMapLoadSuccess_Flag;									// �r�b�g�}�b�v�̓ǂݍ��ݐ���/���s
typedef struct tgaRGB
{
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
}COLOR_RGB;
HBITMAP g_DIBSection = NULL;		// DIBSection���g���ĕ`����s���r�b�g�}�b�v�̈�
typedef struct _tagBMP_
{
	LPBITMAPINFO	pBmpInfo;		// �r�b�g�}�b�v��񂪊i�[����Ă���擪�A�h���X
	LPBYTE			pImage;			// �r�b�g�}�b�v�C���[�W�f�[�^���i�[����Ă���擪�A�h���X
	LPBYTE			pBuffer;		// ��L�Q�̃f�[�^���i�[
	LPBITMAPINFO	pBmpInfo_Mask;	// �r�b�g�}�b�v��񂪊i�[����Ă���擪�A�h���X(�}�X�N�p�^�[��)
	LPBYTE			pImage_Mask;	// �r�b�g�}�b�v�C���[�W�f�[�^���i�[����Ă���擪�A�h���X(�}�X�N�p�^�[��)
	LPBYTE			pBuffer_Mask;	// ��L�Q�̃f�[�^���i�[(�}�X�N�p�^�[��)
}Bmp;
Bmp g_StandardFont;												// �W���t�H���g
Bmp* g_pBmp = NULL;												// �r�b�g�}�b�v���ۑ��p
// �t�H���g�֘A
HFONT g_hFont = NULL;											// �t�H���g�X�^�C���ۑ��p
HWND g_hWnd = NULL;
// ----------------- ���[�U�[�ύXOK -----------------
#define Resource_Count				1							// �ǂݍ��ރ��\�[�X�̐�
int g_iFontHeight = 16;											// �t�H���g�̍���
int g_iFontWidth = 12;											// ���ϕ�����
static char* g_szResourceID[] = 								// ���\�[�X�̖��̂�o�^
{
	"IDB_BITMAP1"
};
// -------------------- �֐��錾 --------------------
BOOL MainRegisterClass(HINSTANCE);								// ���C���E�C���h�E�N���X��o�^
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// �E�C���h�E�̃v���V�[�W��
BOOL Wm_DestroyProc(HWND);										// �I������
void TimerThread(HWND);											// ���C�����[�v�̎��Ԃ����ɕۂ�
BOOL LoadBitMap(HWND, UINT);										// �r�b�g�}�b�v�̓ǂݍ���
void DrawInit(HWND);											// �摜�����֘A�̏�������
void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short);		// �r�b�g�}�b�v�֘A�̕`��
void DrawBitMap(HWND, int, int, int, int, int, int, UINT, short, float);	// �r�b�g�}�b�v�֘A�̕`��
void CreateMaskThread();										// �d�ˍ��킹�p�C���[�W�摜�쐬
void ClearScreen(HWND);											// ��ʂ����F�œh��Ԃ�
void CreateFont(HWND);											// �t�H���g�̍쐬
void DrawText(HWND, int, int, const char*);						// ������̕`��(��{����)
void DrawText(HWND, int, int, const char*, COLORREF);				// ������̕`��
void DrawPoint(HWND, int, int, COLORREF);							// �_�̕`��
void DrawLine(HWND, int, int, int, int, COLORREF);					// ���̕`��
void CreateDIB(HWND);
// --------------------------------------------------
#endif	/* __WINSYSTEM_H__ */