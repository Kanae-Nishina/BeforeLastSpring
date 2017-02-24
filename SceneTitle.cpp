/*
	@file	SceneTitle.h
	@brief	�^�C�g���V�[��
	@date	2017/02/18
	@author	�m�ȍ��c
*/
#include "SceneTitle.h"

/*
	@brief	�R���X�g���N�^
*/
SceneTitle::SceneTitle()
	: m_pCamera(nullptr)
	,m_pUItitle(nullptr)
{
}

/*
	@brief	�f�X�g���N�^
*/
SceneTitle::~SceneTitle()
{
}

/*
	@brief	������
*/
void SceneTitle::Init()
{
	m_pCamera = new Camera;
	m_pUItitle = new Sprite;
	m_pUItitle->LoadTexture(L"Texture/title2.png", { 1000,420 }, { 0,1,1,0 });
}

/*
	@brief	���
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pUItitle);
}

/*
	@brief	�V�[���̍X�V
*/
SceneBase* SceneTitle::Update(SceneRoot* root)
{
	SceneBase* next = this;
	Update();
	return next;
}

/*
	@brief	�X�V
*/
void SceneTitle::Update()
{

}

/*
	@brief	�`��
*/
void SceneTitle::Render()
{
	m_pUItitle->Render({ 0,0 }, { 1,1 });
	m_pCamera->Render();
}

