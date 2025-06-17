#include "EnemyBase.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../Player/Player.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Block/BlockManager.h"
#include "../../Object/AnimationController.h"
#include "../Shot/ShotBase.h"
#include "../Shot/ShotStaright.h"


EnemyBase::EnemyBase(void)
{
	speed_ = 0.0f;
	attackTimeWait_ = 0.0f;
}

void EnemyBase::Init(TYPE type, int baseModel, int baseAtkEffectModel, Player& player)
{
	// モデルのロード
	modelId_ = MV1DuplicateModel(baseModel);

	// エフェクトモデルID
	modelIdAtkEffect_ = baseAtkEffectModel;


	//　識別番号割り当て
	type_ = type;

	player_ = &player;

	// 自己発行有効
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// パラメータ割り当て
	SetParam();

	// モデルスケール割り当て
	MV1SetScale(modelId_, scales_);

	// 生成位置割り当て
	SetSpawnPosition();

	// アニメーション
	animController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		// アニメーション割り当て
		animController_->AddInFbx(i, SPEED_ANIM, i);
	}

	// 初期アニメーション割り当て
	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));


	// 待機状態化
	ChangeState(STATE::STANDBY);
}

void EnemyBase::Update(void)
{
	// プレイヤー未割当時処理終了
	if (player_ == nullptr) return;


	switch (state_)
	{
		case EnemyBase::STATE::STANDBY:
		{
			// 待機状態更新
			UpdateStandby();
		}
		break;
		

		case EnemyBase::STATE::ATTACK:
		{
			// 攻撃処理更新
			UpdateAttack();
		}

		case EnemyBase::STATE::HIT_REACT:
		{
			// 被ダメージ処理
			UpdateHitReact();
		}
		break;
		
		case EnemyBase::STATE::DEAD_REACT:
		{
			// 死亡処理
			UpdateDeadReact();
		}
		break;
	}

	animController_->Update();

	// 弾丸更新処理
	UpdateShot();
}

void EnemyBase::Draw(void)
{
	color_ = COLOR_DEFAULT;

	switch (state_)
	{
		case EnemyBase::STATE::STANDBY:
		{
			// 待機状態描画
			DrawStandby();
		}
		break;

		case EnemyBase::STATE::ATTACK:
		{
			// 攻撃状態描画
			DrawAttack();
		}
		break;
		
		case EnemyBase::STATE::HIT_REACT:
		{
			// 攻撃状態描画
			DrawHitReact();
		}
		break;
		
		case EnemyBase::STATE::DEAD_REACT:
		{
			// 攻撃状態描画
			DrawDeadReact();
		}
		break;
	}

	if (state_ != STATE::END)
	{
		MV1SetDifColorScale(modelId_, color_);

		// モデル描画
		MV1DrawModel(modelId_);

#ifdef _DEBUG
		// 当たり判定表示
		DrawSphere3D(pos_, colRad_, 8, 0xffff00, 0xffffff, false);
#endif
	}

	// 弾丸描画
	DrawShot();
}

void EnemyBase::Release(void)
{
	MV1DeleteModel(modelId_);

	if (!shots_.empty())
	{
		for (auto& shot : shots_)
		{
			shot->Release();
			delete shot;
		}
	}
	shots_.clear();

	// アニメーション解放
	animController_->Release();
	delete animController_;
}



VECTOR EnemyBase::GetPos(void) const
{
	return pos_;
}

float EnemyBase::GetColRad(void) const
{
	return colRad_;
}

bool EnemyBase::GetIsCollision(void) const
{
	bool ret = false;

	if (state_ == STATE::STANDBY ||
		state_ == STATE::ATTACK)
	{
		ret = true;
	}

	return ret;
}


bool EnemyBase::GetIsActive(void) const
{
	bool ret = true;

	if (!GetIsCollision() &&
		hp_ <= 0.0f)
	{
		ret = false;
	}

	return ret;
}

void EnemyBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
		case STATE::STANDBY:
		{

		}
		break;

		case STATE::ATTACK:
		{
			
		}
		break;

		case STATE::HIT_REACT:
		{
			ChangeHitReact();
		}
		break;
		
		case STATE::DEAD_REACT:
		{
			ChangeDeadReact();
		}
		break;
		
		case STATE::END:
		{
			
		}
		break;
	}
}

std::vector<ShotBase*> EnemyBase::GetEShots(void)
{
	return shots_;
}

void EnemyBase::SetDamage(int damage)
{
	// ダメージが０の時、処理終了
	if (damage == 0) return;

	// ダメージが負の時、正の値に戻す
	int num = ((damage < 0) ? -damage : damage);

	// 被ダメージ処理
	hp_ -= num;

	// HPの値制限
	if (hp_ <= 0)
	{
		hp_ = 0;
		ChangeState(STATE::DEAD_REACT);
	}
	else
	{
		ChangeState(STATE::HIT_REACT);
	}
}


void EnemyBase::LookPlayerRotate(void)
{
	VECTOR pPos = player_->GetPos();

	VECTOR diff = VSub(pos_, pPos);

	// プレイヤーが上空か墜落時はY軸に追尾しない
	if (pos_.y < pPos.y ||
		pPos.y < LOOK_TARGET_MIN_Y)
	{
		diff.y = 0.0f;
	}

	// プレイヤーの方向に向く
	dir_ = VNorm(diff);

	angles_.y = atan2f(diff.x, diff.z);

	/*
	
	diff.y = 0.0f;
	VECTOR dir = VNorm(diff);
	
	*/


	// 向きを割り当て
	MV1SetRotationXYZ(modelId_, angles_);
}

