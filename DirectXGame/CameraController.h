#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Player;

class CameraController {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }
	void Reset();
	Camera& GetCamera() { return camera_; }

	struct Rect {
		float left = 0.f;
		float right = 1.f;
		float bottom = 0.f;
		float top = 1.f;
	};

	void SetMovableArea(Rect area) { moveArea_ = area; }

private:
	Camera camera_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0.f, 0.f, -15.f};
	Rect moveArea_ = {0.f, 100.f, 0.f, 100.f};
	Vector3 targetPos_{};
	static inline const float kInterpolationRate = 0.15f;
	static inline const float kVelocityBias = 1.25f;
	static inline const Rect margin = {-10.f, 10.f, -5.f, 5.f};
};
