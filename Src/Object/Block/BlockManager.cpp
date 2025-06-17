#include "BlockManager.h"
#include <string>
#include <fstream>
#include <algorithm>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "Block.h"


BlockManager::BlockManager(void)
{
}

void BlockManager::Init(void)
{
	// 各種ブロックモデルのロード
	std::string PATH = Application::PATH_MODEL + "Blocks/";
	std::string path = PATH;

	int num = 0;

	num = static_cast<int>(Block::TYPE::GRASS);
	path = (PATH + "Block_Grass.mv1");
	baseBlockModelIds_.push_back(MV1LoadModel(path.c_str()));

	num = static_cast<int>(Block::TYPE::METAL);
	path = (PATH + "Block_Metal.mv1");
	baseBlockModelIds_.push_back(MV1LoadModel(path.c_str()));
	
	num = static_cast<int>(Block::TYPE::ICE);
	path = (PATH + "Block_Ice.mv1");
	baseBlockModelIds_.push_back(MV1LoadModel(path.c_str()));
	
	num = static_cast<int>(Block::TYPE::BRICK);
	path = (PATH + "Block_GreyBricks.mv1");
	baseBlockModelIds_.push_back(MV1LoadModel(path.c_str()));
	
	num = static_cast<int>(Block::TYPE::CRYSTAL);
	path = (PATH + "Block_Crystal.mv1");
	baseBlockModelIds_.push_back(MV1LoadModel(path.c_str()));
	
	num = static_cast<int>(Block::TYPE::BLANK);
	path = (PATH + "Block_Blank.mv1");
	baseBlockModelIds_.push_back(MV1LoadModel(path.c_str()));

	LoadMapCsvData();

#pragma region ベータ授業データ
	/*
	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			// int型の2次元配列から値を取り出す
			int chipNo = mapData_[z][x];

			// int型をキャストして enum classに変換
			Block::TYPE type = static_cast<Block::TYPE>(chipNo);

			// 先に読み込んでいたモデルのハンドルIDを取得する
			int baseModelId = baseBlockModelIds_[chipNo];

			// ブロックを生成
			Block* block = new Block();
			block->Create(type, baseModelId, x, z);

			// ２次元配列にブロッククラスのポインタを格納
			blocks_[z][x] = block;
		}
	}*/
#pragma endregion

}
void BlockManager::Update(void)
{
	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			blocks_[z][x]->Update();
		}
	}
}
void BlockManager::Draw(void)
{
	// マップの描画
	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			blocks_[z][x]->Draw();
		}
	}
}
void BlockManager::Release(void)
{
	// モデルのメモリ解放
	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			blocks_[z][x]->Release();
			delete blocks_[z][x];
		}
	}

	// モデルのメモリ解放
	for (int i = 0; i < baseBlockModelIds_.size(); i++)
	{
		MV1DeleteModel(baseBlockModelIds_[i]);
	}
}

bool BlockManager::IsCollisionLine(VECTOR topPos, VECTOR downPos, MV1_COLL_RESULT_POLY* result)
{
	bool ret = false;

	for (int z = 0; z < NUM_BLOCK_Z; z++)
	{
		for (int x = 0; x < NUM_BLOCK_X; x++)
		{
			// 2次元配列からBlock情報を取り出す
			Block* block = blocks_[z][x];

			// 線分とモデルの衝突判定
			MV1_COLL_RESULT_POLY res =
				MV1CollCheck_Line(block->GetModelId(), -1, topPos, downPos);

			if (res.HitFlag)
			{
				// 結果を返す
				*result = res;
				return true;
			}
		}
	}

	return ret;
}



void BlockManager::LoadMapCsvData(void)
{
	// ファイルの読込
	std::ifstream ifs = std::ifstream(Application::PATH_MAP_DATA);

	// ファイル読み込み失敗
	if (!ifs) return;

	// ファイルを１行ずつ読み込む
	std::string line; // 1行の文字情報
	std::vector<std::string> strSplit; // 1行を1文字の動的配列に分割

	int chipNo = -1;
	int z	   = NUM_BLOCK_Z - 1;

	while (getline(ifs, line))
	{
		// １行をカンマ区切りで分割
		strSplit = AsoUtility::Split(line, ',');

		int size = strSplit.size();

#pragma region plusAlpha
		// 横の要素数が生成最大数を超えたとき、制限
		if (size > NUM_BLOCK_X) size = NUM_BLOCK_X;
#pragma endregion

		for (int x = 0; x < size; x++)
		{
#pragma region PlusAlpha
			if (strSplit[x] == "-1" ||
				chipNo > static_cast<int>(Block::TYPE::BLANK) ||
				chipNo <= -1)
			{
				// 読み込み失敗時、空にする
				chipNo = static_cast<int>(Block::TYPE::BLANK);
			}

			// 文字列が全て数字か否か
			if (std::all_of(strSplit[x].cbegin(),
							strSplit[x].cend(),
							isdigit))
			{
				// stringからintに変換
				chipNo = stoi(strSplit[x]);
			}
			else
			{
				// 文字列の時、空にする
				chipNo = static_cast<int>(Block::TYPE::BLANK);
			}
#pragma endregion

			
			// stringからintに変換
			//chipNo = stoi(strSplit[x]);

			// int型をキャストして enum classに変換
			Block::TYPE type = static_cast<Block::TYPE>(chipNo);

			// 先に読み込んでいたモデルのハンドルIDを取得する
			int baseModelId = baseBlockModelIds_[chipNo];

			// ブロックを生成
			Block* block = new Block();
			block->Create(type, baseModelId, x, z);

			// ２次元配列にブロッククラスのポインタを格納
			blocks_[z][x] = block;
		}
		z--;
	}
}