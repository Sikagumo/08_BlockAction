#pragma once
#include "./ShotBase.h"

class ShotRock : public ShotBase
{
public:

	// スケール
	static constexpr float SCALE = 3.5f;

	// 速度
	static constexpr float SPEED_START = 15.0f;

	// 速度減少値
	static constexpr float SPEED_DEC = 4.0f;

	// 生存時間
	static constexpr float TIME_ALIVE = 4.5f;

	// 半径
	static constexpr float RADIUS = 275.0f;

	// 上方向の吹っ飛ばし力
	static constexpr float UP_POWER_START = 23.0f;

	// 吹っ飛ばし減少値
	static constexpr float UP_POWER_DEC = 5.0f;


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	ShotRock(void);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">弾の種類</param>
	/// <param name="baseModelId">弾のモデルハンドル</param>
	ShotRock(ShotBase::TYPE type, int baseModelId);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~ShotRock(void)override = default;


	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="pos"></param>
	void SetCollisionStage(const VECTOR& pos)override;


protected:


	// 衝突したか否かの判定
	bool isHit_;

	// 上方向の吹っ飛ばしベクトル
	float upVecY_;

	// 上方向の吹っ飛ばし値
	float upPower_;


	/// <summary>
	/// パラメータ割り当て
	/// </summary>
	void SetParam(void)override;

	/// <summary>
	/// 移動処理
	/// </summary>
	void ProcessMove(void)override;
};