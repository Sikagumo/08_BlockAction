#include "Enemy_Giant.h"
#include <DxLib.h>
#include <string>
#include "../Player/Player.h"
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../AnimationController.h"
#include "../../Manager/SceneManager.h"
#include "../Shot/ShotBase.h"

Enemy_Giant::Enemy_Giant(void)
{
	isShot_ = false;
}

void Enemy_Giant::SetParam(void)
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
	speed_ = SPEED;

	// 半径割り当て
	colRad_ = RADIUS;
}

void Enemy_Giant::ChangeStandby(void)
{
	// 攻撃間隔初期化
	attackTimeWait_ = ATTACK_TERM;

	// 待機アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::WALK));
}

void Enemy_Giant::ChangeAttack(void)
{
	// 発射有効時間初期化
	attackTimeAct_ = ATTACK_ATIVE;

	// 発射有効化
	isShot_ = false;

	// 攻撃アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
}

void Enemy_Giant::ChangeHitReact(void)
{
	damageTime_ = DAMAGE_MAX_TIME;

	// 被ダメージ時アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::HIT_REACT), false);
}


void Enemy_Giant::UpdateStandby(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 移動処理
	Move();

	attackTimeWait_ -= delta;

	if (attackTimeWait_ < 0.0f)
	{
		// 攻撃状態に遷移
		ChangeState(STATE::ATTACK);
	}

	// プレイヤー向き追尾
	LookPlayerRotate();

	if (isMove_)
	{
		// 移動アニメーション再生
		//animController_->Play(static_cast<int>(ANIM_TYPE::WALK));
	}
	else
	{
		// 待機アニメーション再生
		//animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
}

void Enemy_Giant::UpdateAttack(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	if (!isShot_)
	{
		attackTimeAct_ -= delta;

		if (attackTimeAct_ < 0.0f)
		{
			isShot_ = true;

			// 正面に発射
			SetShot(dir_);
		}
	}

	if (animController_->IsEnd())
	{
		// アニメーション終了時、状態遷移
		ChangeState(STATE::STANDBY);
	}
}

void Enemy_Giant::UpdateHitReact(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 攻撃有効時間減少
	attackTimeWait_ -= delta;


	damageCnt_++;

	damageTime_ -= delta;

	// プレイヤー向き追尾
	LookPlayerRotate();

	if (damageTime_ < 0.0f &&
		animController_->IsEnd())
	{
		// 状態遷移
		ChangeState(STATE::STANDBY);
	}
}

void Enemy_Giant::SetShot(const VECTOR& dir)
{
	// 有効な弾を取得
	ShotBase* shot = GetValidShot();

	VECTOR shotPos = pos_;

	// 発射位置調整
	shotPos.y += SHOT_Y_OFFSET;

	// 弾を生成
	shot->CreateShot(shotPos, dir);
}
