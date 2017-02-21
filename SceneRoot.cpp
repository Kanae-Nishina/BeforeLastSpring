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
	:m_pScene(nullptr)
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
	m_pScene = new SceneTitle;
	m_pScene->Init();
}


/*
	@brief	���
*/
void SceneRoot::Destroy()
{
	delete m_pScene;
	m_pScene = nullptr;
}

/*
	@brief	�V�[���J�ڂ̍X�V����
*/
SceneBase* SceneRoot::Update(SceneBase* scene)
{
	//�V�[���̎��s
	SceneBase* next = m_pScene->Update(this);

	//�߂�l�����݂̃V�[���ƈقȂ��Ă���΃V�[���J��
	if (next != m_pScene)
	{
		Scene* casted = dynamic_cast<Scene*>(next);
		m_pScene->Destroy();
		delete m_pScene;

		m_pScene = casted;
		m_pScene->Init();
	}
	return this;
}

/*
	@brief	���݂̃V�[���̕`��
*/
void SceneRoot::Render()
{
	m_pScene->Render();
}
