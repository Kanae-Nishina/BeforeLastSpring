/*
	@file	SceneRoot.h
	@brief	�V�[���J�ڂ̊Ǘ��N���X
	@date	2017/02/18
	@author	�m�ȍ��c
*/
#pragma once
#include "Scene.h"
#include "SceneTitle.h"

class SceneRoot :public SceneBase
{
private:
	Scene* m_pScene;		//���݂̃V�[��
public:
	SceneRoot();
	~SceneRoot();

	void Init();							//������
	SceneBase* Update(SceneBase* scene);	//�X�V
	void Render();							//�`��
	void Destroy();							//���
};
