#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Skydome {
public:
	void Initialize(); // 初期化
	void Update();     // 更新
	void Draw();       // 描画
private:
	DirectXCommon* dxCommon_ = nullptr;  
	WorldTransform worldTransform_; //
	Model* model_ = nullptr;        //
	Camera camera_;
};