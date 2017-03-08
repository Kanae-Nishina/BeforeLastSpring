/*
	@file				SceneTitle.h
	@brief			タイトルシーン
	@date			2017/02/18
	@author		仁科香苗

	@date			2017/03/05
	@author		金澤信芳(エフェクトのインクルード追記)
*/
#pragma once
#include "Scene.h"
#include "SceneRoot.h"
#include "SceneMain.h"
#include "Camera.h"
#include "Sprite.h"
#include "Effect/Effect.h"

class SceneTitle:public Scene
{
public:
	SceneTitle();
	~SceneTitle();

	void Init();
	SceneBase* Update(SceneRoot* root);
	void Render();
	void Destroy();
private:
	Sprite* m_uiTitle;
	Camera* m_camera;
	Effect* m_effect;
	void Update();
};