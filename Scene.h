/*
	@file	Scene.h
	@brief	�e�V�[���̊��N���X
	@date	2017/02/21
	@author	�m�ȍ��c
*/
#pragma once
#include "SceneBase.h"
#include "SceneRoot.h"


#if 0 �G�������̂ł����������
class SceneRoot;
class Scene :public SceneBase
{
public:
	virtual ~Scene() {};
	SceneBase* Update(SceneBase* scene);			//���݂̃V�[���̍X�V
	virtual SceneBase* Update(SceneRoot* root) = 0;	//�V�[���J�ڂׂ̈̍X�V
};

/*
	@brief	���݂̃V�[���̍X�V
*/
SceneBase* Scene::Update(SceneBase* scene)
{
	SceneRoot* root = dynamic_cast<SceneRoot*>(scene);
	return Update(root);
}
#endif