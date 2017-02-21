/*
	@file	Scene.h
	@brief	�e�V�[���̃X�[�p�[�N���X
	@date	2017/02/18
	@author	�m�ȍ��c
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