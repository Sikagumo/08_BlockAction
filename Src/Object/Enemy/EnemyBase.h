#pragma once
#include <DxLib.h>
#include <vector>
class Player;
class AnimationController;
class ShotBase;


class EnemyBase
{
public:

	// 敵識別の状態
	enum class TYPE
	{
		DEMON,	// デーモン
		WIZARD, // ウィザード
		GIANT,	// ジャイアント
		MAX
	};

	// 状態
	enum class STATE
	{
		NONE,
		STANDBY, // 待機
		ATTACK,	 // 攻撃
		HIT_REACT,
		DEAD_REACT,
		END,
	};


	/// <summary>
	///  アニメーション種類
	/// </summary>
	enum class ANIM_TYPE
	{
		ATTACK = 0,
		DEATH,
		HIT_REACT,
		IDLE,
		JUMP,
		RUN,
		WALK,

		MAX,
	};

	// アニメーション速度
	static constexpr float SPEED_ANIM = 30.0f;

	// 標準の自己発行
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.6f, 0.6f, 0.6f, 1.0f };

	// 移動可能なプレイヤーとの長さ
	static constexpr float MOVE_LEN_PLAYER = 5.0f;

	// 弾丸配列の要素数
	static constexpr int SHOT_LENGTH_MAX = 5;

	// Y軸の追尾する限界
	static constexpr float LOOK_TARGET_MIN_Y = -101.0f;


	// デフォルトの色
	static constexpr COLOR_F COLOR_DEFAULT = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 被ダメージ時の色
	static constexpr COLOR_F COLOR_DAMAGE = { 1.0f, 0.0f, 0.0f, 1.0f };

	// ダメージ点滅の間隔
	static constexpr int DAMAGE_TERM = 5;

	// 被ダメージ時の最大硬直時間
	static constexpr float DAMAGE_MAX_TIME = 2.5f;


	static constexpr float DEAD_MAX_TIME = 1.0f;


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	EnemyBase(void);

	virtual ~EnemyBase(void) = default;


	void Init(TYPE type, int baseModel, int baseAtkEffectModel,
			  Player& player);

	void Update(void);

	void Draw(void);

	void Release(void);


	/// <summary>
	/// 現在位置取得
	/// </summary>
	VECTOR GetPos(void) const;

	/// <summary>
	/// 半径取得
	/// </summary>
	float GetColRad(void) const;

	/// <summary>
	/// 衝突判定があるか否かの判定
	/// </summary>
	bool GetIsCollision(void) const;

	/// <summary>
	/// 生存しているか否かの判定
	/// </summary>
	bool GetIsActive(void) const;


	// 状態遷移
	void ChangeState(STATE state);

	/// <summary>
	/// 敵弾丸取得処理
	/// </summary>
	/// <returns></returns>
	std::vector<ShotBase*> GetEShots(void);

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void SetDamage(int damage = 1);


protected:

	// プレイヤークラス
	Player* player_;

	// キャラの種類
	TYPE type_;

	// 行動状態
	STATE state_;

	// アニメーション
	AnimationController* animController_;

	std::vector<ShotBase*> shots_;

	// モデルハンドル
	int modelId_;

	// エフェクトモデルID
	int modelIdAtkEffect_;


	// 現在位置
	VECTOR pos_;

	VECTOR angles_;

	// 現在向き(オイラー)
	VECTOR dir_;

	// スケール
	VECTOR scales_;

	// 当たり判定サイズ
	float colRad_;

	COLOR_F color_;

	// HP
	int hp_;

	// 移動速度
	float speed_;

	// 生成位置比率(半径)
	float spawnRange_;

	// 攻撃間隔の時間
	float attackTimeWait_;
	
	// 攻撃有効までの時間
	float attackTimeAct_;

	// 被ダメージ時の点滅
	int damageCnt_;

	// 被ダメージ硬直時間
	float damageTime_;



	// 移動しているか否か
	bool isMove_;


	/// <summary>
	/// パラメータ割り当て
	/// </summary>
	virtual void SetParam(void) = 0;

	/// <summary>
	/// プレイヤー追尾処理
	/// </summary>
	void LookPlayerRotate(void);

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(void);

	/// <summary>
	/// 生成位置割り当て
	/// </summary>
	void SetSpawnPosition(void);

	// 状態遷移
	virtual void ChangeStandby(void);
	virtual void ChangeAttack(void);
	virtual void ChangeHitReact(void);
	virtual void ChangeDeadReact(void);
	virtual void ChangeEnd(void);

	// 状態別更新
	virtual void UpdateStandby(void);
	virtual void UpdateAttack(void);
	virtual void UpdateHitReact(void);
	virtual void UpdateDeadReact(void);

	// 状態別描画
	virtual void DrawStandby(void);
	virtual void DrawAttack(void);
	virtual void DrawHitReact(void);
	virtual void DrawDeadReact(void);


	/// <summary>
	/// 弾の更新
	/// </summary>
	void UpdateShot(void);

	/// <summary>
	/// 弾の描画
	/// </summary>
	void DrawShot(void);

	/// <summary>
	/// 有効な弾の取得処理
	/// </summary>
	ShotBase* GetValidShot(void);
};