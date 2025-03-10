#include"Skydome.h"

Skydome::Skydome() 
{}

Skydome::~Skydome() 
{
	delete model_;
}

void Skydome::Initialize(Model* model, DebugCamera* camera){ 
		// NULLポインタチェック
	assert(model);
	// 引数をメンバ変数に記録
	model_ = model;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	//
	camera_ = camera;
}

void Skydome::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() { 
	model_->Draw(worldTransform_, camera_->GetCamera());
}