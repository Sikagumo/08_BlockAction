#pragma once
class Player;
#include "../../Application.h"
#include <vector>
#include <string>

class AttackManager
{
public:

	// パンチ画像
	const std::string PATH_PUNCH = (Application::PATH_IMAGE + "Punch.png");

	// 斧画像の後ろ
	const std::string PATH_AXE = (Application::PATH_IMAGE + "Axe.png");


	// ＨＰ描画の開始座標(X)
	static constexpr int POS_X = 5;

	// ＨＰ描画の開始座標(Y)
	static constexpr int POS_Y = 5;

	// ＨＰアイコンの描画幅
	static constexpr int HP_ICON_WIDTH = 100;

	// HPアイコンの初期倍率
	static constexpr float SIZE = 0.75f;

	// HP位置の調整値値
	static constexpr int OFFSET = 25;


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤークラス</param>
	AttackManager(Player* player);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~AttackManager(void) = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);


private:

	// プレイヤー情報
	Player* player_;

	// パンチ画像ハンドル
	int imgPunch_;

	// 斧画像ハンドル
	int imgAxe_;
};