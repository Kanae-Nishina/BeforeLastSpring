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
	:m_pScene(nullptr)
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
	m_pScene = new SceneTitle;
	m_pScene->Init();
}


/*
	@brief	解放
*/
void SceneRoot::Destroy()
{
	delete m_pScene;
	m_pScene = nullptr;
}

/*
	@brief	シーン遷移の更新処理
*/
SceneBase* SceneRoot::Update(SceneBase* scene)
{
	//シーンの実行
	SceneBase* next = m_pScene->Update(this);

	//戻り値が現在のシーンと異なっていればシーン遷移
	if (next != m_pScene)
	{
		Scene* casted = dynamic_cast<Scene*>(next);
		m_pScene->Destroy();
		delete m_pScene;

		m_pScene = casted;
		m_pScene->Init();
	}
	return this;
}

/*
	@brief	現在のシーンの描画
*/
void SceneRoot::Render()
{
	m_pScene->Render();
}
