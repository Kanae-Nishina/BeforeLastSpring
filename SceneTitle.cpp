/*
	@file	SceneTitle.h
	@brief		タイトルシーン
	@date		2017/02/18
	@author	仁科香苗

	@brief		タイトルにテストサウンド実装
	@date		2017/03/03
	@author	金澤信芳
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
	//m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, {(float)window_width,(float)window_height}, 2);
	m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, { (float)640,(float)480 }, 2); // エフェクト確認のため画像を縮小
	Sound::getInstance().BGM_play("TEST2"); // BGM再生
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

