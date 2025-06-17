#pragma once
#include "../../Application.h"
#include "WeaponBase.h"
#include <string>

class WeaponPunch : public WeaponBase
{
public:

	// ドリル画像
	const std::string PATH_DRILL_X = (Application::PATH_IMAGE + "DrillX.png");
	const std::string PATH_DRILL_Y = (Application::PATH_IMAGE + "DrillY.png");


	// エフェクト画像の分割数
	static constexpr int NUM_SPRITE_X = 4;
	static constexpr int NUM_SPRITE_Y = 3;
	static constexpr int NUM_SPRITE_ALL = NUM_SPRITE_X * NUM_SPRITE_Y;

	// エフェクト画像の分割された大きさ(X)
	static constexpr int SIZE_XSPRITE_X = 100;
	static constexpr int SIZE_XSPRITE_Y = 80;

	// エフェクト画像の分割された大きさ(Y)
	static constexpr int SIZE_YSPRITE_X = 80;
	static constexpr int SIZE_YSPRITE_Y = 100;

	// モデルの大きさ
	static constexpr float MODEL_SCALE = 1.0f;

	// モデルのローカル座標
	static constexpr VECTOR MODEL_LOCAL_POS = {0.0f, 90.0f, 0.0f};

	// 移動速度
	static constexpr float MOVE_SPEED = 12.5f;

	// 当たり判定の半径
	static constexpr float COLLISION_RADIUS = 100.0f;

	// 画像の大きさ倍率
	static constexpr float IMG_SCALE = 280.0f;

	
	// コンストラクタ
	WeaponPunch(const VECTOR& pos);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~WeaponPunch(void) = default;

	void Draw(void) override;

	void Release(void) override;

	// 武器を使用する
	void Use(VECTOR pos, VECTOR dir) override;


protected:

	// 画像やモデルなどのロード
	void Load(void) override;

	// パラメータ設定
	void SetParam(void) override;


private:

	// アニメーションカウンタ
	int cntAnimation_;

	// エフェクト画像(横)
	int imgsX_[NUM_SPRITE_ALL];

	// エフェクト画像(縦)
	int imgsZ_[NUM_SPRITE_ALL];
};