#pragma once
class Player;
#include "../../Application.h"
#include <vector>
#include <string>

class HpManager
{
public:

	struct HEART
	{
		float size; // サイズ
		int posX;
		int posY;
	};

	// HP画像
	const std::string PATH_HEART = (Application::PATH_IMAGE + "Heart.png");

	// HP画像の後ろ
	const std::string PATH_HEART_LOST = (Application::PATH_IMAGE + "HeartLost.png");


	// ＨＰ描画の開始座標(X)
	static constexpr int POS_X = 100;
	
	// ＨＰ描画の開始座標(X)
	static constexpr int POS_X_OFFSET = 60;

	// ＨＰ描画の開始座標(Y)
	static constexpr int POS_Y = 50;

	// ＨＰアイコンの描画幅
	static constexpr int HP_ICON_WIDTH = 100;

	// HPアイコンの初期倍率
	static constexpr float HEART_SIZE = 0.75f;

	// HP位置の調整値値
	static constexpr int HEART_OFFSET = 25;

	// HP減少速度
	static constexpr float HEART_SPEED_DEC = 1.0f;


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤークラス</param>
	HpManager(Player* player);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~HpManager(void) = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);


private:

	// プレイヤー情報
	Player* player_;

	std::vector<HEART> heart_;

	// ＨＰアイコン画像ハンドル
	int imgHeart_;

	// 失ったＨＰアイコン画像ハンドル
	int imgHeartLost_;
};