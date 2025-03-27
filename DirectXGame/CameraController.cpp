#include "CameraController.h"
#include "Player.h"
void CameraController::Initialize() {
	// カメラの初期化
	camera_.Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&camera_);
}

void CameraController::Update() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 追従対象とオフセットと追従対象の速度からカメラの座標を計算
	targetPos_.x = targetWorldTransform.translation_.x + targetOffset_.x + target_->GetVelocity().x * kVelocityBias;
	targetPos_.y = targetWorldTransform.translation_.y + targetOffset_.y + target_->GetVelocity().y * kVelocityBias;
	targetPos_.z = targetWorldTransform.translation_.z + targetOffset_.z + target_->GetVelocity().z * kVelocityBias;

	// 座標補間によりゆっくりと追従
	camera_.translation_.x = std::lerp(camera_.translation_.x, targetPos_.x, kInterpolationRate);
	camera_.translation_.y = std::lerp(camera_.translation_.y, targetPos_.y, kInterpolationRate);
	camera_.translation_.z = std::lerp(camera_.translation_.z, targetPos_.z, kInterpolationRate);

	// 追従対象が画面外に出ないように補正
	camera_.translation_.x = (std::min)(camera_.translation_.x, target_->GetWorldTransform().translation_.x + margin.right);
	camera_.translation_.x = (std::max)(camera_.translation_.x, target_->GetWorldTransform().translation_.x + margin.left);
	camera_.translation_.y = (std::min)(camera_.translation_.y, target_->GetWorldTransform().translation_.y + margin.top);
	camera_.translation_.y = (std::max)(camera_.translation_.y, target_->GetWorldTransform().translation_.y + margin.bottom);

	// 移動範囲制限
	camera_.translation_.x = (std::min)(camera_.translation_.x, moveArea_.right);
	camera_.translation_.x = (std::max)(camera_.translation_.x, moveArea_.left);
	camera_.translation_.y = (std::min)(camera_.translation_.y, moveArea_.top);
	camera_.translation_.y = (std::max)(camera_.translation_.y, moveArea_.bottom);

	// 行列を更新する
	camera_.UpdateMatrix();
}

void CameraController::Reset() { 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}