#include "Grid.h"

Grid::Grid(void)
{
	Init();
}

Grid::~Grid(void)
{
}

void Grid::Init(void)
{
	
}

void Grid::Update(void)
{
}

void Grid::Draw(void)
{
	float num;	  // 線の値
	VECTOR sPos;  // 開始位置
	VECTOR ePos;  // 終了位置

	for (int i = -HNUM; i < HNUM; i++)
	{
		num = static_cast<int>(i);

		// X軸(赤)
		sPos = { -HLEN, 0.0f, (num * TERM) };
		ePos = {  HLEN, 0.0f, (num * TERM) };
		DrawGridLine(sPos, ePos, 0xFF0000);

		// Z軸(青)
		sPos = { (num * TERM), 0.0f, -HLEN };
		ePos = { (num * TERM), 0.0f,  HLEN };
		DrawGridLine(sPos, ePos, 0x0000FF);
	}

	// Y軸(緑)
	sPos = { 0.0f, 0.0f, 0.0f };
	ePos = { 0.0f, LEN , 0.0f };
	DrawGridLine(sPos, ePos, 0x00FF00);
}

void Grid::Release(void)
{
}

void Grid::DrawGridLine(VECTOR& startPos, VECTOR& endPos, unsigned int coler)
{
	/*　グリッド線描画処理　*/

	DrawLine3D(startPos, endPos, coler);
	DrawSphere3D(endPos, SPHERE_RAD, SPHERE_DIV, coler, coler, true);
}
