#include "WeaponBase.h"
#include <DxLib.h>

WeaponBase::WeaponBase(const VECTOR& pos)
{
	pos_ = pos;
}

WeaponBase::WeaponBase(void)
{
}

void WeaponBase::Init(void)
{
	// 生存フラグの無効化
	isAlive_ = false;

	// モデルロード処理
	Load();

	// パラメータ割り当て
	SetParam();

	// モデルサイズ割り当て
	MV1SetScale(modelId_, scales_);
}

void WeaponBase::Update(void)
{
	if (!isAlive_) return;

	// 移動処理
	Move();

}


VECTOR WeaponBase::GetPos(void)
{
	return pos_;
}

float WeaponBase::GetColRad(void)
{
	return colRad_;
}

void WeaponBase::Use(VECTOR pos, VECTOR dir)
{
}

bool WeaponBase::IsAlive(void)
{
	return isAlive_;
}

WeaponBase::TYPE WeaponBase::GetType(void)
{
	return type_;
}

void WeaponBase::Move(void)
{
	// 移動方向
	VECTOR moveVec = VScale(moveDir_, speed_);

	// Y軸は考慮しない
	moveVec.y = 0.0f;

	// 移動処理(一方方向)
	pos_ = VAdd(pos_, moveVec);
}
