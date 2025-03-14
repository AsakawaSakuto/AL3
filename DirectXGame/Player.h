#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

/// <summary>
/// 実機
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, DebugCamera* camera, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	void MoveControl();
	void JumpControl();
	void TurnControl();
	//  ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//
	DebugCamera* camera_ = nullptr;
	//
	Input* input_ = nullptr; 
	//
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 0.5f;
	//
	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.f;
	float turnTimer_ = 0.f;
	static inline const float kTimeTrun = 0.3f;
	float EaseInOutSide(float easing);

	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 0.1f;
	static inline const float kJumpAcceleration = 1.f;
	bool landing = false;
};
