#pragma once
#include <DxLib.h>

class Block
{
public:
	// ブロックモデルの自己発光色
	static constexpr COLOR_F COLOR_EMI = { 0.5f, 0.5f, 0.5f, 1.0f };

	// ブロックモデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// ブロック種別
	enum class TYPE
	{
		NONE = -1,
		GRASS,		// 草
		METAL,		// 鉄
		ICE,		// 氷
		BRICK,		// レンガ
		CRYSTAL,	// クリスタル
		BLANK,		// 空
	};


	// コンストラクタ
	Block(void);
	// デストラクタ
	~Block(void) = default;

	// ブロックを生成する
	void Create(TYPE type, int baseModelId, int mapX, int mapZ);


	void Update(void);

	void Draw(void);

	void Release(void);

	// モデルのハンドルIDを取得する
	int GetModelId(void)const;

private:

	// ブロック種別
	TYPE type_;

	// モデルのハンドルID
	int modelId_;

	// 座標
	VECTOR pos_;

	// 大きさ
	VECTOR scales_;
};