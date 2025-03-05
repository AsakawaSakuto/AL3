#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 
	delete debugCamera_;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();

}

void GameScene::Initialize() {
	// 必須？
	dxCommon_ = DirectXCommon::GetInstance();
    //
	model_ = Model::Create(); 
	// ファイルを指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("cube.jpg");
	//
	const uint32_t kNumBlockHorizontal = 20;
    //
	const float kBlockWidth = 2.f;
	//
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	//
	for (size_t i = 0; i < kNumBlockHorizontal; i++) {
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.f;
	}

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
}

void GameScene::Update() {
	//
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		//
		Matrix4x4 affineMatrix = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
		//
		worldTransformBlock->matWorld_ = affineMatrix;
		//
		worldTransformBlock->TransferMatrix();
	}
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		model_->Draw(*worldTransformBlock, debugCamera_->GetCamera(), textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}