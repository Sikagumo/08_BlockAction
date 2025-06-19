#pragma once
#include "../Application.h"
#include "SceneBase.h"
#include <DxLib.h>
#include <string>
class BlockManager;
class Player;
class Grid;
class EnemyManager;
class HpManager;
class AttackManager;


class GameScene : public SceneBase
{

public:

	enum class GAME_STATE
	{
		NONE,
		GAME_ACTIVE,
		GAME_OVER,
		GAME_CLEAR,
	};


	const std::string PATH_GAME_OVER   = (Application::PATH_IMAGE + "Gameover.png");
	const std::string PATH_GAME_CLEAR  = (Application::PATH_IMAGE + "Gameclear.png");
	const std::string PATH_BACK_GROUND = (Application::PATH_IMAGE + "Background.png");

	static constexpr int GAME_END_X = -600;
	static constexpr int GAME_END_Y = -450;

	// 重力
	static constexpr float GRAVITY = 0.8f;

	// 縦位置のY軸制限値
	static constexpr float POS_LIMIT_Y = -1500.0f;


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	GameScene(void);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~GameScene(void) = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void) override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void) override;


private:

	BlockManager* blockManager_;

	EnemyManager* enemyManager_;

	// HPのUI
	HpManager* hpManager_;

	// 攻撃のUI
	AttackManager* atkManager_;

	// プレイヤー
	Player* player_;

	Grid* grid_;

	// ゲーム状態
	GAME_STATE state_;

	// 背景画像
	int imgBack_;

	// ゲームオーバー画像
	int imgGameOver_;
	
	// ゲームクリア画像
	int imgGameClear_;


	// 地面との衝突用線分
	VECTOR lineTopPos_;
	VECTOR lineDownPos_;



	/// <summary>
	/// 各衝突判定
	/// </summary>
	void Collision(void);

	/// <summary>
	/// ゲーム終了の処理
	/// </summary>
	void ProcessGameEnd(void);

	/// <summary>
	/// ステージブロックとプレイヤーの衝突
	/// </summary>
	void CollisionStageToPlayer(void);
	
	/// <summary>
	/// ステージブロックと敵の石弾の衝突
	/// </summary>
	void CollisionStageToRock(void);

	/// <summary>
	/// 敵とプレイヤーの衝突
	/// </summary>
	void CollisionEnemyToPlayer(void);
	
	/// <summary>
	/// 敵とプレイヤーの攻撃の衝突
	/// </summary>
	void CollisionWeapon(void);
};