#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class GameScene
{
public:
	GameScene();
	~GameScene();
	void Initialize(); // 初期化
	void Update();     // 更新
	void Draw();       // 描画
private:
	DirectXCommon* dxCommon_ = nullptr;
	uint32_t textureHandle_ = 0; // テクスチャハンドル
	Sprite* sprite_ = nullptr;   // スプライト
	Model* model_ = nullptr;     // 3dモデル

	uint32_t fanfareSoundHandle_ = 0; // サウンドデータハンドル
	uint32_t fanfareVoiceHandle_ = 0; // 音声再生ハンドル
	Audio* audio_ = nullptr;

	Input* input_ = nullptr; // 汎用

	float inputFloat[3] = {}; // ImGuiで値を入力する変数

	Camera camera_;                      // カメラ
	WorldTransform worldTransform_;      // ワールドトランスフォーム
	DebugCamera* debugCamera_ = nullptr; // デバッグカメラ
};