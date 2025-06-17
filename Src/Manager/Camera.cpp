#include "Camera.h"
#include "InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Player/Player.h"

Camera::Camera(void)
{
}

void Camera::Init(void)
{
	pos_	= DEFAULT_POS;	  // 座標
	angles_ = DEFAULT_ANGLES; // 角度
}

void Camera::Update(void)
{
	if (player_ != nullptr)
	{
		// プレイヤー追尾処理

		VECTOR playerPos = player_->GetPos();

		// 追尾位置調整値を加算
		playerPos = VAdd(playerPos, FOLLOW_OFFSET);

		pos_.x = playerPos.x;
		pos_.z = playerPos.z;
	}
	else
	{
		Move();

		Rotate();
	}
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定(SetDrawScreenでリセット)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);
}

void Camera::DrawDebug(void)
{
#ifdef _DEBUG
	DrawFormatString(0, 32, 0xFFFFFF,
					 "カメラ座標(%.2f,%.2f, %.2f)",
					pos_.x, pos_.y, pos_.z);

	DrawFormatString(
		0, 48, 0xffffff,
		"カメラ角度：(% .2f, % .2f, % .2f)", angles_.x, angles_.y, angles_.z);

#endif // _DEBUG
}

void Camera::Release(void)
{
}

void Camera::SetFollow(Player* player)
{
	player_ = player;
}


void Camera::Move(void)
{
	InputManager& ins = InputManager::GetInstance();

	// 移動方向を決める
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	if (ins.IsNew(KEY_INPUT_I)) { moveDir = VAdd(moveDir, AsoUtility::DIR_F); }
	if (ins.IsNew(KEY_INPUT_K)) { moveDir = VAdd(moveDir, AsoUtility::DIR_B); }
	if (ins.IsNew(KEY_INPUT_J)) { moveDir = VAdd(moveDir, AsoUtility::DIR_L); }
	if (ins.IsNew(KEY_INPUT_L)) { moveDir = VAdd(moveDir, AsoUtility::DIR_R); }

#pragma region 授業プログラム
	/*
	if (ins.IsNew(KEY_INPUT_I)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_K)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_J)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_L)) { moveDir = AsoUtility::DIR_R; }
	*/
#pragma endregion

	// 移動量を計算する(方向×スピード)
	VECTOR movePow = VScale(moveDir, SPEED_MOVE);

	// 移動処理(座標＋移動量)
	pos_ = VAdd(pos_, movePow);
}

void Camera::Rotate(void)
{
	InputManager& ins = InputManager::GetInstance();

	// X軸の角度調整
	float anglePowRad = AsoUtility::Deg2Rad(SPEED_ANGLE_DEG);
	if (ins.IsNew(KEY_INPUT_U)) { angles_.x += anglePowRad; }
	if (ins.IsNew(KEY_INPUT_O)) { angles_.x -= anglePowRad; }
}