#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include "Player.h"

class GameScene {
public:
	GameScene();
	~GameScene();
	void Initialize(); // 初期化
	void Update();     // 更新
	void Draw();       // 描画
private:
	DirectXCommon* dxCommon_ = nullptr;
	uint32_t textureHandle_ = 0; // テクスチャハンドル
	Model* model_ = nullptr;     // 3dモデル

	Input* input_ = nullptr; // 汎用

	Camera camera_;                      // カメラ
	WorldTransform worldTransform_;      // ワールドトランスフォーム
	DebugCamera* debugCamera_ = nullptr; // デバッグカメラ

	Player* player_ = nullptr; //実機
};