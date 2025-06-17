#pragma once
#include "./ShotBase.h"

class ShotStaright : public ShotBase
{
public:

	// スケール
	static constexpr float SCALE = 0.4f;

	// 速度
	static constexpr float SPEED = 8.0f;

	// 生存時間
	static constexpr float TIME_ALIVE = 5.0f;

	// 半径
	static constexpr float RADIUS = 30.0f;

	ShotStaright(void);

	ShotStaright(TYPE type, int baseModelId);

	~ShotStaright(void)override = default;


protected:

	void SetParam(void)override;
};