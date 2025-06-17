#pragma once
#include "EnemyBase.h"
class Player;


class Enemy_Wizard : public EnemyBase
{
public:

	// 最大HP
	static constexpr int MAX_HP = 2-2;

	// サイズ
	static constexpr float SCALE = 0.7f;

	// 生成位置の比率
	static constexpr float SPAWN_RAMGE = 0.75f;

	// 攻撃間隔
	static constexpr float ATTACK_TERM = 2.25f;
	
	// 攻撃有効時間
	static constexpr float ATTACK_ATIVE = 0.285f;

	// 半径の大きさ
	static constexpr float RADIUS = 40.0f;


	Enemy_Wizard(void);

	~Enemy_Wizard(void) = default;

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

	// 状態別更新
	void UpdateStandby(void)override;
	void UpdateAttack(void)override;
};