void EnemyBase::Move(void)
{
	// 一時的に移動しない判定
	isMove_ = false;

	// プレイヤーとの距離を取得
	VECTOR len = {};
	float moveLenX = MOVE_LEN_PLAYER * Player::SCALES.x;
	float moveLenZ = MOVE_LEN_PLAYER * Player::SCALES.z;

	if (player_->GetPos().x > (pos_.x + moveLenX) ||
	   (player_->GetPos().x + moveLenX) < pos_.x)
	{
		len.x = player_->GetPos().x - pos_.x;
	}

	if (player_->GetPos().z > (pos_.z + moveLenZ) ||
	   (player_->GetPos().z + moveLenZ) < pos_.z)
	{
		len.z = player_->GetPos().z - pos_.z;
	}
	

	// ベクトルの長さを取得
	float sqrtLen = (len.x * len.x) + (len.z * len.z);
	sqrtLen = sqrtf(sqrtLen);

	// 移動量が０の時、処理終了
	if (sqrtLen == 0.0f) return;

	// 移動している
	isMove_ = true;

	// 単位ベクトル
	VECTOR unitVec = {};

	unitVec.x = len.x / sqrtLen;
	unitVec.z = len.z / sqrtLen;

	// 移動処理
	pos_.x += (unitVec.x * speed_);
	pos_.z += (unitVec.z * speed_);

	/*　移動制限処理　*/
	if (pos_.x < 0.0f) pos_.x = 0.0f;
	if (pos_.z < 0.0f) pos_.z = 0.0f;
	if (pos_.x > BlockManager::WORLD_SIZE.x) pos_.x = BlockManager::WORLD_SIZE.x;
	if (pos_.z > BlockManager::WORLD_SIZE.z) pos_.z = BlockManager::WORLD_SIZE.z;

	// 位置割り当て
	MV1SetPosition(modelId_, pos_);
}

void EnemyBase::SetSpawnPosition(void)
{
	// ワールドの中心
	VECTOR worldPos = { (BlockManager::WORLD_SIZE.x / 2.0f),
					  0.0f,
					  (BlockManager::WORLD_SIZE.z / 2.0f)};

	// 平均半径
	float radius = (worldPos.x + worldPos.z) / 2.0f;

	// 生成位置の長さの比率を調整
	radius *= spawnRange_;


	float degree = static_cast<float>(GetRand(360));

	float rot = degree * (DX_PI_F / 180.0f);
	//float rot = AsoUtility::Deg2Rad(degree);

	// 向き
	float dirX = sinf(rot);
	float dirZ = cosf(rot);
	VECTOR dir = {dirX, 0.0f,dirZ};

	// 方向×長さ
	VECTOR vec = VScale(dir, radius);

	// ワールド位置にベクトルを反映
	VECTOR pos = VAdd(worldPos, vec);

	// 位置に割り当て
	pos_ = pos;
	

	MV1SetPosition(modelId_, pos_);
}


void EnemyBase::ChangeStandby(void)
{
	// 待機アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void EnemyBase::ChangeAttack(void)
{
	// 攻撃アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
}

void EnemyBase::ChangeHitReact(void)
{
	damageTime_ = DAMAGE_MAX_TIME;

	// 被ダメージ時アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::HIT_REACT), false);
}

void EnemyBase::ChangeDeadReact(void)
{
	damageTime_ = DEAD_MAX_TIME;

	// 撃破時アニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
}

void EnemyBase::ChangeEnd(void)
{
	ChangeState(STATE::END);
}


void EnemyBase::UpdateStandby(void)
{
	// 被ダメージアニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void EnemyBase::UpdateAttack(void)
{
	// 被ダメージアニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::ATTACK), false);
}

void EnemyBase::UpdateHitReact(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	damageCnt_++;

	damageTime_ -= delta;

	if (damageTime_ < 0.0f &&
		animController_->IsEnd())
	{
		// 状態遷移
		ChangeState(STATE::STANDBY);
	}
}

void EnemyBase::UpdateDeadReact(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	damageCnt_++;

	damageTime_ -= delta;


	if (damageTime_ < 0.0f &&
		animController_->IsEnd())
	{
		// 状態遷移
		ChangeState(STATE::END);
	}
}


void EnemyBase::DrawStandby(void)
{
}

void EnemyBase::DrawAttack(void)
{
}

void EnemyBase::DrawHitReact(void)
{
	color_ = COLOR_DEFAULT;

	if ((damageCnt_ % DAMAGE_TERM) == 0)
	{
		color_ = COLOR_DAMAGE;
	}

	// 被ダメージアニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::HIT_REACT), false);
}

void EnemyBase::DrawDeadReact(void)
{
	color_ = COLOR_DEFAULT;

	if ((damageCnt_ % DAMAGE_TERM) == 0)
	{
		color_ = COLOR_DAMAGE;
	}

	// 被ダメージアニメーション再生
	animController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
}


void EnemyBase::UpdateShot(void)
{
	for (auto& shot : shots_)
	{
		if (!shot->GetIsAlive())
		{
			// 弾が無効時、スキップ
			continue;
		}

		// 弾丸更新処理
		shot->Update();
	}
}

void EnemyBase::DrawShot(void)
{
	if (shots_.empty()) return;

	for (auto& shot : shots_)
	{
		if (!shot->GetIsAlive())
		{
			continue;
		}

		// 描画処理
		shot->Draw();
	}
}

ShotBase* EnemyBase::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		// 未使用弾丸があるときは再使用するs
		if (!shots_[i]->GetIsAlive())
		{
			return shots_[i];
		}
	}

	ShotBase* shot = new ShotStaright(ShotBase::TYPE::STRAIGHT, modelIdAtkEffect_);

	// リストに格納
	shots_.push_back(shot);

	return shot;
}
