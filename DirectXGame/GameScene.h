#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include <vector>
#include "Player.h"
#include "Skydome.h"
#include "MapCipField.h"
#include "CameraController.h"

class GameScene {
public:
	GameScene();
	~GameScene();
	void Initialize(); // 初期化
	void Update();     // 更新
	void Draw();       // 描画
private:
	DirectXCommon* dxCommon_ = nullptr;                              // 汎用
	Camera camera_;                                                  // カメラ
	DebugCamera* debugCamera_ = nullptr;                             // デバッグカメラ
	bool isDebugCameraIsActive_ = false;                             // デバッグカメラの切り替えフラグ
	Input* input_ = nullptr;                                         // 入力受付

	// ブロック関連
	Model* modelBlock_ = nullptr;                                    // 3Dmodel
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_; // 可変個配列
	
	// 天球関連
	Skydome* skydome_ = nullptr;    // クラス
	Model* modelSkydome_ = nullptr; // 天球

	// プレイヤー関連
	Player* player_ = nullptr;          // クラス
	Model* modelPlayer_ = nullptr;      // 3dモデル
	uint32_t textureHandlePlayer_ = 0u; // 画像ハンドル

	// マップチップ
	MapChipField* mapChipField_ = nullptr;
	void GenerateBlocks(); // ブロックの生成

	//
	CameraController* cameraController_ = nullptr;
};