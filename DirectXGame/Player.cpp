#define NOMIMAX
#include "Player.h"
#include <numbers>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

#pragma region Matrix
namespace Matrix {
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{};
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;
	return result;
}
// 拡縮
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;
	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float angle) {
	Matrix4x4 result{};

	result.m[0][0] = 1;
	result.m[1][1] = std::cos(angle);
	result.m[1][2] = std::sin(angle);
	result.m[2][1] = -std::sin(angle);
	result.m[2][2] = std::cos(angle);
	result.m[3][3] = 1;
	return result;
}
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float angle) {
	Matrix4x4 result{};
	result.m[0][0] = std::cos(angle);
	result.m[0][2] = -std::sin(angle);
	result.m[1][1] = 1;
	result.m[2][0] = std::sin(angle);
	result.m[2][2] = std::cos(angle);
	result.m[3][3] = 1;
	return result;
}
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float angle) {
	Matrix4x4 result{};
	result.m[0][0] = std::cos(angle);
	result.m[0][1] = std::sin(angle);
	result.m[1][0] = -std::sin(angle);
	result.m[1][1] = std::cos(angle);
	result.m[2][2] = 1;
	result.m[3][3] = 1;
	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 worldMatrix = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return worldMatrix;
}
} // namespace Matrix
#pragma endregion

void Player::MoveControl() {
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			//
			if (velocity_.x < 0.f) {
				velocity_.x *= (1.f - kAttenuation);
			}
			acceleration.x += kAcceleration;
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 1.f;
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			//
			if (velocity_.x < 0.f) {
				velocity_.x *= (1.f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 1.f;
			}
		}
		//
		velocity_ += acceleration;
		//
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		//
		velocity_.x *= (1.f - kAttenuation);
	}
}

void Player::JumpControl() {
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0.f, kJumpAcceleration, 0.f);
			landing = false;
		}
	}
	if (velocity_.y <= 0.f) {
		if (worldTransform_.translation_.y <= 2.f) {
			landing = true;
		}
	}
	if (onGround_) {
		if (velocity_.y > 0.f) {
			onGround_ = false;
		}
	} else {
		velocity_ += Vector3(0.f, -kGravityAcceleration, 0.f);
		velocity_.y = (std::max)(velocity_.y, -kLimitFallSpeed);
		if (landing) {
			worldTransform_.translation_.y = 2.f;
			velocity_.x *= (1.f - kAttenuation);
			velocity_.y = 0.f;
			onGround_ = true;
		}
	}
}

void Player::TurnControl() {
	if (turnTimer_ > 0.f) {
		turnTimer_ -= 1.f / 60.f;
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.f, std::numbers::pi_v<float> * 3.f / 2.f};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		float timeRotate = 1 + turnTimer_ / kTimeTrun;
		float easing = Player::EaseInOutSide(timeRotate);
		worldTransform_.rotation_.y = lerp(turnFirstRotationY_, destinationRotationY, easing);
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, DebugCamera* camera, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);
	// 引数をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.f;
	//
	camera_ = camera;
	input_ = Input::GetInstance();
	velocity_ = {};
}

float Player::EaseInOutSide(float easing) { 
	return -(cosf(float(M_PI) * easing) - 1.f) / 2.f; 
}

void Player::Update() {
	//
	if (onGround_) {
		MoveControl();
	}

	JumpControl();

	TurnControl();
	
	worldTransform_.translation_ += velocity_;
	//
	Matrix4x4 affinMatrix = Matrix::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = affinMatrix;
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, camera_->GetCamera(), textureHandle_); 
}