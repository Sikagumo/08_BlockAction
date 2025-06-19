#include "EnemyManager.h"
#include <DxLib.h>
#include <map>

#include "../Application.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Enemy/Enemy_Demon.h"
#include "../Object/Enemy/Enemy_Wizard.h"
#include "../Object/Enemy/Enemy_Giant.h"
#include "../Object/Block/BlockManager.h"
#include "../Object/Shot/ShotBase.h"
#include "../Utility/AsoUtility.h"
#include "./SceneManager.h"

// 敵の種類
using ENEMY_TYPE = EnemyBase::TYPE;
using SHOT_TYPE = ShotBase::TYPE;


EnemyManager::EnemyManager(Player* player)
{
	player_ = player;

	// リストクリア
	modelIdsEnemy_.clear();
	enemys_.clear();
}

void EnemyManager::Init(void)
{
	timeSpawn_ = SPAWN_WAVE_01;

	wave_ = WAVE::WAVE01;

	// 敵読み込み処理
	LoadEnemy();
}

void EnemyManager::LoadEnemy(void)
{
	// 各モデルをロード
	int type;

	// デーモン
	type = static_cast<int>(ENEMY_TYPE::DEMON);
	modelIdsEnemy_.emplace(type, MV1LoadModel(PATH_DEMON.c_str()));

	// ウィザード
	type = static_cast<int>(ENEMY_TYPE::WIZARD);
	modelIdsEnemy_.emplace(type, MV1LoadModel(PATH_WIZARD.c_str()));

	// ジャイアント
	type = static_cast<int>(ENEMY_TYPE::GIANT);
	modelIdsEnemy_.emplace(type, MV1LoadModel(PATH_GIANT.c_str()));


	// 火炎弾
	type = static_cast<int>(SHOT_TYPE::STRAIGHT);
	modelIdsAtkEffect_.emplace(type, MV1LoadModel(PATH_FIRE.c_str()));

	// 岩石
	type = static_cast<int>(SHOT_TYPE::ROCK);
	modelIdsAtkEffect_.emplace(type, MV1LoadModel(PATH_ROCK.c_str()));

#ifdef _DEBUG

	for (auto& enemyId : modelIdsEnemy_)
	{
		if (enemyId.second == -1)
		{
			OutputDebugString("\n敵弾丸モデルが未割当の部分があります。(；；)\n\n");
		}
	}

	for (auto& atkId : modelIdsAtkEffect_)
	{
		if (atkId.second == -1)
		{
			OutputDebugString("\n敵弾丸モデルが未割当の部分があります。(；；)\n\n");
		}
	}
#endif
}


void EnemyManager::Update(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 敵生成時間の減少
	timeSpawn_ -= delta;

	// ウェーブ時間の増加
	timeWave_ += delta;


	switch (wave_)
	{
		case WAVE::WAVE01:
		{
			UpdateWave01();
		}
		break;

		case WAVE::WAVE02:
		{
			UpdateWave02();
		}
		break;

		case WAVE::WAVE03:
		{
			UpdateWave03();
		}
		break;

		case WAVE::END:
		{
			UpdateWaveEnd();
		}
		break;
	}

	// 敵更新処理
	if (!enemys_.empty())
	{
		for (auto& enemy : enemys_)
		{
			// 敵更新
			enemy.second->Update();
		}
	}
}

void EnemyManager::Draw(void)
{
	if (!enemys_.empty())
	{
		for (auto& enemy : enemys_)
		{
			enemy.second->Draw();
		}
	}

#ifdef _DEBUG
	DrawFormatString(250,0,0xFF0000, "wave:%.1f, spawn:%.1f", timeWave_, timeSpawn_);

#endif
}

void EnemyManager::Release(void)
{
	// 敵解放
	if (!enemys_.empty())
	{
		for (auto& enemy : enemys_)
		{
			enemy.second->Release();

			delete enemy.second;
		}
		enemys_.clear();
	}


	// 敵モデル解放
	for (auto& handle : modelIdsEnemy_)
	{
		MV1DeleteModel(handle.second);
	}
	modelIdsEnemy_.clear();
	

	// 敵弾解放
	for (auto& handle : modelIdsAtkEffect_)
	{
		MV1DeleteModel(handle.second);
	}
	modelIdsAtkEffect_.clear();
}

const std::map<int, EnemyBase*>& EnemyManager::GetEnemys(void)
{
	return enemys_;
}

bool EnemyManager::GetWaveEnd(void)
{
	bool ret = false;
	if (wave_ == WAVE::END)
	{
		ret = true;
	}
	return ret;
}

