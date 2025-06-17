#include "ShotStaright.h"
#include "ShotBase.h"


ShotStaright::ShotStaright(void)
{
	
}

ShotStaright::ShotStaright(TYPE type, int baseModelId)
{
	type_ = type;
	modelId_ = MV1DuplicateModel(baseModelId);
}

void ShotStaright::SetParam(void)
{
	scales_ = { SCALE,SCALE,SCALE };

	speed_ = SPEED;

	isAlive_ = true;

	aliveTime_ = TIME_ALIVE;

	collisionRadius_ = RADIUS;
}
