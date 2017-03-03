/*
	@file	SceneRoot.cpp
	@brief	�V�[���J�ڊǗ��N���X
	@date	2017/02/18
	@author	�m�ȍ��c
*/
#include "SceneRoot.h"

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
	delete m_scene;
	m_scene = nullptr;
}

/*
	@brief	�V�[���J�ڂ̍X�V����
*/
SceneBase* SceneRoot::Update(SceneBase* scene)
{
	//�V�[���̎��s
	SceneBase* next = m_scene->Update(this);

	//�߂�l�����݂̃V�[���ƈقȂ��Ă���΃V�[���J��
	if (next != m_scene)
	{
		Scene* casted = dynamic_cast<Scene*>(next);
		m_scene->Destroy();
		delete m_scene;

		m_scene = casted;
		m_scene->Init();
	}
	return this;
}

/*
	@brief	���݂̃V�[���̕`��
*/
void SceneRoot::Render()
{
	m_scene->Render();
}
