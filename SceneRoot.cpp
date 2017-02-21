/*
	@file	SceneRoot.cpp
	@brief	シーン遷移のマネージャ
	@date	2017/02/21
	@author	仁科香苗
*/

#include "SceneRoot.h"

#if 0 エラったのでいったん消す
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
	SAFE_DELETE(m_scene);
}

/*
	@brief	更新
*/
SceneBase* SceneRoot::Update(SceneBase* scene)
{
	//シーンの実行
	SceneBase* next = m_scene->Update(this);
	//戻り値が現在のシーンと異なっていたらシーン切り替え
	if (next != m_scene)
	{
		m_scene->Destroy();
		delete m_scene;
		Scene* casted = dynamic_cast<Scene*>(next);
		m_scene = casted;
		m_scene->Init();
	}
	return this;
}

/*
	@brief	描画
*/
void SceneRoot::Render()
{
	m_scene->Render();
}
#endif