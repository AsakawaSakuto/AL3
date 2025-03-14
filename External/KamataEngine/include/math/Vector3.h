#pragma once

namespace KamataEngine {

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	/// <summary>
	/// 複合加算演算子の作成
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	Vector3& operator+=(const Vector3& num) {
		x = this->x + num.x;
		y = this->y + num.y;
		z = this->z + num.z;
		return *this;
	}

	/// <summary>
	/// 複合減算演算子の作成
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	Vector3& operator-=(const Vector3& num) {
		x = this->x - num.x;
		y = this->y - num.y;
		z = this->z - num.z;
		return *this;
	}

};
} // namespace KamataEngine
