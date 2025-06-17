#pragma once
#include <chrono>
class SceneBase;
class Camera;
class Fader;
class Grid;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="nextId">遷移後のシーン状態</param>
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void)const;

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	/// <summary>
	/// カメラ取得
	/// </summary>
	/// <param name=""></param>
	Camera& GetCamera(void);


private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;	   // 現在シーン
	SCENE_ID waitSceneId_; // 待機シーン

	// フェード
	Fader* fader_;

	// カメラ
	Camera* camera_;

	// 各種シーン
	SceneBase* scene_;

	// グリッド線
	Grid* grid_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);

	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;

	// デストラクタも同様
	~SceneManager(void) = default;
	
	// 3D初期化処理
	void Init3D(void);

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

};