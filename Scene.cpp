/*
	@file	Scene.cpp
	@brief	�V�[���̍X�V����
	@date	2017/02/18
	@author	�m�ȍ��c
*/
#include "Scene.h"
#include "SceneRoot.h"

SceneBase* Scene::Update(SceneBase* scene)
{
	SceneRoot* root = dynamic_cast<SceneRoot*>(scene);
	return Update(root);
}