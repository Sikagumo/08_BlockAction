#pragma once
#include "../../Application.h"
#include <DxLib.h>
#include <string>
class WeaponBase;
class WeaponPunch;
class WeaponAxe;
class AnimationController;

class Player
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		DEATH = 0, // 死亡
		DUCK,
		HIT_REACT, // 被ダメージ

		IDLE, // 待機
		IDLE_ATTACK, // 攻撃待機
		IDLE_HOLD, // 待機

		JUMP,
		JUMP_IDLE,
		JUMP_LAND,

		NO,
		PUNCH, // 攻撃

		RUN,	  // 走る
		RUN_IDLE,
		RUN_HOLD,

		WALK,
		WALK_HOLD,

		WAVE, // 手を振る(勝利演出)

		YES,

		MAX,

		DANCE_1,

		DANCE_2,
	};

	enum class ATTACK_STATE
	{
		NONE = 0,
		PUNCH,
		AXE,
	};


	// 状態
	enum class STATE
	{
		NONE = 0,
		STANDBY,   // 状態待機
		KNOCKBACK, // ノックバック
		ATTACK,    // 攻撃
		DEAD,      // 死亡
		VICTORY,   // 勝利
		END,       // 終了
	};

	const std::string PATH_PLAYER = (Application::PATH_MODEL + "Player/Player.mv1");
	const std::string PATH_ANIM_DANCE_1 = (Application::PATH_MODEL + "Player/Dance1.mv1");
	const std::string PATH_ANIM_DANCE_2 = (Application::PATH_MODEL + "Player/Dance2.mv1");


	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// 初期角度
	static constexpr float DEFAULT_ANGLE_Y = 180.0f;

	// リスポーンする位置
	static constexpr float RESPAWN_POS_Y = 800.0f;

	// 標準の自己発光色
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.5f, 0.5f, 0.5f, 0.5f };


	static constexpr int MAX_HP = 5; // 最大HP

	// 移動スピード
	static constexpr float SPEED_MOVE = 17.5f;

	// 吹っ飛ばされる力
	static constexpr float KNOCK_POWER = 20.0f;


	// アニメーション速度
	static constexpr float SPEED_ANIM = 32.5f;

	// ノックバック速度
	static constexpr float SPEED_KNOCKBACK = 10.0f;


	// ジャンプ力
	static constexpr float JUMP_POW = 22.5f;

	// ジャンプ待機
	static constexpr float JUMP_WAIT_TIME = 1.0f;

	// 地面衝突線分の当たり判定の長さ
	static constexpr float COLLISION_LEN = 10.0f;

	// 半径の大きさ
	static constexpr float RADIUS = 50.0f;

	
	// デフォルトの色
	static constexpr COLOR_F COLOR_DEFAULT = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	// 被ダメージ時の色
	static constexpr COLOR_F COLOR_KNOCK = { 1.0f, 0.0f, 0.0f, 1.0f };

	// ダメージ描画間隔
	static constexpr int KNOCK_TERM = 8;



	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Player(void);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~Player(void) = default;


	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(const VECTOR& startPos);

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
	/// 位置取得処理
	/// </summary>
	/// <param name=""></param>
	const VECTOR& GetPos(void) const;

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CollisionStage(VECTOR pos);

	/// <summary>
	/// 吹っ飛ばし処理
	/// </summary>
	/// <param name="dirXZ">吹っ飛ばすX・Z軸の方向</param>
	/// <param name="jumpPow">上に吹っ飛ばす力</param>
	void SetKnockBack(VECTOR dirXZ, float jumpPow);

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="damage">ダメージの値</param>
	void SetDamage(int damage = 1);

	/// <summary>
	/// プレイヤー状態割り当て
	/// </summary>
	void SetPState(STATE state);


	void SetIsJump(bool flag);


	/// <summary>
	/// 現在HP取得
	/// </summary>
	int GetHp(void)const;

	/// <summary>
	/// 現在の状態を取得
	/// </summary>
	Player::STATE GetPState(void)const;
	
	/// <summary>
	/// 現在の攻撃状態を取得
	/// </summary>
	Player::ATTACK_STATE GetAttackState(void)const;

	/// <summary>
	/// 無敵状態か否か
	/// </summary>
	/// <returns></returns>
	bool IsInvincive(void)const;

	/// <summary>
	/// 使用している武器を取得
	/// </summary>
	/// <returns></returns>
	WeaponBase& GetUseWepon(void)const;


private:

	// アニメーション
	AnimationController* animController_;

	// 武器
	WeaponBase* weaponUse_;
	WeaponBase* weaponPunch_;
	WeaponBase* weaponAxe_;

	// 行動状態
	STATE state_;

	// 攻撃状態
	ATTACK_STATE atkState_;


	// モデルのハンドルID
	int modelId_;
	int modelAnim1Id_;
	int modelAnim2Id_;


	VECTOR pos_;

	// 前位置(+@)
	VECTOR prePos_;

	// 開始位置(+@)
	VECTOR posStart_;

	// 移動方向
	VECTOR moveDir_;

	// 回転度(オイラー)
	VECTOR angles_;

	// スケール
	VECTOR scales_;

	// 吹っ飛ばす方向
	VECTOR knockDir_;

	// 吹っ飛ばし時の点滅
	int knockCnt_;


	// 現在HP
	int hp_;

	// ジャンプ力
	float jumpPow_;

	// ジャンプしているか否か
	bool isJump_;

	// 待機時間(+@)
	float waitTime_;


	/// <summary>
	/// 武器初期化
	/// </summary>
	void InitWeapon(void);

	/// <summary>
	/// アニメーション初期化
	/// </summary>
	void InitAnim(void);
	
	/// <summary>
	/// 待機処理更新処理
	/// </summary>
	void UpdateStandby(void);

	/// <summary>
	/// 移動処理
	/// </summary>
	void ProcessMove(void);

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	void ProcessJump(void);

	/// <summary>
	/// リスポーン処理
	/// </summary>
	void ProcessRespawn(void);
	
	/// <summary>
	/// 攻撃有効処理
	/// </summary>
	void ProcessAttack(void);


	/// <summary>
	/// 吹っ飛ばし処理
	/// </summary>
	void UpdateKnockBack(void);

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void UpdateAttack(void);


	/// <summary>
	/// 状態遷移処理
	/// </summary>
	/// <param name="state">遷移後の状態</param>
	void ChangeState(STATE state);


	/// <summary>
	/// 吹っ飛ばし状態遷移処理
	/// </summary>
	void ChangeKnockBack(void);
	
	/// <summary>
	/// 攻撃状態遷移処理
	/// </summary>
	void ChangeAttack(void);
	
	/// <summary>
	/// 攻撃状態遷移処理
	/// </summary>
	void ChangeDead(void);
	
	/// <summary>
	/// 攻撃状態遷移処理
	/// </summary>
	void ChangeVictory(void);
};