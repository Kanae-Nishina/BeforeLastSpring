#pragma once
/*
	@file	SceneMain.cpp
	@brief	ƒƒCƒ“ƒV[ƒ“
	@date	2017/02/21
	@author	m‰È•c
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

