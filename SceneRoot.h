/*
	@file	SceneRoot.h
	@brief	シーン遷移の管理クラス
	@date	2017/02/18
	@author	仁科香苗
*/
#pragma once
#include "Scene.h"
#include "SceneTitle.h"

class SceneRoot :public SceneBase
{
private:
	Scene* m_pScene;		//現在のシーン
public:
	SceneRoot();
	~SceneRoot();

	void Init();							//初期化
	SceneBase* Update(SceneBase* scene);	//更新
	void Render();							//描画
	void Destroy();							//解放
};
