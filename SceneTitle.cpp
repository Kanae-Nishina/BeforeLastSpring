/*
	@file	SceneTitle.cpp
	@brief		タイトルシーン
	@date		2017/02/18
	@author	仁科香苗
	@author	金澤信芳(サウンド実装、エフェクト実装)
*/
#include "SceneTitle.h"

/*
	@brief	コンストラクタ
*/
SceneTitle::SceneTitle()
	: m_camera(nullptr)
	,m_uiTitle(nullptr)
	,m_effect(nullptr)
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
	m_effect = new Effect;

	//m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, {(float)window_width,(float)window_height}, 2);
	m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, { (float)640,(float)480 }, 2); // エフェクト確認のため画像を縮小
	Sound::getInstance().PlayBGM("TEST2"); // BGM再生

	m_effect->Load("Effect/efk/Laser.efk", 1.0f, 1.0f);
}

/*
	@brief	解放
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_uiTitle);
	SAFE_DELETE(m_effect);
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
	if (GetKeyState(VK_DOWN) & 0x80) 
	{
		Sound::getInstance().StopBGM("TEST2"); // BGM停止
	}
	if (GetKeyState(VK_UP) & 0x80)
	{
		m_effect->Play(D3DXVECTOR3(0, 0, 0), 1.0f);
		Sound::getInstance().PlayBGM("TEST2"); // BGM再生
	}
}

/*
	@brief	描画
*/
void SceneTitle::Render()
{
	m_uiTitle->Render({ 0,0 }, { 1,1 });
	m_camera->Render();
	m_effect->Render();
}
