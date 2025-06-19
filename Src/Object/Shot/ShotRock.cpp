#include "ShotRock.h"
#include "./ShotBase.h"
#include "../../Scene/GameScene.h";
#include "../../Manager/SceneManager.h"


ShotRock::ShotRock(void)
{

}

ShotRock::ShotRock(ShotBase::TYPE type, int baseModelId)
{
	type_ = type;

	modelId_ = MV1DuplicateModel(baseModelId);
}


void ShotRock::SetParam(void)
{
	// スケール
	scales_ = { SCALE,SCALE,SCALE };

	// 移動速度
	speed_ = SPEED_START;

	// 生存フラグ
	isAlive_ = true;

	// 生存時間
	aliveTime_ = TIME_ALIVE;

	// 当たり判定の半径
	colRad_ = RADIUS;


	// 上方向の吹っ飛ばし力
	upPower_ = UP_POWER_START;

	// 上方向の吹っ飛ばしベクトル
	upVecY_ = upPower_;

	// 衝突フラグ
	isHit_ = false;
}


void ShotRock::ProcessMove(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	speed_ -= SPEED_DEC * delta;
	if (speed_ < 0.0f)
	{
		speed_ = 0.0f;
	}

	// 発射方向を正規化
	VECTOR movePow = VNorm(dir_);

	// 移動量の計算(方向×スピード)
	movePow = VScale(movePow, -speed_);


	if (isHit_)
	{
		isHit_ = false;

		// 吹っ飛ばし半減
		upPower_ -= UP_POWER_DEC;

		if (upPower_ < 0.0f)
		{
			upPower_ = 0.0f;
		}

		// 吹っ飛ばし再割り当て
		upVecY_ = upPower_;
	}

	// 上方向に吹っ飛ばす
	pos_.y += upVecY_;

	if (!isHit_)
	{
		// 重力加算
		upVecY_ -= GameScene::GRAVITY;
	}


	// 移動処理
	pos_ = VAdd(pos_, movePow);


	// 一定の下位置に到着時、無効化
	if (pos_.y < GameScene::POS_LIMIT_Y)
	{
		isAlive_ = false;
	}
}

void ShotRock::SetCollisionStage(const VECTOR& pos)
{
	isHit_ = true;
}