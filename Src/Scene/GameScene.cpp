#include "SceneBase.h"
#include "GameScene.h"
#include <DxLib.h>
#include <vector>
#include "../Object/Block/BlockManager.h"
#include "../Object/Player/Player.h"
#include "../Object/Weapon/WeaponBase.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Shot/ShotBase.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/EnemyManager.h"
#include "../Common/Grid.h"
#include "../Common/UI/HpManager.h"
#include "../Common/UI/AttackManager.h"


GameScene::GameScene(void)
{
	state_ = GAME_STATE::NONE;
}

void GameScene::Init(void)
{
	state_ = GAME_STATE::GAME_ACTIVE;

	grid_ = new Grid;

	blockManager_ = new BlockManager;
	blockManager_->Init();

	player_ = new Player;

	VECTOR pos = AsoUtility::VECTOR_ONE;
	pos = VAdd(pos, { (BlockManager::WORLD_SIZE.x / 2.0f),
					   0.0f,
					  (BlockManager::WORLD_SIZE.z / 2.0f)});

	player_->Init(pos);

	// 敵マネージャ
	enemyManager_ = new EnemyManager(player_);
	enemyManager_->Init();

	// HPのUI
	hpManager_ = new HpManager(player_);
	hpManager_->Init();
	
	// 攻撃のUI
	atkManager_ = new AttackManager(player_);
	atkManager_->Init();
	
	imgGameOver_ = LoadGraph(PATH_GAME_OVER.c_str());

	imgGameClear_ = LoadGraph(PATH_GAME_CLEAR.c_str());

	imgBack_ = LoadGraph(PATH_BACK_GROUND.c_str());
}

