/*
	@file	SceneRoot.h
	@brief	�V�[���J�ڂ̃}�l�[�W��
	@date	2017/02/21
	@author	�m�ȍ��c
*/

#pragma once
#include <process.h>
#include "Common.h"
#include "SceneBase.h"
#include "Scene.h"
#include "SceneTitle.h"

#if 0 �G�������̂ł����������
class SceneRoot :public SceneBase
{
private:
	Scene* m_scene;
	//static bool m_loadFlg;
	//static void Load();
public:
	SceneRoot();
	~SceneRoot();
	void Init();											//������
	SceneBase* Update(SceneBase* scene);					//�X�V
	void Render();											//�`��
	void Destroy();											//���
	//static unsigned int WINAPI ThreadFunc(LPVOID arg);
};

#endif