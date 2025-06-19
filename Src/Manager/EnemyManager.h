#pragma once
#include <map>
#include <vector>
#include <string>
#include "../Application.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Shot/ShotBase.h"
class EnemyBase;
class Player;


class EnemyManager
{
public:

	enum class WAVE
	{
		WAVE01,
		WAVE02,
		WAVE03,
		END,
	};

	// モデルパス
	const std::string PATH_DEMON  = Application::PATH_MODEL + "Enemy/Demon.mv1";
	const std::string PATH_WIZARD = Application::PATH_MODEL + "Enemy/Wizard.mv1";
	const std::string PATH_GIANT  = Application::PATH_MODEL + "Enemy/Giant.mv1";


	const std::string PATH_FIRE = Application::PATH_MODEL + "Effect/FireBall/FireBall.mv1";
	const std::string PATH_ROCK = Application::PATH_MODEL + "Effect/Rockfall/Rock.mv1";


	// 生成時間
	static constexpr float SPAWN_WAVE_01  = 3.0f;
	static constexpr float SPAWN_WAVE_02  = 4.0f;

	static constexpr float WAVE_TIME_STATE = 12.5f;




	EnemyManager(Player* player);

	~EnemyManager(void) = default;

	void Init(void);

	void Update(void);

	void Draw(void);

	void Release(void);

	// 敵リストを取得
	const std::map<int, EnemyBase*>& GetEnemys(void);

	/// <summary>
	/// ウェーブクリア
	/// </summary>
	bool GetWaveEnd(void);


private:

	WAVE wave_;


	// 敵リスト
	std::map<int, EnemyBase*> enemys_;

	// 敵のモデルハンドルID
	std::map<int, int> modelIdsEnemy_;

	// エフェクトのモデルハンドルID
	std::map<int, int> modelIdsAtkEffect_;


	// プレイヤー(参照)
	Player* player_;


	// 出現カウンタ
	float timeSpawn_;

	// 生成数
	int spawnCount_;

	// ウェーブ時間
	float timeWave_;


	void ChangeWave(void);

	void UpdateWave01(void);
	void UpdateWave02(void);
	void UpdateWave03(void);
	void UpdateWaveEnd(void);

	void ChangeWave02(void);
	void ChangeWave03(void);

	/// <summary>
	/// 敵の読み込み処理
	/// </summary>
	void LoadEnemy(void);

	/// <summary>
	/// 敵生成処理
	/// </summary>
	/// <param name="type">生成する敵のタイプ</param>
	void SpawnEnemy(int type);
};