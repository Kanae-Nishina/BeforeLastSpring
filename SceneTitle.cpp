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
	: m_camera(nullptr)
	,m_uiTitle(nullptr)
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
	m_camera = new Camera;
	m_uiTitle = new Sprite;
	m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, {(float)window_width,(float)window_height}, 5);
}

/*
	@brief	���
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_uiTitle);
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
	m_uiTitle->Render({ 0,0 }, { 1,1 });
	m_camera->Render();
}

