#include "Enemy_Demon.h"
#include <DxLib.h>
#include <string>
#include "../Player/Player.h"
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../AnimationController.h"


Enemy_Demon::Enemy_Demon(void)
{
	isMove_ = true;
}


void Enemy_Demon::SetParam(void)
{
	// サイズ割り当て
	scales_ = { SCALE, SCALE, SCALE };

	// 最大HP割り当て
	hp_ = MAX_HP;

	// 移動速度割り当て
	speed_ = SPEED;

	// 生成位置の比率割り当て
	spawnRange_ = SPAWN_RAMGE;

	// 半径割り当て
	colRad_ = RADIUS;
}

void Enemy_Demon::UpdateStandby(void)
{
	// プレイヤー追尾移動処理
	Move();

	// プレイヤー向き追尾
	LookPlayerRotate();

	if (isMove_)
	{
		// 移動アニメーション再生
		animController_->Play(static_cast<int>(ANIM_TYPE::WALK));
	}
	else
	{
		// 待機アニメーション再生
		animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
}