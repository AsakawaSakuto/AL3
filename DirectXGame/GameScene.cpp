#include "GameScene.h"

#pragma region Matrix
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
#pragma endregion

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete player_;
	delete skydome_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance(); // 必須？
	input_ = Input::GetInstance();            // 入力

	/*-----------
        カメラ	
	-----------*/
	camera_.Initialize();                                        // カメラの初期化
	PrimitiveDrawer::GetInstance()->SetViewProjection(&camera_);
	debugCamera_ = new DebugCamera(1280, 720);                   // デバッグカメラの生成
	

	/*-----------
	   ブロック
	-----------*/

	// 3dモデル
    modelBlock_ = Model::Create();

	/*-----------
	     天球
	-----------*/
	skydome_ = new Skydome();                              // new 
	modelSkydome_ = Model::CreateFromOBJ("skydome", true); // モデル読み込み
	skydome_->Initialize(modelSkydome_, debugCamera_);     // 初期化

	/*-----------
	  プレイヤー
	-----------*/
	player_ = new Player();                                                // new
	modelPlayer_= Model::Create();                                         // モデル生成
	textureHandlePlayer_ = TextureManager::Load("uvChecker.png");          // 画像読み込み
	Vector3 playerPos = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(modelPlayer_, textureHandlePlayer_, debugCamera_, playerPos); // 初期化

	/*-----------
	  マップチップ
	-----------*/
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
}

void GameScene::Update() {
	/*-----------
	 ブロックの更新
	-----------*/ 
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			} else {
				// アフィン変換行列の作成
				Matrix4x4 affineMatrix = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// ブロックに代入
				worldTransformBlock->matWorld_ = affineMatrix;
				// 定数バッファに送信する
				worldTransformBlock->TransferMatrix();
			}
		}
	}

	// プレイヤー更新
	player_->Update();

	// 天球更新
	skydome_->Update();

	/*----------
	 カメラの処理
	----------*/
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) { // SPACEでフラグの切り替え
		if (isDebugCameraIsActive_) {
			isDebugCameraIsActive_ = false;
		} else {
			isDebugCameraIsActive_ = true;
		}
	}
	if (isDebugCameraIsActive_) {
		debugCamera_->Update();                                          // デバッグカメラ更新
		camera_.matView = debugCamera_->GetCamera().matView;             // ビュー行列を代入
		camera_.matProjection = debugCamera_->GetCamera().matProjection; // プロジェクション行列を代入
		camera_.TransferMatrix();                                        // カメラ行列の転送
	} else {
		camera_.UpdateMatrix();                                          // カメラ行列の更新と転送
	}
#endif // _DEBUG
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			} else {
				modelBlock_->Draw(*worldTransformBlock, camera_);
			}
		}
	}

	skydome_->Draw(); // 天球
	player_->Draw();  // プレイヤー

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->lkNumBlockVirtical;
	uint32_t numBlockHorizontal = mapChipField_->lkNumBlockHorizontal;
	// 要素数を変更する
	// 列数を設定(縦方向)
	worldTransformBlocks_.resize(numBlockHorizontal);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		// 1列の要素数を設定 (横方向)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapTypeByIndex(j,i)==MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}