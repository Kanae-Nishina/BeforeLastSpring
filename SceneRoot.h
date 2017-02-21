/*
	@file	SceneRoot.h
	@brief	シーン遷移のマネージャ
	@date	2017/02/21
	@author	仁科香苗
*/

#pragma once
#include <process.h>
#include "Common.h"
#include "SceneBase.h"
#include "Scene.h"
#include "SceneTitle.h"

#if 0 エラったのでいったん消す
class SceneRoot :public SceneBase
{
private:
	Scene* m_scene;
	//static bool m_loadFlg;
	//static void Load();
public:
	SceneRoot();
	~SceneRoot();
	void Init();											//初期化
	SceneBase* Update(SceneBase* scene);					//更新
	void Render();											//描画
	void Destroy();											//解放
	//static unsigned int WINAPI ThreadFunc(LPVOID arg);
};

#endif