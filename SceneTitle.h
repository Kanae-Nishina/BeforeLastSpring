/*
	@file	SceneTitle.h
	@brief	�^�C�g���V�[��
	@date	2017/02/21
	@author	�m�ȍ��c
*/

#if 0 �G�������̂ł����������
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