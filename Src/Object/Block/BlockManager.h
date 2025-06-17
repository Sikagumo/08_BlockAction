#pragma once
#include <vector>
#include <DxLib.h>
class Block;

class BlockManager
{
public:
	// ブロックの数(マップのサイズ)
	static constexpr int NUM_BLOCK_X = 30;
	static constexpr int NUM_BLOCK_Z = 30;

	// 1ブロックあたりのサイズ
	static constexpr float SIZE_BLOCK = 100.0f;

	// マップ全体のサイズ
	static constexpr VECTOR WORLD_SIZE = { (SIZE_BLOCK * static_cast<float>(NUM_BLOCK_X)),
											800.0f,
										  (SIZE_BLOCK * static_cast<float>(NUM_BLOCK_Z)) };


	// コンストラクタ
	BlockManager(void);

	// デストラクタ
	~BlockManager(void) = default;

	void Init(void);

	void Update(void);

	void Draw(void);

	void Release(void);

	/// <summary>
	/// 線分とブロックの当たり判定
	/// </summary>
	/// <param name="topPos">上の位置</param>
	/// <param name="downPos">下の位置</param>
	/// <param name="result"></param>
	bool IsCollisionLine(VECTOR topPos, VECTOR downPos, MV1_COLL_RESULT_POLY* result);

	
private:

	// ブロック用のモデルハンドルID
	std::vector<int> baseBlockModelIds_;

	// マップデータ
	int mapData_[NUM_BLOCK_Z][NUM_BLOCK_X];

	Block* blocks_[NUM_BLOCK_Z][NUM_BLOCK_X];


	void LoadMapCsvData(void);
};
