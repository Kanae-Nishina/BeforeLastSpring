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
	: m_pCamera(nullptr)
	,m_pUItitle(nullptr)
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
	m_pCamera = new Camera;
	m_pUItitle = new Sprite;
	m_pUItitle->LoadTexture(L"Texture/title2.png", { 1000,420 }, { 0,1,1,0 });
}

/*
	@brief	解放
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pUItitle);
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
	m_pUItitle->Render({ 0,0 }, { 1,1 });
	m_pCamera->Render();
}

