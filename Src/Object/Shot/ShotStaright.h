#pragma once
#include "./ShotBase.h"

class ShotStaright : public ShotBase
{
public:

	// スケール
	static constexpr float SCALE = 0.4f;

	// 速度
	static constexpr float SPEED = 6.5f;

	// 生存時間
	static constexpr float TIME_ALIVE = 5.0f;

	// 半径
	static constexpr float RADIUS = 30.0f;


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	ShotStaright(void);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">弾の種類</param>
	/// <param name="baseModelId">弾のモデルハンドル</param>
	ShotStaright(ShotBase::TYPE type, int baseModelId);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~ShotStaright(void)override = default;


protected:

	/// <summary>
	/// パラメータ割り当て
	/// </summary>
	void SetParam(void)override;
};