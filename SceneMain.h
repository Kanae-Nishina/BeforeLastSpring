#pragma once
/*
	@file	SceneMain.cpp
	@brief	���C���V�[��
	@date	2017/02/21
	@author	�m�ȍ��c
*/
#pragma once
#include "Scene.h"
#include "SceneRoot.h"
#include "SceneTitle.h"

class SceneMain:public Scene
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	SceneBase* Update(SceneRoot* root);
	void Render();
	void Destroy();
};