void GameScene::Update(void)
{
	// 状態未割当時は処理終了
	if (state_ == GAME_STATE::NONE) return;

	// プレイヤー更新処理
	player_->Update();


	if (state_ == GAME_STATE::GAME_ACTIVE)
	{
		enemyManager_->Update();

		// 衝突判定
		Collision();

		hpManager_->Update();
		atkManager_->Update();

		// ゲーム終了処理
		ProcessGameEnd();
	}

	else
	{
		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			// タイトルシーンに戻る
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
}

void GameScene::Draw(void)
{
	// 背景表示
	DrawGraph(0, 0, imgBack_, true);

#ifdef _DEBUG
	grid_->Draw();
#endif


	blockManager_->Draw();

	player_->Draw();

	enemyManager_->Draw();

	hpManager_->Draw();

	atkManager_->Draw();

	
	if (state_ != GAME_STATE::GAME_ACTIVE)
	{
		int image = -1;
		int x = (Application::SCREEN_SIZE_X / 2) + (GAME_END_X / 2);
		int y = (Application::SCREEN_SIZE_Y / 2) + (GAME_END_Y / 2);

		if (state_ == GAME_STATE::GAME_CLEAR)
		{
			image = imgGameClear_;
		}
		if (state_ == GAME_STATE::GAME_OVER)
		{
			image = imgGameOver_;
		}

		DrawGraph(x, y, image, true);
	}	


#ifdef _DEBUG
	DrawFormatString(0, 0, 0xFFFFFF, "GameScene");

	// 地面との衝突用線分
	DrawSphere3D(lineTopPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);
	DrawSphere3D(lineDownPos_, 20.0f, 10, 0x00ff00, 0x00ff00, true);
	DrawLine3D(lineTopPos_, lineDownPos_, 0xff0000);
#endif

}

void GameScene::Release(void)
{
	// 背景解放
	DeleteGraph(imgBack_);

	// ゲームクリア画像解放
	DeleteGraph(imgGameClear_);
	
	// ゲームオーバー画像解放
	DeleteGraph(imgGameOver_);


	atkManager_->Release();
	delete atkManager_;

	hpManager_->Release();
	delete hpManager_;


	enemyManager_->Release();
	delete enemyManager_;


	player_->Release();
	delete player_;

	blockManager_->Release();
	delete blockManager_;
	
	grid_->Release();
	delete grid_;
}

void GameScene::Collision(void)
{
	// ステージブロックとプレイヤーの衝突
	CollisionStageToPlayer();

	// ステージブロックと敵弾の衝突
	CollisionStageToRock();

	// プレイヤーと敵の衝突
	CollisionEnemyToPlayer();

	// 敵とプレイヤーの攻撃の衝突
	CollisionWeapon();
}

void GameScene::ProcessGameEnd(void)
{
	// プレイヤーのHPが０の時
	if (player_->GetHp() <= 0)
	{
		// プレイヤーが終了状態の時
		if (player_->GetPState() == Player::STATE::END)
		{
			// ゲームオーバー状態にする
			state_ = GAME_STATE::GAME_OVER;
		}
	}

	// 敵を全て撃破したとき、
	else if (enemyManager_->GetWaveEnd())
	{
		// ゲームクリア状態にする
		state_ = GAME_STATE::GAME_CLEAR;

		// プレイヤーに勝利演出を行わせる
		player_->SetPState(Player::STATE::VICTORY);
	}

}

void GameScene::CollisionStageToPlayer(void)
{
	// ステージブロックとプレイヤーの衝突
	VECTOR playerPos = player_->GetPos();

	// 線分の上座標
	VECTOR topPos = playerPos;
	topPos.y	  += (Player::COLLISION_LEN * 2.0f);
	lineTopPos_   = topPos;

	// 線分の下座標
	VECTOR downPos = playerPos;
	downPos.y	   -= Player::COLLISION_LEN;
	lineDownPos_   = downPos;

	// 線分とブロックモデルの衝突判定
	MV1_COLL_RESULT_POLY result;


	if (blockManager_->IsCollisionLine(topPos, downPos, &result))
	{
		// プレイヤーに衝突座標を渡す
		player_->CollisionStage(result.HitPosition);
	}
}

void GameScene::CollisionStageToRock(void)
{
	// 敵の情報を取得
	std::map<int, EnemyBase*> enemys = enemyManager_->GetEnemys();

	for (auto& enemy : enemys)
	{
		std::vector<ShotBase*> shot = enemy.second->GetEShots();

		int size = shot.size();
		for (int i = 0; i < size; i++)
		{
			VECTOR shotPos = shot[i]->GetPos();
			float shotRad = shot[i]->GetColRad();

			// 石弾出ない時・弾丸が無効時は、スキップ
			if (shot[i]->GetType() != ShotBase::TYPE::ROCK ||
				!shot[i]->GetIsAlive())
			{
				continue;
			}

			
			// 線分の上座標
			VECTOR topPos = shotPos;
			topPos.y += (shotRad * 2.0f);

			// 線分の下座標
			VECTOR downPos = shotPos;
			downPos.y -= shotRad;


			// 線分とブロックモデルの衝突判定
			MV1_COLL_RESULT_POLY result;

			if (blockManager_->IsCollisionLine(topPos, downPos, &result))
			{
				// 石弾に衝突座標を渡す
				shot[i]->SetCollisionStage(result.HitPosition);
			}
		}
	}
}

void GameScene::CollisionEnemyToPlayer(void)
{
	// エネミーとプレイヤーの衝突判定
	VECTOR pPos = player_->GetPos();

	// 敵の情報を取得
	std::map<int, EnemyBase*> enemys = enemyManager_->GetEnemys();

	WeaponBase& weapon = player_->GetUseWepon();

	for (auto& enemy : enemys)
	{
		VECTOR enemyPos = enemy.second->GetPos();
		float enemyRad  = enemy.second->GetColRad();

		if (player_->IsInvincive())
		{
			// 無敵中は処理をしない
			return;
		}


		// 敵とプレイヤーの衝突判定条件を満たしたら、
		if (AsoUtility::IsHitSpheres(pPos, Player::RADIUS,
									 enemyPos, enemyRad))
		{
			if (!enemy.second->GetIsCollision())
			{
				// 当たり判定が無いとき、処理終了
				continue;
			}

			VECTOR knockDir = VSub(pPos, enemyPos);
			knockDir.y = 0.0f;

			knockDir = AsoUtility::VNormalize(knockDir);

			player_->SetKnockBack(knockDir, 20.0f);
			player_->SetDamage();
		}
	}
	
	for (auto& enemy : enemys)
	{
		std::vector<ShotBase*> shot = enemy.second->GetEShots();

		int size = shot.size();
		for (int i = 0; i < size; i++)
		{
			VECTOR shotPos = shot[i]->GetPos();
			float shotRad  = shot[i]->GetColRad();

			if (player_->IsInvincive())
			{
				// 無敵中は処理をしない
				return;
			}

			if (!shot[i]->GetIsAlive())
			{
				// 弾丸が無効時はスキップ
				continue;
			}

			// 敵の弾とプレイヤーの衝突判定条件を満たしたら、
			if (AsoUtility::IsHitSpheres(pPos, Player::RADIUS,
										 shotPos, shotRad))
			{
				VECTOR knockDir = VSub(pPos, enemy.second->GetPos());
				knockDir.y = 0.0f;

				knockDir = AsoUtility::VNormalize(knockDir);

				player_->SetKnockBack(knockDir, 20.0f);
				player_->SetDamage();

				// 弾無効化
				shot[i]->SetIsAlive(false);
			}
		}
	}
}

void GameScene::CollisionWeapon(void)
{
	WeaponBase& useWeapon = player_->GetUseWepon();

	if (!useWeapon.IsAlive())
	{
		return;
	}

	VECTOR weaponPos = useWeapon.GetPos();
	float weaponRad  = useWeapon.GetColRad();


	// 敵の情報を取得
	std::map<int, EnemyBase*> enemys = enemyManager_->GetEnemys();


	for (auto& enemy : enemys)
	{
		if (!enemy.second->GetIsCollision())
		{
			// 当たり判定が無いとき、スキップ
			continue;
		}

		VECTOR enemyPos = enemy.second->GetPos();
		float enemyRad  = enemy.second->GetColRad();


		if (AsoUtility::IsHitSpheres(weaponPos, weaponRad,
									 enemyPos, enemyRad))
		{
			enemy.second->SetDamage();
		}
	}
}
