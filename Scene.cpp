/*
	@file	Scene.cpp
	@brief	シーンの更新処理
	@date	2017/02/18
	@author	仁科香苗
*/
#include "Scene.h"
#include "SceneRoot.h"

SceneBase* Scene::Update(SceneBase* scene)
{
	SceneRoot* root = dynamic_cast<SceneRoot*>(scene);
	return Update(root);
}