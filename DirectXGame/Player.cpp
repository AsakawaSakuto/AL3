#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle, DebugCamera* camera) {
    // NULLポインタチェック
	assert(model);
	// 引数をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	//
	camera_ = camera;
}

void Player::Update() 
{
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() 
{
	model_->Draw(worldTransform_, camera_->GetCamera(), textureHandle_); 
}