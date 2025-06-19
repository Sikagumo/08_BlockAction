#include "WeaponPunch.h"
#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../Player/Player.h"


WeaponPunch::WeaponPunch(const VECTOR& pos)
{
	pos_ = pos;
}


void WeaponPunch::Draw(void)
{
	if (!isAlive_) return;

	bool isXRev = false;
	bool isZRev = false;
	int img = -1;

	// 縦画像
	if (moveDir_.z != 0.0f)
	{
		img = imgsZ_[cntAnimation_];

		if (moveDir_.z < 0.0f)
		{
			isZRev = true;
		}
	}

	// 横画像
	if (moveDir_.x != 0.0f)
	{
		img = imgsX_[cntAnimation_];

		if (moveDir_.x > 0.0f)
		{
			isXRev = true;
		}
	}
	

	DrawBillboard3D(pos_, 0.5f, 0.5f,
					IMG_SCALE, 0.0,
					img, true,
					isXRev, isZRev);

	// スプライトアニメーションカウンタ
	cntAnimation_++;

	if (cntAnimation_ >= NUM_SPRITE_ALL)
	{
		isAlive_ = false;
	}

#ifdef _DEBUG
	// デバッグ用衝突判定
	//DrawSphere3D(pos_, colRad_, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void WeaponPunch::Release(void)
{
	// エフェクト画像のメモリの解放
	for (int i = 0; i < NUM_SPRITE_ALL; i++)
	{
		DeleteGraph(imgsX_[i]);
		DeleteGraph(imgsZ_[i]);
	}
}


void WeaponPunch::Use(VECTOR pos, VECTOR dir)
{
	// 武器の高さ調整
	pos_ = VAdd(pos, localPos_);
	moveDir_ = dir;
	isAlive_ = true;
	cntAnimation_ = 0;
}

void WeaponPunch::Load(void)
{
	// エフェクト画像(横)のロード
	LoadDivGraph((PATH_DRILL_X).c_str(),
				 NUM_SPRITE_ALL,
				 NUM_SPRITE_X, NUM_SPRITE_Y,
				 SIZE_XSPRITE_X, SIZE_XSPRITE_Y,
				 imgsX_);

	// エフェクト画像(横)のロード
	LoadDivGraph((PATH_DRILL_Y).c_str(),
				 NUM_SPRITE_ALL,
				 NUM_SPRITE_X, NUM_SPRITE_Y,
				 SIZE_YSPRITE_X, SIZE_YSPRITE_Y,
				 imgsZ_);
}

void WeaponPunch::SetParam(void)
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

	// アニメーションカウンタ初期化
	cntAnimation_ = 0;
}