#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

// 前方宣言
class Player;

/// <summary>
/// カメラコントローラー
/// </summary>
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
	
	/// <summary>
	/// 追従対象のセッター
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(Player* target) { target_ = target; }

	/// <summary>
	/// 最初にピッタリ追従するための関数
	/// </summary>
	void Reset();

	/// <summary>
	/// カメラのゲッター
	/// </summary>
	/// <returns>カメラ</returns>
	Camera& GetCamera() { return camera_; }

	// 矩形
	struct Rect {
		float left = 0.f;   // 左端
		float right = 1.f;  // 右端
		float bottom = 0.f; // 下端
		float top = 1.f;    // 上端
	};

	/// <summary>
	/// カメラ移動範囲のセッター
	/// </summary>
	/// <param name="area">矩形(4方向端)</param>
	void SetMovableArea(Rect area) { moveArea_ = area; }

private:
	// カメラ
	Camera camera_;

	// 追従対象
	Player* target_ = nullptr;

	// 追従対象とカメラ座標の差(オフセット)
	Vector3 targetOffset_ = {0.f, 0.f, -15.f};

	// カメラ移動範囲
	Rect moveArea_ = {};

	// カメラの目標座標
	Vector3 targetPos_{};

	// 座標補間割合
	static inline const float kInterpolationRate = 0.15f;

	// 速度掛け率
	static inline const float kVelocityBias = 1.25f;

	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-10.f, 10.f, -5.f, 5.f};
};
