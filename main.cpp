// --------------------------------------------------
//�@DrawSample Ver0.50
//	Main.cpp				Programing By N.Sonoda
// --------------------------------------------------
// -------------------- IncludeFile -----------------
#include "Main.h"
// ------------------- �}�N���錾 -------------------
#define SCREEN_WIDTH			(640.0)				// �E�B���h�E�̉��T�C�Y
#define SCREEN_HEIGHT			(480.0)				// �E�B���h�E�̏c�T�C�Y
#define BALL_SIZE				(48)				// �\������]���r(���͋�)�̃T�C�Y
#define BALL_HALF_SIZE			(24)				// �\������]���r(���͋�)�̔����̃T�C�Y
#define NUM_POSITION			(8)					// �]���r(���͋�)��\�����郋�[�g�̐�
#define SPEED					(5.0)
//#define�͔z��̏��߂̒萔�ɂ��Ȃ��̂ł����const�Ő錾����̂���������
//�܂��AC++11����͂��̂悤�Ȍ��ߑł��̒l�ɑ΂��Ă�constexpr�Ƃ���������������

// -------------------- �֐��錾 --------------------
double GetDistance(int x, int y, int x2, int y2, char direction);

// -------------------- �ϐ��錾 --------------------
struct Pos {
	double x;
	double y;
};

struct Pos Pos0 = { (SCREEN_WIDTH / 2 - BALL_HALF_SIZE), (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) };// �^�񒆂̍��W��o�^����\����
struct Pos Pos1 = { 0.0, 0.0 };																		// ����̍��W��o�^����\����
struct Pos Pos2 = { (SCREEN_WIDTH / 2 - BALL_HALF_SIZE), 0 };									// �^��̍��W��o�^����\����
struct Pos Pos3 = { (SCREEN_WIDTH - BALL_SIZE), 0 };											// �E��̍��W��o�^����\����
struct Pos Pos4 = { (SCREEN_WIDTH - BALL_SIZE), (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) };			// �^�E�̍��W��o�^����\����
struct Pos Pos5 = { (SCREEN_WIDTH - BALL_SIZE), (SCREEN_HEIGHT - BALL_SIZE) };					// �E���̍��W��o�^����\����
struct Pos Pos6 = { (SCREEN_WIDTH / 2 - BALL_HALF_SIZE), (SCREEN_HEIGHT - BALL_SIZE) };			// �^���̍��W��o�^����\����
struct Pos Pos7 = { 0, (SCREEN_HEIGHT - BALL_SIZE) };											// �����̍��W��o�^����\����
struct Pos Pos8 = { 0, (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) };									// �^���̍��W��o�^����\����

																								//�\���̂̔z��ł��悩�����̂ł́H
struct Pos Pos[9] = { (SCREEN_WIDTH / 2 - BALL_HALF_SIZE), (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) ,
	0.0, 0.0,
	(SCREEN_WIDTH / 2 - BALL_HALF_SIZE), 0 ,
	(SCREEN_WIDTH - BALL_SIZE), 0,
	(SCREEN_WIDTH - BALL_SIZE), (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE),
	(SCREEN_WIDTH - BALL_SIZE), (SCREEN_HEIGHT - BALL_SIZE),
	(SCREEN_WIDTH / 2 - BALL_HALF_SIZE), (SCREEN_HEIGHT - BALL_SIZE),
	0, (SCREEN_HEIGHT - BALL_SIZE),
	0, (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) };

