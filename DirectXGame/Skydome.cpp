#include"Skydome.h"

void Skydome::Initialize() 
{ 
	// 必須？
	dxCommon_ = DirectXCommon::GetInstance();
	model_ = Model::Create();
}

void Skydome::Update() 
{}

void Skydome::Draw() 
{ 
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	model_->Draw(worldTransform_, camera_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}