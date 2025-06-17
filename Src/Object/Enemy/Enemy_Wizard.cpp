#include "Enemy_Wizard.h"
#include <DxLib.h>
#include <string>
#include "../Player/Player.h"
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../AnimationController.h"
#include "../../Manager/SceneManager.h"
#include "../Shot/ShotBase.h"



Enemy_Wizard::Enemy_Wizard(void)
{
	isMove_ = false;

	isShot_ = false;
}


void Enemy_Wizard::SetParam(void)
{
	// サイズ割り当て
	scales_ = { SCALE, SCALE, SCALE };

	// 最大HP割り当て
	hp_ = MAX_HP;

	// 生成位置の比率割り当て
	spawnRange_ = SPAWN_RAMGE;

	// 攻撃頻度割り当て
	attackTimeWait_ = ATTACK_TERM;

	// 攻撃有効までの時間割り当て
	attackTimeAct_ = ATTACK_TERM;

	// 移動速度
	speed_ = 0.0f;

	// 半径割り当て
	colRad_ = RADIUS;
}

void Enemy_Wizard::ChangeStandby(void)
{
	// 状態遷移
	ChangeState(STATE::STANDBY);

	// 攻撃間隔初期化
	attackTimeWait_ = ATTACK_TERM;

	// 待機アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

}

void Enemy_Wizard::ChangeAttack(void)
{
	// 状態遷移
	ChangeState(STATE::ATTACK);

	// 攻撃アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);

	attackTimeAct_ = ATTACK_ATIVE;

	// 発射有効化
	isShot_ = false;
}

void Enemy_Wizard::UpdateStandby(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	attackTimeWait_ -= delta;

	if (attackTimeWait_ < 0.0f)
	{
		// 攻撃状態に遷移
		ChangeAttack();
	}

	// プレイヤー向き追尾
	LookPlayerRotate();
}

void Enemy_Wizard::UpdateAttack(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	if (!isShot_)
	{
		attackTimeAct_ -= delta;

		if (attackTimeAct_ < 0.0f)
		{
			isShot_ = true;

			// 有効な弾を取得
			ShotBase* shot = GetValidShot();

			// 弾を生成
			shot->CreateShot(pos_, dir_);
		}
	}

	if (animController_->IsEnd())
	{
		// アニメーション終了時、状態遷移
		ChangeStandby();
	}
}