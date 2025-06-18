#include "Application.h"
#include <DxLib.h>
#include "./\Manager/InputManager.h"
#include "./Manager/SceneManager.h"
#include "./Common/FrameRate.h"


Application* Application::instance_ = nullptr;

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_MODEL = PATH_DATA + "Model/";
const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";
const std::string Application::PATH_MAP_DATA = PATH_DATA + "MapData/BlockActionData.csv";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("2416026 中川原 諒");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	// FPS制御マネージャ初期化
	FrameRate::CreateInstance();

}

void Application::Run(void)
{
	FrameRate& fps = FrameRate::GetInstance();

	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// FPS更新
		fps.Update();

		// フレームレート制御
		if (fps.GetLimitFrameRate() == false)
		{
			fps.SetFrameRate();

			inputManager.Update();
			sceneManager.Update();

			// 描画処理
			Draw();
		}
	}
}

void Application::Draw()
{
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();


	// シーン描画
	SceneManager::GetInstance().Draw();


	if (CheckHitKey(KEY_INPUT_TAB))
	{
		FrameRate::GetInstance().Draw();
	}

	ScreenFlip();
}

void Application::Destroy(void)
{
	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}


	// FPS管理解放
	FrameRate::GetInstance().Release();

	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	// 入力制御解放
	InputManager::GetInstance().Destroy();

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}
