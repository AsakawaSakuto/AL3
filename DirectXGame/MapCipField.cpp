#include "MapCipField.h"
#include <map>
#include <fstream>
#include <sstream>

namespace {
   std::map<std::string, MapChipType> mapCipTable = {
       {"0", MapChipType::kBlank},
       {"1", MapChipType::kBlock},
   };
}

void MapChipField::ResetMapChipData() {
    // マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(lkNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(lkNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) { 
	// マップチップデータをリセット
	ResetMapChipData();
	
	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列にストリームコピー
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < lkNumBlockVirtical; i++) {
		std::string line;
		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < lkNumBlockHorizontal; j++) {
			
			std::string word;
			getline(line_stream, word, ',');

			if (mapCipTable.contains(word)) {
				mapChipData_.data[i][j] = mapCipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || lkNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
    if (yIndex<0||lkNumBlockVirtical-1<yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { 
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (lkNumBlockVirtical - 1 - yIndex), 0);
}