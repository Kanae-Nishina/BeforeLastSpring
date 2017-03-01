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
	: m_camera(nullptr)
	,m_uiTitle(nullptr)
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
	m_camera = new Camera;
	m_uiTitle = new Sprite;
	m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, {(float)window_width,(float)window_height}, 5);
}

/*
	@brief	解放
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_uiTitle);
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
	m_uiTitle->Render({ 0,0 }, { 1,1 });
	m_camera->Render();
}

