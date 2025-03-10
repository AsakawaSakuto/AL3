#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Skydome {
public:
	Skydome();
	~Skydome();
	void Initialize(Model* model, uint32_t textureHandle, DebugCamera* camera); // 初期化
	void Update();                                                              // 更新
	void Draw();                                                                // 描画
private:
	//  ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//
	DebugCamera* camera_ = nullptr;
};