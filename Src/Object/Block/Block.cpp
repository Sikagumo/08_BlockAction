#include "Block.h"
#include "BlockManager.h"


Block::Block(void)
{
}

void Block::Create(TYPE type, int baseModelId, int mapX, int mapZ)
{
	// ブロックの種類
	type_ = type;

	// モデルのハンドルID
	modelId_ = MV1DuplicateModel(baseModelId);

	// 色の調整(自己発光)
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI);


	// 1ブロックあたりの大きさ
	const float SIZE_BLOCK = BlockManager::SIZE_BLOCK;

	// 1ブロックあたりの半分の大きさ
	const float SIZE_HALF_BLOCK = (SIZE_BLOCK / 2.0f);


	// 引数で指定されたマップ座標から座標を計算する
	// 今回の３Ｄモデルの中心座標は、ブロックの中心に位置する
	float x = static_cast<float>(mapX);
	float z = static_cast<float>(mapZ);

	VECTOR pos = VGet(((SIZE_BLOCK * x) + SIZE_HALF_BLOCK),
					  -SIZE_HALF_BLOCK,
					  ((SIZE_BLOCK * z) + SIZE_HALF_BLOCK));


	// 座標設定
	pos_ = pos;
	MV1SetPosition(modelId_, pos);

	// 大きさ設定
	scales_ = SCALES;
	MV1SetScale(modelId_, SCALES);

	MV1SetupCollInfo(modelId_, -1);
}

void Block::Update(void)
{
}

void Block::Draw(void)
{
	// モデルの描画
	MV1DrawModel(modelId_);
}

void Block::Release(void)
{
	// モデルのメモリ解放
	MV1DeleteModel(modelId_);
}

int Block::GetModelId(void)const
{
	return modelId_;
}