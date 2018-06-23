// --------------------------------------------------
//　DrawSample Ver0.50
//	Main.cpp				Programing By N.Sonoda
// --------------------------------------------------
// -------------------- IncludeFile -----------------
#include "Main.h"
// ------------------- マクロ宣言 -------------------
#define SCREEN_WIDTH			(640.0)				// ウィンドウの横サイズ
#define SCREEN_HEIGHT			(480.0)				// ウィンドウの縦サイズ
#define BALL_SIZE				(48)				// 表示するゾンビ(今は玉)のサイズ
#define BALL_HALF_SIZE			(24)				// 表示するゾンビ(今は玉)の半分のサイズ
#define NUM_POSITION			(8)					// ゾンビ(今は玉)を表示するルートの数
#define SPEED					(5.0)
//#defineは配列の初めの定数にしないのであればconstで宣言するのがおすすめ
//また、C++11からはこのような決め打ちの値に対してはconstexprという書き方もある

// -------------------- 関数宣言 --------------------
double GetDistance(int x, int y, int x2, int y2, char direction);

// -------------------- 変数宣言 --------------------
struct Pos {
	double x;
	double y;
};

struct Pos Pos0 = { (SCREEN_WIDTH / 2 - BALL_HALF_SIZE), (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) };// 真ん中の座標を登録する構造体
struct Pos Pos1 = { 0.0, 0.0 };																		// 左上の座標を登録する構造体
struct Pos Pos2 = { (SCREEN_WIDTH / 2 - BALL_HALF_SIZE), 0 };									// 真上の座標を登録する構造体
struct Pos Pos3 = { (SCREEN_WIDTH - BALL_SIZE), 0 };											// 右上の座標を登録する構造体
struct Pos Pos4 = { (SCREEN_WIDTH - BALL_SIZE), (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) };			// 真右の座標を登録する構造体
struct Pos Pos5 = { (SCREEN_WIDTH - BALL_SIZE), (SCREEN_HEIGHT - BALL_SIZE) };					// 右下の座標を登録する構造体
struct Pos Pos6 = { (SCREEN_WIDTH / 2 - BALL_HALF_SIZE), (SCREEN_HEIGHT - BALL_SIZE) };			// 真下の座標を登録する構造体
struct Pos Pos7 = { 0, (SCREEN_HEIGHT - BALL_SIZE) };											// 左下の座標を登録する構造体
struct Pos Pos8 = { 0, (SCREEN_HEIGHT / 2 - BALL_HALF_SIZE) };									// 真左の座標を登録する構造体

																								//構造体の配列でもよかったのでは？
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
//　関数名	:MainLoop			メインプログラム	
//	引数	:hWnd				ウィンドウのハンドル
//	戻り値	:					なし
// ------------------------------------------------------------------------
void MainLoop(HWND hWnd)
{
	// -------- 初期化 --------
	// -------- 制御 --------

	//--- 関数を使わずに行うプログラム ---
	/*
	double Xdis = Pos0.x - Pos1.x;						// 左上の玉と真ん中の玉とのX軸の距離
	double Ydis = Pos0.y - Pos1.y;						// 左上の玉と真ん中の玉とのY軸の距離
	double Distance = sqrt(Xdis * Xdis + Ydis * Ydis);	// 二点間の距離

	double Xspeed = Xdis / Distance * SPEED;				// 1フレームに動く玉のX軸の移動量
	double Yspeed = Ydis / Distance * SPEED;				// 1フレームに動く玉のY軸の移動量

	Pos1.x += (int)(Xspeed);
	Pos1.y += (int)(Yspeed);
	*/

	// --- 関数を用いて行うプログラム(ここがうまくいきませんでした) ---
	//Pos1.x += (int)(GetDistance(Pos1.x, Pos1.y, Pos0.x, Pos0.y, 1));
	//Pos1.y += (int)(GetDistance(Pos1.x, Pos1.y, Pos0.x, Pos0.y, 2));

	//関数ありきで移動計算
	constexpr double stoprange = 1.005;//ある範囲に入ったら無理やり中心の玉と同じ座標にする(1より少し多いくらいで収まるようです)
	for (int i = 1; i < 9; i++) {
		Pos[i].x += (int)(GetDistance(Pos[i].x, Pos[i].y, Pos[0].x, Pos[0].y, 1));
		Pos[i].y += (int)(GetDistance(Pos[i].x, Pos[i].y, Pos[0].x, Pos[0].y, 2));
		if (Pos[i].x>Pos[0].x - stoprange && Pos[i].x<Pos[0].x + stoprange && Pos[i].y>Pos[0].y - stoprange && Pos[i].y<Pos[0].y + stoprange) {
			Pos[i].x = Pos[0].x;
			Pos[i].y = Pos[0].y;
		}

	}
	// -------- 描画 --------
	ClearScreen(hWnd);
	//DrawBitMap(hWnd, (int)Pos0.x, (int)Pos0.y, 48, 0, 48, 48, 0, 1);			// 中心の玉の描画
	//DrawBitMap(hWnd, (int)Pos1.x, (int)Pos1.y, 48, 0, 48, 48, 0, 1);			// 左上の玉の描画
	//DrawBitMap(hWnd, Pos2.x, Pos2.y, (48 * 2), 0, 48, 48, 0, 1);	// 真上の玉の描画
	//DrawBitMap(hWnd, Pos3.x, Pos3.y, (48 * 3), 0, 48, 48, 0, 1);	// 右上の玉の描画
	//DrawBitMap(hWnd, Pos4.x, Pos4.y, (48 * 4), 0, 48, 48, 0, 1);	// 真右の玉の描画
	//DrawBitMap(hWnd, Pos5.x, Pos5.y, (48 * 5), 0, 48, 48, 0, 1);	// 右下の玉の描画
	//DrawBitMap(hWnd, Pos6.x, Pos6.y, 48 , 0, 48, 48, 0, 1);		// 真下の玉の描画
	//DrawBitMap(hWnd, Pos7.x, Pos7.y, (48 * 2), 0, 48, 48, 0, 1);	// 左下の玉の描画
	//DrawBitMap(hWnd, Pos8.x, Pos8.y, (48 * 3), 0, 48, 48, 0, 1);	// 真左の玉の描画

	//全玉の表示
	for (int i = 0; i < 9; i++) DrawBitMap(hWnd, (int)Pos[i].x, (int)Pos[i].y, 48, 0, 48, 48, 0, 1);

}

double GetDistance(int x, int y, int x2, int y2, char direction) {

	double Xdis = x2 - x;									// 二点間のX軸の距離
	double Ydis = y2 - y;									// 二点間のY軸の距離
	double Distance = sqrt(Xdis * Xdis + Ydis * Ydis);		// 二点間の距離

															//一部の玉は四捨五入で座標が飛ぶ
	double Xspeed = Xdis / Distance * SPEED/*+0.5*/;				// 1フレームに動く玉のX軸の移動量
	double Yspeed = Ydis / Distance * SPEED/*+0.5*/;				// 1フレームに動く玉のY軸の移動量

	if (direction == 1)
		return Xspeed;										// X軸の移動量の値を返す
	if (direction == 2)
		return Yspeed;										// Y軸の移動量の値を返す
}