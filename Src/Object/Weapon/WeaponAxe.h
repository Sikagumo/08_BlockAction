#pragma once
#include "WeaponBase.h"
#include <DxLib.h>

class WeaponAxe : public WeaponBase
{
public:

	// モデルの大きさ
	static constexpr float MODEL_SCALE = 0.6f;

	static constexpr float MODEL_ANGLE_Y = 90.0f;

	// モデルのローカル座標
	static constexpr VECTOR MODEL_LOCAL_POS = { 0.0f, 90.0f, 0.0f };

	// 移動速度
	static constexpr float MOVE_SPEED = 12.5f;

	// 当たり判定の半径
	static constexpr float COLLISION_RADIUS = 70.0f;

	// 使用時のジャンプ力
	static constexpr float JUMP_POW = 30.0f;

	// 重力
	static constexpr float GRAVITY = 2.0f;

	WeaponAxe(const VECTOR&);

	~WeaponAxe(void) = default;

	void Draw(void) override;

	void Release(void) override;

	void Use(VECTOR pos, VECTOR dir) override;


protected:

	/// <summary>
	/// 画像やモデルのロード
	/// </summary>
	void Load(void) override;

	/// <summary>
	/// パラメータ設定
	/// </summary>
	void SetParam(void) override;

private:
	
	float jumpPow_;

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(void) override;

};