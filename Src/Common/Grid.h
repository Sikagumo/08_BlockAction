#pragma once
#include <DxLib.h>

class Grid
{
public:

	// 線の長さ
	static constexpr float LEN = 3000.0f;

	// 線の長さの半分
	static constexpr float HLEN = (LEN / 2.0f);

	// 線の間隔
	static constexpr float TERM = 100.0f;

	// 線の数
	static const int NUM = static_cast<int>(LEN / TERM);

	// 線の数の半分
	static const int HNUM = (NUM / 2);

	// 球体の半径
	static constexpr float SPHERE_RAD = 25.0f;

	// 球体の滑らかさ
	static constexpr int SPHERE_DIV = 10;


	// コンストラクタ
	Grid(void);

	// デストラクタ
	~Grid(void);


	void Init(void);
	
	void Update(void);

	void Draw(void);

	void Release(void);

private:

	/// <summary>
	/// グリッド線描画処理
	/// </summary>
	/// <param name="startPos">開始位置</param>
	/// <param name="endPos">終了位置</param>
	/// <param name="coler">色</param>
	void DrawGridLine(VECTOR& startPos, VECTOR& endPos, unsigned int coler);

};