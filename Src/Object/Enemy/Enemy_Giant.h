#pragma once
#pragma once
#include "EnemyBase.h"
class Player;


class Enemy_Giant : public EnemyBase
{
public:

	// 最大HP
	static constexpr int MAX_HP = 5;

	// サイズ
	static constexpr float SCALE = 1.85f;

	// 移動速度
	static constexpr float SPEED = 2.25f;

	// 生成位置の比率
	static constexpr float SPAWN_RAMGE = 0.75f;

	// 攻撃間隔
	static constexpr float ATTACK_TERM = 1.0f;

	// 攻撃有効時間
	static constexpr float ATTACK_ATIVE = 0.185f;

	// 半径の大きさ
	static constexpr float RADIUS = 300.0f;

	// 縦発射位置調整
	static constexpr float SHOT_Y_OFFSET = 50.0f;

	// 被ダメージ時の最大硬直時間
	static constexpr float DAMAGE_MAX_TIME = 1.5f;


	Enemy_Giant(void);

	~Enemy_Giant(void) = default;

	// こちらでもよい
	//virtual ~Enemy_Wizard(void) = default;


protected:

	// 発射フラグ
	bool isShot_;

	/// <summary>
	/// パラメータ割り当て
	/// </summary>
	void SetParam(void)override;


	// 状態遷移
	void ChangeStandby(void)override;

	void ChangeAttack(void)override;

	void ChangeHitReact(void)override;

	// 状態別更新
	void UpdateStandby(void)override;
	void UpdateAttack(void)override;
	void UpdateHitReact(void)override;

	/// <summary>
	/// 弾生成処理
	/// </summary>
	/// <param name="dir">発射方向</param>
	void SetShot(const VECTOR& dir);
};