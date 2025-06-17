#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Object/Player/Player.h"
#include "AttackManager.h"
#include <DxLib.h>
#include <string>

AttackManager::AttackManager(Player* player)
{
	player_ = player;
}


void AttackManager::Init(void)
{
	// ‚Q‚c‰æ‘œ‚Ìƒ[ƒh
	imgPunch_ = LoadGraph(PATH_PUNCH.c_str());

	imgAxe_ = LoadGraph(PATH_AXE.c_str());
}

void AttackManager::Update(void)
{

}

void AttackManager::Draw(void)
{
	int image = -1;

	if (player_->GetAttackState() == Player::ATTACK_STATE::PUNCH)
	{
		image = imgPunch_;
	}
	else if (player_->GetAttackState() == Player::ATTACK_STATE::AXE)
	{
		image = imgAxe_;
	}

	DrawGraph(POS_X, POS_Y, image, true);
}

void AttackManager::Release(void)
{
	// ‚Q‚c‰æ‘œ‚Ìƒƒ‚ƒŠ‰ğ•ú
	DeleteGraph(imgPunch_);
	DeleteGraph(imgAxe_);
}