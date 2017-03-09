/*
	@file	SceneTitle.cpp
	@brief		�^�C�g���V�[��
	@date		2017/02/18
	@author	�m�ȍ��c
	@author	���V�M�F(�T�E���h�����A�G�t�F�N�g����)
*/
#include "SceneTitle.h"

/*
	@brief	�R���X�g���N�^
*/
SceneTitle::SceneTitle()
	: m_camera(nullptr)
	,m_uiTitle(nullptr)
	,m_effect(nullptr)
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
	m_effect = new Effect;

	//m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, {(float)window_width,(float)window_height}, 2);
	m_uiTitle->LoadTexture(L"Texture/pipo-2017phoenix.png", { 2,10 }, { (float)640,(float)480 }, 2); // �G�t�F�N�g�m�F�̂��߉摜���k��
	Sound::getInstance().PlayBGM("TEST2"); // BGM�Đ�

	m_effect->Load("Effect/efk/Laser.efk", 1.0f, 1.0f);
}

/*
	@brief	���
*/
void SceneTitle::Destroy()
{
	SAFE_DELETE(m_camera);
	SAFE_DELETE(m_uiTitle);
	SAFE_DELETE(m_effect);
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
	if (GetKeyState(VK_DOWN) & 0x80) 
	{
		Sound::getInstance().StopBGM("TEST2"); // BGM��~
	}
	if (GetKeyState(VK_UP) & 0x80)
	{
		m_effect->Play(D3DXVECTOR3(0, 0, 0), 1.0f);
		Sound::getInstance().PlayBGM("TEST2"); // BGM�Đ�
	}
}

/*
	@brief	�`��
*/
void SceneTitle::Render()
{
	m_uiTitle->Render({ 0,0 }, { 1,1 });
	m_camera->Render();
	m_effect->Render();
}
