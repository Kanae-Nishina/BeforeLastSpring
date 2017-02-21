/*
	@file	Scene.h
	@brief	各シーンの基底クラス
	@date	2017/02/21
	@author	仁科香苗
*/
#pragma once
#include "SceneBase.h"
#include "SceneRoot.h"


#if 0 エラったのでいったん消す
class SceneRoot;
class Scene :public SceneBase
{
public:
	virtual ~Scene() {};
	SceneBase* Update(SceneBase* scene);			//現在のシーンの更新
	virtual SceneBase* Update(SceneRoot* root) = 0;	//シーン遷移の為の更新
};

/*
	@brief	現在のシーンの更新
*/
SceneBase* Scene::Update(SceneBase* scene)
{
	SceneRoot* root = dynamic_cast<SceneRoot*>(scene);
	return Update(root);
}
#endif