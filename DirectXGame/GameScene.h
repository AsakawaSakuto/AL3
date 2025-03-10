#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include <vector>
#include "Skydome.h"

class GameScene {
public:
	GameScene();
	~GameScene();
	void Initialize(); // 初期化
	void Update();     // 更新
	void Draw();       // 描画
private:
	DirectXCommon* dxCommon_ = nullptr;                              // 汎用
	Model* model_ = nullptr;                                         // 3Dmodel
	Camera camera_;                                                  // カメラ
	DebugCamera* debugCamera_ = nullptr;                             // デバッグカメラ
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_; // 可変個配列
	bool isDebugCameraIsActive_ = false;                             // デバッグカメラの切り替えフラグ
	Input* input_ = nullptr;                                         // 入力受付
	// 
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	uint32_t textureHandleSkydome_ = 0;
};