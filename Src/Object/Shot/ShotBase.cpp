#include "ShotBase.h"
#include <DxLib.h>
#include <math.h>
#include "../../Manager/SceneManager.h"



ShotBase::ShotBase(void)
{
}


ShotBase::ShotBase(TYPE type, int baseModelId)
{
	type_ = type;

	// 使用メモリ容量と読み込み時間の削減のため
	// モデルデータをいくつもメモリ上に存在させない
	modelId_ = MV1DuplicateModel(baseModelId);
}


void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
	// 弾の発射位置を設定
	pos_ = pos;

	// 弾の発射方向の設定
	dir_ = dir;

	// パラメータ設定
	SetParam();

	VECTOR vec = VScale(dir_, -50.0f);

	vec = VAdd(pos_, vec);

	// 位置の設定
	MV1SetPosition(modelId_, vec);

	// 大きさの設定
	MV1SetScale(modelId_, scales_);
}

void ShotBase::Update(void)
{
	// 生存していなければ処理中断
	if (!GetIsAlive()) return;

	// 弾を移動させる
	// 移動量の計算(方向×スピード)
	VECTOR movePow = VScale(dir_, -speed_);

	// 移動処理
	pos_ = VAdd(pos_, movePow);

	// 位置の設定
	MV1SetPosition(modelId_, pos_);


	// 生存カウンタの減少
	ReduceAliveTime();
}

void ShotBase::Draw()
{
	// 生存していなければ処理中断
	if (!GetIsAlive()) return;

	// モデルの描画
	MV1DrawModel(modelId_);

#ifdef _DEBUG
	// デバッグ用：衝突判定用球体
	DrawSphere3D(pos_, collisionRadius_, 10,
				 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void ShotBase::Release(void)
{
	MV1DeleteModel(modelId_);
}


bool ShotBase::GetIsAlive(void)
{
	return isAlive_;
}

VECTOR ShotBase::GetPos(void)
{
	return pos_;
}

float ShotBase::GetColRad(void)
{
	return collisionRadius_;
}

ShotBase::TYPE ShotBase::GetType(void)
{
	return type_;
}

void ShotBase::SetIsAlive(bool isAlive)
{
	isAlive_ = isAlive;
}


void ShotBase::ReduceAliveTime(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();

	aliveTime_ -= delta;

	if (aliveTime_ < 0.0f)
	{
		isAlive_ = false;
	}
}