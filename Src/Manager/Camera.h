#pragma once
#include <DxLib.h>
class Player;

class Camera
{
public:
	// カメラの初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f, 1000.0f, -1000.0f };

	// カメラの初期角度
	static constexpr VECTOR DEFAULT_ANGLES = { 30.0f * (DX_PI_F / 180.0f), 0.0f, 0.0f };

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 3.0f;

	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 5000.0f;

	// カメラスピード(移動)
	static constexpr float SPEED_MOVE = 10.0f;

	// カメラスピード(度)
	static constexpr float SPEED_ANGLE_DEG = 1.0f;

	// 追尾位置調整値
	static constexpr VECTOR FOLLOW_OFFSET = { 0.0f, 0.0f, -1250.0f };


	Camera(void);

	~Camera(void) = default;


	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画前のカメラ設定
	/// </summary>
	void SetBeforeDraw(void);

	/// <summary>
	/// デバッグ用描画
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);
	
	/// <summary>
	/// 追尾対象割り当て
	/// </summary>
	/// <param name="player"></param>
	void SetFollow(Player* player);


private:

	// プレイヤー
	Player* player_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// 移動処理
	void Move(void);

	// 回転処理
	void Rotate(void);
};
