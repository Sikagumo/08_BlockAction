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

// “G‚Ìí—Ş
using ENEMY_TYPE = EnemyBase::TYPE;


EnemyManager::EnemyManager(Player* player)
{
	player_ = player;

	// ƒŠƒXƒgƒNƒŠƒA
	modelIdsEnemy_.clear();
	enemys_.clear();
}

void EnemyManager::Init(void)
{
	timeSpawn_ = SPAWN_WAVE_01;

	wave_ = WAVE::WAVE01;

	// Še“Gƒ‚ƒfƒ‹‚ğƒ[ƒh
	int type;

	type = static_cast<int>(ENEMY_TYPE::DEMON);
	modelIdsEnemy_.emplace(type, MV1LoadModel(PATH_DEMON.c_str()));

	type = static_cast<int>(ENEMY_TYPE::WIZARD);
	modelIdsEnemy_.emplace(type, MV1LoadModel(PATH_WIZARD.c_str()));

	type = static_cast<int>(ENEMY_TYPE::GIANT);
	modelIdsEnemy_.emplace(type, MV1LoadModel(PATH_GIANT.c_str()));

	// ‰Î‰Š’e
	modelIdsAtkEffect_.emplace_back(MV1LoadModel(PATH_FIRE.c_str()));

	// ŠâÎ
	modelIdsAtkEffect_.emplace_back(MV1LoadModel(PATH_ROCK.c_str()));
}

void EnemyManager::Update(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// “G¶¬ŠÔ‚ÌŒ¸­
	timeSpawn_ -= delta;

	// ƒEƒF[ƒuŠÔ‚Ì‘‰Á
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

	// “GXVˆ—
	if (!enemys_.empty())
	{
		for (auto& enemy : enemys_)
		{
			// “GXV
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
	// “G‰ğ•ú
	if (!enemys_.empty())
	{
		for (auto& enemy : enemys_)
		{
			enemy.second->Release();

			delete enemy.second;
		}
		enemys_.clear();
	}


	// “Gƒ‚ƒfƒ‹‰ğ•ú
	for (auto& handle : modelIdsEnemy_)
	{
		MV1DeleteModel(handle.second);
	}
	modelIdsEnemy_.clear();
	

	// “G’e‰ğ•ú
	int size = modelIdsAtkEffect_.size();
	for (int i = 0; i < size; i++)
	{
		MV1DeleteModel(modelIdsAtkEffect_[i]);
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
	if (curWave == maxWave)
	{
		int temp = 0;
	}
	wave_ = static_cast<WAVE>(curWave);
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
		timeWave_ = 0.0f;

		timeSpawn_ = SPAWN_WAVE_02;

		ChangeWave();
	}
}

void EnemyManager::UpdateWave02(void)
{
	if (timeSpawn_ < 0.0f)
	{
		timeSpawn_ = SPAWN_WAVE_02;

		SpawnEnemy(static_cast<int>(ENEMY_TYPE::WIZARD));
	}

	if (timeWave_ >= WAVE_TIME_STATE)
	{
		ChangeWave();
	}
}

void EnemyManager::UpdateWave03(void)
{
	bool isKnock = true;
	for (auto& enemy : enemys_)
	{
		if (enemy.second->GetIsActive()) 
		{
			// 1‘Ì‚Å‚àŒ‚”j‚³‚ê‚Ä‚¢‚È‚¢‚È‚çAfalse
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

void EnemyManager::SpawnEnemy(int type)
{
	EnemyBase* enemy = {};

	ENEMY_TYPE eType = static_cast<ENEMY_TYPE>(type);

	int shotId = -1;


	// “G¶¬
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

			shotId = modelIdsAtkEffect_[0];
		}
		break;
	}

	// “G‰Šú‰»
	enemy->Init(eType,
				modelIdsEnemy_[type],
				shotId, *player_);

	// “G‚ğŠi”[
	enemys_.emplace(spawnCount_, enemy);

	// ƒJƒEƒ“ƒ^‘‰Á
	spawnCount_++;

#pragma region ƒ‰ƒ“ƒ_ƒ€“G¶¬
	/*
	if (timeSpawn_ < 0.0f)
	{
		EnemyBase* enemy = {};

		type = static_cast<ENEMY_TYPE>(typeSpawn_);

		int id = -1;

		// “G¶¬
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

		// “G‰Šú‰»
		enemy->Init(type,
					modelIdsEnemy_[typeSpawn_],
					id, *player_);

		// “G‚ğŠi”[
		enemys_.emplace(spawnCount_, enemy);


		// ¶¬‚·‚é“G‚Ì¯•Êó‘Ô•ÏX
		typeSpawn_ = GetRand(static_cast<int>(ENEMY_TYPE::MAX) - 2);


		// ¶¬ŠÔ‰Šú‰»
		type = static_cast<ENEMY_TYPE>(typeSpawn_);

		if (type == ENEMY_TYPE::DEMON)  { timeSpawn_ = SPAWN_INTERVAL_DEMON;  }
		if (type == ENEMY_TYPE::WIZARD) { timeSpawn_ = SPAWN_INTERVAL_WIZARD; }
	}*/
#pragma endregion
}
