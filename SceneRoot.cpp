/*
	@file	SceneRoot.cpp
	@brief	シーン遷移管理クラス
	@date	2017/02/18
	@author	仁科香苗
*/
#include "SceneRoot.h"

/*
	@brief	コンストラクタ
*/
SceneRoot::SceneRoot()
	:m_scene(nullptr)
{
}

/*
	@brief	デストラクタ
*/
SceneRoot::~SceneRoot()
{
}

/*
	@brief	初期化
*/
void SceneRoot::Init()
{
	m_scene = new SceneTitle;
	m_scene->Init();
}


/*
	@brief	解放
*/
void SceneRoot::Destroy()
{
	delete m_scene;
	m_scene = nullptr;
}

/*
	@brief	シーン遷移の更新処理
*/
SceneBase* SceneRoot::Update(SceneBase* scene)
{
	//シーンの実行
	SceneBase* next = m_scene->Update(this);

	//戻り値が現在のシーンと異なっていればシーン遷移
	if (next != m_scene)
	{
		Scene* casted = dynamic_cast<Scene*>(next);
		m_scene->Destroy();
		delete m_scene;

		m_scene = casted;
		m_scene->Init();
	}
	return this;
}

/*
	@brief	現在のシーンの描画
*/
void SceneRoot::Render()
{
	m_scene->Render();
}
