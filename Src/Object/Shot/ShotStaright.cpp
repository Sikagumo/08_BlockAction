#include "ShotStaright.h"
#include "./ShotBase.h"


ShotStaright::ShotStaright(void)
{
	
}

ShotStaright::ShotStaright(ShotBase::TYPE type, int baseModelId)
{
	type_ = type;

	modelId_ = MV1DuplicateModel(baseModelId);
}


void ShotStaright::SetParam(void)
{
	// スケール
	scales_ = { SCALE,SCALE,SCALE };

	// 移動速度
	speed_ = SPEED;

	// 生存フラグ
	isAlive_ = true;

	// 生存時間
	aliveTime_ = TIME_ALIVE;

	// 当たり判定の半径
	colRad_ = RADIUS;
}