// ------------------------------------------------------------------------
//�@�֐���	:MainLoop			���C���v���O����	
//	����	:hWnd				�E�B���h�E�̃n���h��
//	�߂�l	:					�Ȃ�
// ------------------------------------------------------------------------
void MainLoop(HWND hWnd)
{
	// -------- ������ --------
	// -------- ���� --------

	//--- �֐����g�킸�ɍs���v���O���� ---
	/*
	double Xdis = Pos0.x - Pos1.x;						// ����̋ʂƐ^�񒆂̋ʂƂ�X���̋���
	double Ydis = Pos0.y - Pos1.y;						// ����̋ʂƐ^�񒆂̋ʂƂ�Y���̋���
	double Distance = sqrt(Xdis * Xdis + Ydis * Ydis);	// ��_�Ԃ̋���

	double Xspeed = Xdis / Distance * SPEED;				// 1�t���[���ɓ����ʂ�X���̈ړ���
	double Yspeed = Ydis / Distance * SPEED;				// 1�t���[���ɓ����ʂ�Y���̈ړ���

	Pos1.x += (int)(Xspeed);
	Pos1.y += (int)(Yspeed);
	*/

	// --- �֐���p���čs���v���O����(���������܂������܂���ł���) ---
	//Pos1.x += (int)(GetDistance(Pos1.x, Pos1.y, Pos0.x, Pos0.y, 1));
	//Pos1.y += (int)(GetDistance(Pos1.x, Pos1.y, Pos0.x, Pos0.y, 2));

	//�֐����肫�ňړ��v�Z
	constexpr double stoprange = 1.005;//����͈͂ɓ������疳����蒆�S�̋ʂƓ������W�ɂ���(1��菭���������炢�Ŏ��܂�悤�ł�)
	for (int i = 1; i < 9; i++) {
		Pos[i].x += (int)(GetDistance(Pos[i].x, Pos[i].y, Pos[0].x, Pos[0].y, 1));
		Pos[i].y += (int)(GetDistance(Pos[i].x, Pos[i].y, Pos[0].x, Pos[0].y, 2));
		if (Pos[i].x>Pos[0].x - stoprange && Pos[i].x<Pos[0].x + stoprange && Pos[i].y>Pos[0].y - stoprange && Pos[i].y<Pos[0].y + stoprange) {
			Pos[i].x = Pos[0].x;
			Pos[i].y = Pos[0].y;
		}

	}
	// -------- �`�� --------
	ClearScreen(hWnd);
	//DrawBitMap(hWnd, (int)Pos0.x, (int)Pos0.y, 48, 0, 48, 48, 0, 1);			// ���S�̋ʂ̕`��
	//DrawBitMap(hWnd, (int)Pos1.x, (int)Pos1.y, 48, 0, 48, 48, 0, 1);			// ����̋ʂ̕`��
	//DrawBitMap(hWnd, Pos2.x, Pos2.y, (48 * 2), 0, 48, 48, 0, 1);	// �^��̋ʂ̕`��
	//DrawBitMap(hWnd, Pos3.x, Pos3.y, (48 * 3), 0, 48, 48, 0, 1);	// �E��̋ʂ̕`��
	//DrawBitMap(hWnd, Pos4.x, Pos4.y, (48 * 4), 0, 48, 48, 0, 1);	// �^�E�̋ʂ̕`��
	//DrawBitMap(hWnd, Pos5.x, Pos5.y, (48 * 5), 0, 48, 48, 0, 1);	// �E���̋ʂ̕`��
	//DrawBitMap(hWnd, Pos6.x, Pos6.y, 48 , 0, 48, 48, 0, 1);		// �^���̋ʂ̕`��
	//DrawBitMap(hWnd, Pos7.x, Pos7.y, (48 * 2), 0, 48, 48, 0, 1);	// �����̋ʂ̕`��
	//DrawBitMap(hWnd, Pos8.x, Pos8.y, (48 * 3), 0, 48, 48, 0, 1);	// �^���̋ʂ̕`��

	//�S�ʂ̕\��
	for (int i = 0; i < 9; i++) DrawBitMap(hWnd, (int)Pos[i].x, (int)Pos[i].y, 48, 0, 48, 48, 0, 1);

}

double GetDistance(int x, int y, int x2, int y2, char direction) {

	double Xdis = x2 - x;									// ��_�Ԃ�X���̋���
	double Ydis = y2 - y;									// ��_�Ԃ�Y���̋���
	double Distance = sqrt(Xdis * Xdis + Ydis * Ydis);		// ��_�Ԃ̋���

															//�ꕔ�̋ʂ͎l�̌ܓ��ō��W�����
	double Xspeed = Xdis / Distance * SPEED/*+0.5*/;				// 1�t���[���ɓ����ʂ�X���̈ړ���
	double Yspeed = Ydis / Distance * SPEED/*+0.5*/;				// 1�t���[���ɓ����ʂ�Y���̈ړ���

	if (direction == 1)
		return Xspeed;										// X���̈ړ��ʂ̒l��Ԃ�
	if (direction == 2)
		return Yspeed;										// Y���̈ړ��ʂ̒l��Ԃ�
}