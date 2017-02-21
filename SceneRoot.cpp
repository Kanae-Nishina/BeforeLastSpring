/*
	@file	SceneRoot.cpp
	@brief	�V�[���J�ڂ̃}�l�[�W��
	@date	2017/02/21
	@author	�m�ȍ��c
*/

#include "SceneRoot.h"

#if 0 �G�������̂ł����������
/*
	@brief	�R���X�g���N�^
*/
SceneRoot::SceneRoot()
	:m_scene(nullptr)
{
}

/*
	@brief	�f�X�g���N�^
*/
SceneRoot::~SceneRoot()
{
}

/*
	@brief	������
*/
void SceneRoot::Init()
{
	m_scene = new SceneTitle;
	m_scene->Init();
}

/*
	@brief	���
*/
void SceneRoot::Destroy()
{
	SAFE_DELETE(m_scene);
}

/*
	@brief	�X�V
*/
SceneBase* SceneRoot::Update(SceneBase* scene)
{
	//�V�[���̎��s
	SceneBase* next = m_scene->Update(this);
	//�߂�l�����݂̃V�[���ƈقȂ��Ă�����V�[���؂�ւ�
	if (next != m_scene)
	{
		m_scene->Destroy();
		delete m_scene;
		Scene* casted = dynamic_cast<Scene*>(next);
		m_scene = casted;
		m_scene->Init();
	}
	return this;
}

/*
	@brief	�`��
*/
void SceneRoot::Render()
{
	m_scene->Render();
}
#endif