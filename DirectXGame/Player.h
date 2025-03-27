#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

enum class LRDirection {
	kRight,
	kLeft,
};

/// <summary>
/// 実機
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Camera* camera, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

private:
	// 移動の処理
	void MoveControl();

	// ジャンプの処理
	void JumpControl();

	// 実機の左右回転の処理
	void TurnControl();

	// イージング
	float EaseInOutSide(float easing);

	WorldTransform worldTransform_ = {}; // ワールド変換データ
	Model* model_ = nullptr;        // モデル
	Camera* camera_ = nullptr; // デバッグカメラ
	Input* input_ = nullptr;        // 入力
	Vector3 velocity_ = {};         // 速度

	static inline const float kAcceleration = 0.01f; // 加速度
	static inline const float kAttenuation = 0.01f;  // 移動減衰
	static inline const float kLimitRunSpeed = 0.5f; // 最大速度制限
	
	LRDirection lrDirection_ = LRDirection::kRight; // 実機の向き

	float turnFirstRotationY_ = 0.f;            // 旋回開始時の角度
	float turnTimer_ = 0.f;                     // 旋回タイマー
	static inline const float kTimeTrun = 0.3f; // 旋回時間

	bool onGround_ = true;                                 // 設置状態フラグ
	static inline const float kGravityAcceleration = 0.1f; // 重力加速度(下)
	static inline const float kLimitFallSpeed = 0.1f;      // 最大落下速度(下)
	static inline const float kJumpAcceleration = 1.f;     // ジャンプ初速(上)

};
