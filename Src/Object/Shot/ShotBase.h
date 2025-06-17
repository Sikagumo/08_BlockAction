#pragma once
#include <DxLib.h>

class ShotBase
{
public:

	// 弾の種類
	enum class TYPE
	{
		STRAIGHT, // 直進弾
		ROCK	  // 石
	};

	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="baseModelId">元となるモデルのハンドルID</param>
	ShotBase(TYPE type, int baseModelId);

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	ShotBase(void);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	virtual ~ShotBase(void) = default;


	/// <summary>
	/// 弾の生成
	/// </summary>
	/// <param name="pos">表示開始座標</param>
	/// <param name="dir">弾の進行方向</param>
	void CreateShot(VECTOR pos, VECTOR dir);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// メモリ解放処理
	/// </summary>
	void Release(void);



	/// <summary>
	/// 生存判定の取得
	/// </summary>
	bool GetIsAlive(void);

	/// <summary>
	/// 弾の座標の取得
	/// </summary>
	VECTOR GetPos(void);

	/// <summary>
	/// 弾の衝突判定用半径の取得
	/// </summary>
	float GetColRad(void);

	/// <summary>
	/// 弾の種別の取得
	/// </summary>
	TYPE GetType(void);

	/// <summary>
	/// 生存判定の割り当て
	/// </summary>
	void SetIsAlive(bool isAlive);


protected:

	// 弾の種別
	TYPE type_;

	// 弾のモデルID
	int modelId_;

	// 方向
	VECTOR dir_;

	// 弾の大きさ
	VECTOR scales_;

	// 弾の座標
	VECTOR pos_;

	// 弾の移動速度
	float speed_;

	// 弾の生存判定
	bool isAlive_;

	// 弾の生存期間
	float aliveTime_;

	// 弾の衝突判定用半径
	float collisionRadius_;


	/// <summary>
	/// パラメータ設定
	/// </summary>
	virtual void SetParam(void) = 0;

	/// <summary>
	/// 弾の生存期間の減少処理
	/// </summary>
	void ReduceAliveTime(void);
};