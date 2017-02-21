/*
	@file	Scene.h
	@brief	各シーンのスーパークラス
	@date	2017/02/18
	@author	仁科香苗
*/
#pragma once
#include "SceneBase.h"

class SceneRoot;

class Scene :public SceneBase
{
public:
	virtual ~Scene() {};

	SceneBase* Update(SceneBase* scene);
	virtual SceneBase* Update(SceneRoot* root)=0;
	virtual void Update() = 0;
};