#include"Skydome.h"

Skydome::Skydome() 
{}

Skydome::~Skydome() 
{
	delete model_;
}

void Skydome::Initialize() 
{ 
	model_ = Model::Create();
}

void Skydome::Update() 
{}

void Skydome::Draw() 
{ 
	model_->Draw(worldTransform_, camera_);
}