#include "Player.h"
#include <DxLib.h>
#include <string>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Scene/GameScene.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../Weapon/WeaponBase.h"
#include "../Weapon/WeaponPunch.h"
#include "../Weapon/WeaponAxe.h"
#include "../AnimationController.h"


Player::Player(void)
{
	hp_ = -1;
	pos_ = {};
	state_ = STATE::NONE;

	atkState_ = ATTACK_STATE::NONE;
}


void Player::Init(const VECTOR& startPos)
{
	modelId_ = MV1LoadModel(PATH_PLAYER.c_str());

	modelAnim1Id_ = MV1LoadModel(PATH_ANIM_DANCE_1.c_str());
	modelAnim2Id_ = MV1LoadModel(PATH_ANIM_DANCE_2.c_str());

	// 行動待機化
	ChangeState(STATE::STANDBY);

	// 位置
	pos_ = startPos;

	pos_.y += SCALES.y;
	posStart_ = pos_;


	// HP
	hp_ = MAX_HP;

	// ジャンプ力
	jumpPow_ = 0.0f;

	// ジャンプフラグ
	isJump_ = false;

	// モデルの位置割り当て
	MV1SetPosition(modelId_, pos_);

	// plusAlpha
	prePos_ = pos_;

	// サイズ
	scales_ = SCALES;
	MV1SetScale(modelId_, scales_);

	// モデルの角度
	angles_ = { 0.0f, AsoUtility::Deg2Rad(DEFAULT_ANGLE_Y), 0.0f };
	MV1SetRotationXYZ(modelId_, angles_);

	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);


	// カメラ追尾対象に割り当て
	SceneManager::GetInstance().GetCamera().SetFollow(this);


	atkState_ = ATTACK_STATE::PUNCH;

	// 武器初期化
	weaponPunch_ = new WeaponPunch(pos_);
	weaponPunch_->Init();

	weaponAxe_ = new WeaponAxe(pos_);
	weaponAxe_->Init();

	// パンチ割り当て
	weaponUse_ = weaponPunch_;

	// アニメーション
	animController_ = new AnimationController(modelId_);
	int max = static_cast<int>(ANIM_TYPE::MAX);
	for (int i = 0; i < max; i++)
	{
		// アニメーション割り当て
		animController_->AddInFbx(i, SPEED_ANIM, i);
	}


	animController_->Add(static_cast<int>(ANIM_TYPE::DANCE_1),
						 SPEED_ANIM, PATH_ANIM_DANCE_1.c_str());

	animController_->Add(static_cast<int>(ANIM_TYPE::DANCE_2),
						 SPEED_ANIM, PATH_ANIM_DANCE_2.c_str());

	// 初期アニメーション割り当て
	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void Player::Update(void)
{
	switch (state_)
	{
		case Player::STATE::STANDBY:
		{
			UpdateStandby();
		}
		break;

		case Player::STATE::KNOCKBACK:
		{
			// 吹っ飛ばし処理
			UpdateKnockBack();
		}
		break;

		case Player::STATE::ATTACK:
		{
			// 攻撃処理
			UpdateAttack();
		}
		break;

		case Player::STATE::DEAD:
		{
			if (animController_->IsEnd())
			{
				ChangeState(STATE::END);
			}
		}
		break;

		case Player::STATE::VICTORY:
		{
			angles_.y = AsoUtility::Deg2Rad(-DEFAULT_ANGLE_Y);

			if (animController_->IsEnd())
			{
				//ChangeState(STATE::END);
			}
		}
		break;

		case Player::STATE::END:
		{

		}
		break;
	}

	if (state_ == STATE::STANDBY ||
		state_ == STATE::KNOCKBACK ||
		state_ == STATE::ATTACK)
	{
		weaponUse_->Update();
	}

	// リスポーン処理
	ProcessRespawn();


	// アニメーション更新処理
	animController_->Update();

	/*
	if (angles_ == AsoUtility::DIR_F) { angles_.y += AsoUtility::Deg2RadF(0.0f); }
	if (angles_ == AsoUtility::DIR_S) { angles_.y += AsoUtility::Deg2RadF(180.0f); }
	if (angles_ == AsoUtility::DIR_A) { angles_.y += AsoUtility::Deg2RadF(270.0f); }
	if (angles_ == AsoUtility::DIR_D) { angles_.y += AsoUtility::Deg2RadF(90.0f); }
	*/
}

