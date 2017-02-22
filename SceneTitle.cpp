/*
	@file	SceneTitle.h
	@brief	タイトルシーン
	@date	2017/02/18
	@author	仁科香苗
*/
#include "SceneTitle.h"

/*
	@brief	コンストラクタ
*/
SceneTitle::SceneTitle()
{
}

/*
	@brief	デストラクタ
*/
SceneTitle::~SceneTitle()
{
}

/*
	@brief	初期化
*/
void SceneTitle::Init()
{
}

/*
	@brief	解放
*/
void SceneTitle::Destroy()
{
}

/*
	@brief	シーンの更新
*/
SceneBase* SceneTitle::Update(SceneRoot* root)
{
	SceneBase* next = this;
	Update();
	return next;
}

/*
	@brief	更新
*/
void SceneTitle::Update()
{

}

/*
	@brief	描画
*/
void SceneTitle::Render()
{

}