void EnemyManager::ChangeWave(void)
{
	int curWave = static_cast<int>(wave_);
	int maxWave = static_cast<int>(WAVE::END);


	if ((curWave + 1) <= maxWave)
	{
		curWave++;
	}
	if (curWave >= maxWave)
	{
		int temp = 0;
	}

	// ウェーブ割り当て
	wave_ = static_cast<WAVE>(curWave);

	switch (wave_)
	{
		case WAVE::WAVE02:
		{
			ChangeWave02();
		}
		break;
		
		case WAVE::WAVE03:
		{
			ChangeWave03();
		}
		break;
	}
}

void EnemyManager::UpdateWave01(void)
{
	if (timeSpawn_ < 0.0f)
	{
		timeSpawn_ = SPAWN_WAVE_01;

		SpawnEnemy(static_cast<int>(ENEMY_TYPE::DEMON));
	}

	if (timeWave_ >= WAVE_TIME_STATE)
	{
		ChangeWave();
	}
}

void EnemyManager::UpdateWave02(void)
{
	if (timeWave_ < WAVE_TIME_STATE)
	{
		if (timeSpawn_ < 0.0f)
		{
			timeSpawn_ = SPAWN_WAVE_02;

			// ウィザード生成
			SpawnEnemy(static_cast<int>(ENEMY_TYPE::WIZARD));
		}
	}

	else
	{
		bool isKnock = true;
		for (auto& enemy : enemys_)
		{
			if (enemy.second->GetIsActive())
			{
				// 1体でも撃破されていないなら、false
				isKnock = false;
				break;
			}
		}

		// 全撃破時、次のウェーブに遷移
		if (isKnock)
		{
			ChangeWave();
		}
	}
}

void EnemyManager::UpdateWave03(void)
{
	bool isKnock = true;
	for (auto& enemy : enemys_)
	{
		if (enemy.second->GetIsActive()) 
		{
			// 1体でも撃破されていないなら、false
			isKnock = false;
			break;
		}
	}

	if (isKnock)
	{
		ChangeWave();
	}
}


void EnemyManager::UpdateWaveEnd(void)
{

}


void EnemyManager::ChangeWave02(void)
{
	timeWave_ = 0.0f;

	timeSpawn_ = SPAWN_WAVE_02;
}

void EnemyManager::ChangeWave03(void)
{
	timeWave_ = 0.0f;

	// ジャイアント1体生成
	SpawnEnemy(static_cast<int>(ENEMY_TYPE::GIANT));
}


void EnemyManager::SpawnEnemy(int type)
{
	EnemyBase* enemy = {};

	ENEMY_TYPE eType = static_cast<ENEMY_TYPE>(type);

	// 弾のハンドル
	int shotId = -1;

	// 弾丸の種類
	SHOT_TYPE shotType = SHOT_TYPE::NONE;


	// 敵生成
	switch (eType)
	{
		case ENEMY_TYPE::DEMON:
		{
			enemy = new Enemy_Demon();
		}
		break;

		case ENEMY_TYPE::WIZARD:
		{
			enemy = new Enemy_Wizard();

			shotType = SHOT_TYPE::STRAIGHT;

			int id = static_cast<int>(shotType);
			shotId = modelIdsAtkEffect_[id];
		}
		break;
		
		case ENEMY_TYPE::GIANT:
		{
			enemy = new Enemy_Giant();

			shotType = SHOT_TYPE::ROCK;

			int id = static_cast<int>(shotType);
			shotId = modelIdsAtkEffect_[id];
		}
		break;
	}

	// 敵初期化
	enemy->Init(eType, modelIdsEnemy_[type],
				*player_,
				shotType, shotId);

	// 敵を格納
	enemys_.emplace(spawnCount_, enemy);

	// カウンタ増加
	spawnCount_++;

#pragma region ランダム敵生成
	/*
	if (timeSpawn_ < 0.0f)
	{
		EnemyBase* enemy = {};

		type = static_cast<ENEMY_TYPE>(typeSpawn_);

		int id = -1;

		// 敵生成
		switch (type)
		{
			case ENEMY_TYPE::DEMON:
			{
				enemy = new Enemy_Demon();
			}
			break;

			case ENEMY_TYPE::WIZARD:
			{
				enemy = new Enemy_Wizard();

				id = modelIdsAtkEffect_[0];
			}
			break;
		}

		// 敵初期化
		enemy->Init(type,
					modelIdsEnemy_[typeSpawn_],
					id, *player_);

		// 敵を格納
		enemys_.emplace(spawnCount_, enemy);


		// 生成する敵の識別状態変更
		typeSpawn_ = GetRand(static_cast<int>(ENEMY_TYPE::MAX) - 2);


		// 生成時間初期化
		type = static_cast<ENEMY_TYPE>(typeSpawn_);

		if (type == ENEMY_TYPE::DEMON)  { timeSpawn_ = SPAWN_INTERVAL_DEMON;  }
		if (type == ENEMY_TYPE::WIZARD) { timeSpawn_ = SPAWN_INTERVAL_WIZARD; }
	}*/
#pragma endregion
}
