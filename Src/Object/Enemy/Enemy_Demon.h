#pragma once
#include "EnemyBase.h"
class Player;


class Enemy_Demon : public EnemyBase
{
public:

	// 最大HP
	static constexpr int MAX_HP = 3;

	// 移動速度
	static constexpr float SPEED = 3.75f;

	// サイズ
	static constexpr float SCALE = 0.75f;

	// 生成位置の比率
	static constexpr float SPAWN_RAMGE = 1.0f;

	// 半径の大きさ
	static constexpr float RADIUS = 50.0f;


	Enemy_Demon(void);

	~Enemy_Demon(void) = default;


protected:

	/// <summary>
	/// パラメータ割り当て
	/// </summary>
	void SetParam(void)override;

	// 状態別更新
	void UpdateStandby(void)override;
};