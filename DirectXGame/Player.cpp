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
	// 移動入力
	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
        // 左右加速度
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 左移動中の右入力
			if (velocity_.x < 0.f) {
				// 速度と逆方向に入力中はブレーキ
				velocity_.x *= (1.f - kAttenuation);
			}
			acceleration.x += kAcceleration;
			// 左右切り替え
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				// 旋回開始時のの角度を記録する
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				// 旋回タイマーに時間を設定する
				turnTimer_ = 0.1f;
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 右移動中の左入力
			if (velocity_.x < 0.f) {
				// 速度と逆方向に入力中はブレーキ
				velocity_.x *= (1.f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
			// 左右切り替え
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				// 旋回開始時のの角度を記録する
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				// 旋回タイマーに時間を設定する
				turnTimer_ = 0.1f;
			}
		}
		// 加速/減速
		velocity_ += acceleration;
		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		// 非入力時は移動減衰をかける
		velocity_.x *= (1.f - kAttenuation);
	}
}

void Player::JumpControl() {
	// 着地フラグ
	bool landing = false;

	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0.f, kJumpAcceleration, 0.f);
			landing = false;
		}
	}
	// 地面との当たり判定
	if (velocity_.y <= 0.f) {
		// Y座標が地面以下なら着地
		if (worldTransform_.translation_.y <= 2.f) {
			landing = true;
		}
	}
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 落下速度
		velocity_ += Vector3(0.f, -kGravityAcceleration, 0.f);
		// 落下速度制限
		velocity_.y = (std::max)(velocity_.y, -kLimitFallSpeed);
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 2.f;
			// 摩擦で横方向速度を減衰
			velocity_.x *= (1.f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.f;
			// 設置状態に移行
			onGround_ = true;
		}
	}
}

void Player::TurnControl() {
	// 左右の自キャラ角度テーブル
	if (turnTimer_ > 0.f) {
		turnTimer_ -= 1.f / 60.f;
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.f, 
			std::numbers::pi_v<float> * 3.f / 2.f,
			};
		// 状態に応じた角度を取得
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		float timeRotate = 1 + turnTimer_ / kTimeTrun;
		// イージング処理
		float easing = Player::EaseInOutSide(timeRotate);
		// タイマーを使って角度補間
		worldTransform_.rotation_.y = std::lerp(turnFirstRotationY_, destinationRotationY, easing);
	}
}

void Player::Initialize(Model* model, Camera* camera, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);
	// 引数をメンバ変数に記録
	model_ = model;
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
	// 行列更新
	Matrix4x4 affinMatrix = Matrix::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = affinMatrix;
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *camera_); 
}