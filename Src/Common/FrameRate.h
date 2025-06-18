/* -------------------------
　フレームレート制御系データ
 --------------------------- */

class FrameRate
{
public:

	// 平均フレームレート
	static constexpr int FRAME_RATE_AVE = 60;

	// 設定フレームレート(ミリ秒単位)
	static constexpr float FRAME_RATE = (1000 / FRAME_RATE_AVE);

	// テキスト横位置
	static constexpr int TEXT_POS_X = 70;

	// テキスト縦位置
	static constexpr int TEXT_POS_Y = 25;


	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>フレームレートマネージャ</returns>
	static FrameRate& GetInstance(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// インスタンス削除処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// フレームレート割り当て
	/// </summary>
	void SetFrameRate(void);

	/// <summary>
	/// フレームレート制限 判定
	/// </summary>
	/// <returns>制御するか否か</returns>
	bool GetLimitFrameRate(void);


private:

	// 静的インスタンス
	static FrameRate* manager_;

	// 現在の時間
	int curTime_;

	// 前フレーム実行時間
	int lateTime_;

	// フレームカウンタ
	int counter_;

	// フレーム更新時間
	int updateTime_;

	// フレームレート(描画用)
	float viewFramelate_;


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	FrameRate(void);

	/// <summary>
	/// デストラクタ(private)
	/// </summary>
	~FrameRate(void) = default;

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	FrameRate(FrameRate& other) = default;
};