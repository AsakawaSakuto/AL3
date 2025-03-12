#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;


enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.f;
	static inline const float kBlockHeight = 2.f;
	// ブロックの個数
	static inline const uint32_t lkNumBlockVirtical = 20;
	static inline const uint32_t lkNumBlockHorizontal = 100;

	MapChipData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
};