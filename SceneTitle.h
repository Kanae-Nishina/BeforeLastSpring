/*
	@file	SceneTitle.h
	@brief	タイトルシーン
	@date	2017/02/21
	@author	仁科香苗
*/

#if 0 エラったのでいったん消す
#pragma once
#include "Scene.h"
#include "SceneRoot.h"
class SceneTitle :public Scene
{
public:
	SceneTitle();
	~SceneTitle();

	void Init();
	SceneBase* Update(SceneRoot* root);
	void Render();
	void Destroy();
};

#endif 