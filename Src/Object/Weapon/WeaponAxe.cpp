#include "WeaponAxe.h"
#include "../../Application.h"
#include "../Player/Player.h"
#include "../../Utility/AsoUtility.h"
#include <string>

WeaponAxe::WeaponAxe(const VECTOR&)
{
}

void WeaponAxe::Draw(void)
{
	if (!isAlive_) return;

	// モデル描画
	MV1DrawModel(modelId_);


#ifdef _DEBUG
	// デバッグ用衝突判定
	//DrawSphere3D(pos_, colRad_, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void WeaponAxe::Release(void)
{
	MV1DeleteModel(modelId_);
}

void WeaponAxe::Use(VECTOR pos, VECTOR dir)
{
	// 武器の高さ調整
	pos_ = VAdd(pos, localPos_);

	moveDir_ = dir;

	isAlive_ = true;

	jumpPow_ = JUMP_POW;

	angles_.y = atan2f(moveDir_.x, moveDir_.z);

	angles_.x = 0.0f;
	angles_.y += AsoUtility::Deg2Rad(MODEL_ANGLE_Y);
	angles_.z = 0.0f;


	// 角度調整
	MV1SetRotationXYZ(modelId_, angles_);
}

void WeaponAxe::Load(void)
{
	std::string path = Application::PATH_MODEL + "Weapon/Axe_Wood.mv1";
	modelId_ = MV1LoadModel(path.c_str());
}

void WeaponAxe::SetParam(void)
{
	type_ = TYPE::PUNCH;

	// モデルの大きさ
	scales_ = { MODEL_SCALE, MODEL_SCALE, MODEL_SCALE };

	// 移動スピード
	speed_ = MOVE_SPEED;

	// 衝突判定用半径
	colRad_ = COLLISION_RADIUS;

	// 使用時の位置調整
	localPos_ = MODEL_LOCAL_POS;

	// ジャンプ力
	jumpPow_ = 0.0f;
}

void WeaponAxe::Move(void)
{
	// 移動方向
	VECTOR moveVec = VScale(moveDir_, speed_);

	moveVec.y = 0.0f;


	jumpPow_ -= GRAVITY;

	moveVec.y += jumpPow_;


	pos_ = VAdd(pos_, moveVec);

	// 位置調整
	MV1SetPosition(modelId_, pos_);


	if (pos_.y <= 0.0f)
	{
		isAlive_ = false;
	}
}