void Player::Draw(void)
{
	COLOR_F color = COLOR_DEFAULT;
	
	if (state_ == STATE::ATTACK)
	{
		weaponUse_->Draw();
	}

	if (state_ == STATE::KNOCKBACK)
	{
		if (knockCnt_ % KNOCK_TERM == 0)
		{
			// 衝突時、吹っ飛ばし時の色割り当て
			color = COLOR_KNOCK;
		}
	}

	// ディフューズカラーを割り当て
	MV1SetDifColorScale(modelId_, color);

	// モデル描画
	MV1DrawModel(modelId_);


#ifdef _DEBUG
	DrawFormatString(0, 80, 0x0000FF, "pPos: %.1f, %.1f, %.1f, angleY:%.1f°", pos_.x, pos_.y, pos_.z, AsoUtility::Rad2Deg(angles_.y));

	// 当たり判定表示
	DrawSphere3D(pos_, RADIUS, 8, 0xff00ff, 0xffffff, false);
#endif
}

void Player::Release(void)
{
	// モデル解放
	MV1DeleteModel(modelId_);

	// アニメーション解放
	animController_->Release();
	delete animController_;

	// 斧解放
	weaponAxe_->Release();
	delete weaponAxe_;

	// パンチ解放
	weaponPunch_->Release();
	delete weaponPunch_;

	weaponUse_ = nullptr;
}

const VECTOR& Player::GetPos(void) const
{
	return pos_;
}

void Player::CollisionStage(VECTOR pos)
{
	// 衝突判定に指定座標を押し戻す
	pos_ = pos;
	jumpPow_ = 0.0f;

	// ジャンプフラグ有効化
	isJump_ = false;
}

void Player::SetKnockBack(VECTOR dirXZ, float jumpPow)
{
	if (state_ == STATE::KNOCKBACK) return;

	// 吹っ飛ばし状態化
	ChangeState(STATE::KNOCKBACK);

	
	knockDir_ = dirXZ;

	jumpPow_ = jumpPow;

	// ジャンプ判定にする
	isJump_ = true;

	// 点滅カウンタ初期化
	knockCnt_ = 0;

	// 攻撃された方向にプレイヤーを向かせる
	angles_.y = atan2(dirXZ.x, dirXZ.z);
	MV1SetRotationXYZ(modelId_, angles_);
}

void Player::SetDamage(int damage)
{
	// ダメージ値が負の時は正の値にする
	damage *= (damage < 0 ? -1 : 1);

	hp_ -= damage;

	if (hp_ <= 0)
	{
		hp_ = 0;
		ChangeState(STATE::DEAD);
	}
}

void Player::SetPState(STATE state)
{
	ChangeState(state);
}

int Player::GetHp(void)
{
	return hp_;
}

Player::STATE Player::GetPState(void)
{
	return state_;
}

Player::ATTACK_STATE Player::GetAttackState(void)
{
	return atkState_;
}

bool Player::IsInvincive(void)
{
	return (state_ == STATE::KNOCKBACK
		 || state_ == STATE::DEAD
		 || state_ == STATE::END);

}


WeaponBase& Player::GetUseWepon(void)const
{
	return *weaponUse_;
}


void Player::UpdateStandby(void)
{
	// 移動処理
	ProcessMove();

	// 攻撃有効処理
	ProcessAttack();

	// ジャンプ処理
	ProcessJump();
}

void Player::ProcessMove(void)
{
	InputManager& input = InputManager::GetInstance();
	VECTOR moveVec = AsoUtility::VECTOR_ZERO;
	VECTOR dirVec = AsoUtility::VECTOR_ZERO;

	bool isInput = false; // 入力判定フラグ


	if (input.IsNew(KEY_INPUT_W))
	{
		moveVec = VAdd(moveVec, AsoUtility::DIR_F);

		//moveDir_ = AsoUtility::DIR_F;
		isInput = true;
	}
	
	if (input.IsNew(KEY_INPUT_S))
	{
		moveVec = VAdd(moveVec, AsoUtility::DIR_B);

		//moveDir_ = AsoUtility::DIR_B;
		isInput = true;
	}
	
	if (input.IsNew(KEY_INPUT_A))
	{
		moveVec = VAdd(moveVec, AsoUtility::DIR_L);

		//moveDir_ = AsoUtility::DIR_L;
		isInput = true;
	}

	if (input.IsNew(KEY_INPUT_D))
	{
		moveVec = VAdd(moveVec, AsoUtility::DIR_R);
		//moveDir_ = AsoUtility::DIR_R;
		isInput = true;
	}


	// 移動量が０ではない時、移動
	if (!AsoUtility::EqualsVZero(moveVec))
	{
		// 移動アニメーション再生
		if (!isJump_)
		{
			animController_->Play(static_cast<int>(ANIM_TYPE::WALK));
		}

		/* ＋＠ */

		float length = sqrtf((moveVec.x * moveVec.x) + (moveVec.z * moveVec.z));

		// 移動ベクトルを正規化
		moveVec.x /= length;
		moveVec.z /= length;

		if (length > SPEED_MOVE)
		{
			// 長さが移動速度を超えた時、スカラー倍

			float scale = (SPEED_MOVE / length);

			moveVec.x *= scale;
			moveVec.z *= scale;
		}
		/* -＋＠- */

		// 移動量を計算(移動向き×移動速度)
		VECTOR movePow = VScale(moveVec, SPEED_MOVE);

		// 位置に反映する
		pos_ = VAdd(pos_, movePow);
		MV1SetPosition(modelId_, pos_);

		/* +@ */
		// 移動方向によって向きを変える
		dirVec = VSub(pos_, prePos_);
		dirVec.y = 0.0f;
		prePos_ = pos_;
		/* - +@ - */


		// 進行方向を向く
		angles_.y = atan2f(dirVec.x, dirVec.z);

		// 初期角度を補正する
		angles_.y += AsoUtility::Deg2Rad(DEFAULT_ANGLE_Y);

		// オイラー角でモデルを回転
		MV1SetRotationXYZ(modelId_, angles_);

		VECTOR dir = VNorm(dirVec);
		dir.y = angles_.y;


		// 移動方向割り当て
		moveDir_ = dir;
	}
	else if (!isJump_)
	{
		// 待機アニメーション再生
		animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
}

void Player::ProcessJump(void)
{
	InputManager& ins = InputManager::GetInstance();

	// 重力を加算(一定を制限)
	jumpPow_ -= GameScene::GRAVITY;


	if (ins.IsTrgDown(KEY_INPUT_SPACE) && !isJump_)
	{
		isJump_ = true;
		jumpPow_ = JUMP_POW; // ジャンプ力加算

		// アニメーション再生
		animController_->Play(static_cast<int>(ANIM_TYPE::JUMP), false);
	}

	// 座標に移動量を加算
	pos_.y += jumpPow_;

	// モデル割り当て
	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessAttack(void)
{
	InputManager& input = InputManager::GetInstance();

	if (!weaponUse_->IsAlive())
	{
		// 攻撃状態遷移
		if (input.IsTrgDown(KEY_INPUT_J))
		{
			if (atkState_ == ATTACK_STATE::PUNCH)
			{
				atkState_ = ATTACK_STATE::AXE;
				weaponUse_ = weaponAxe_;
			}
			else if (atkState_ == ATTACK_STATE::AXE)
			{
				atkState_ = ATTACK_STATE::PUNCH;
				weaponUse_ = weaponPunch_;
			}
		}

		if (input.IsTrgDown(KEY_INPUT_K) && !isJump_)
		{
			// アニメーション再生
			// 攻撃有効化
			ChangeState(STATE::ATTACK);
		}
	}
}

void Player::ProcessRespawn(void)
{
	if (pos_.y < RESPAWN_POS_Y_CHECK)
	{
		VECTOR pos = posStart_;

		pos.y += RESPAWN_POS_Y;
		pos_ = pos;

		jumpPow_ = 0.0f; // ジャンプ加速度を初期化

		isJump_ = true; // ジャンプ無効化

		/* +@ */
		if (state_ == STATE::KNOCKBACK)
		{
			ChangeState(STATE::STANDBY);
		}
	}
}


void Player::UpdateKnockBack(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 着地時、吹っ飛ばし終了
	if (!isJump_)
	{
		ChangeState(STATE::STANDBY);

		return;
	}

	knockCnt_++;

	// ジャンプさせる
	jumpPow_ -= GameScene::GRAVITY;
	pos_.y += jumpPow_;

	// 吹っ飛ばし処理
	VECTOR knockVec = VScale(knockDir_, KNOCK_POWER);
	pos_ = VAdd(pos_, knockVec);
	MV1SetPosition(modelId_, pos_);
}

void Player::UpdateAttack(void)
{
	if (animController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
	}
}


void Player::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
		case STATE::KNOCKBACK:
		{
			ChangeKnockBack();
		}
		break;
		
		case STATE::ATTACK:
		{
			ChangeAttack();
		}
		break;
		
		case STATE::DEAD:
		{
			ChangeDead();
		}
		break;
		
		case STATE::VICTORY:
		{
			ChangeVictory();
		}
		break;
		
		case STATE::END:
		{
			
		}
		break;
	}
}


void Player::ChangeKnockBack(void)
{
	// ジャンプ判定にする
	isJump_ = true;

	// 点滅カウンタ初期化
	knockCnt_ = 0;

	// アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::DUCK), false);
}

void Player::ChangeAttack(void)
{
	weaponUse_->Use(pos_, moveDir_);

	// アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::PUNCH), false);
}

void Player::ChangeDead(void)
{
	// アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
}

void Player::ChangeVictory(void)
{
	// アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::DANCE_1));

	//animController_->Play(static_cast<int>(ANIM_TYPE::WAVE), false);

	// 正面に向きを変える
	angles_.y = AsoUtility::Deg2Rad(0);
	MV1SetRotationXYZ(modelId_, angles_);
}
