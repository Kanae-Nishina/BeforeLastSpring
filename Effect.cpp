/*
	@file			Effect.cpp
	@brief		�G�t�F�N�g
	@date		2017/03/04
	@author	���V�M�F
*/

#include "Effect.h"

::Effekseer::Manager*					Effect::m_manager	= nullptr;
::EffekseerRenderer::Renderer*		Effect::m_renderer		= nullptr;
::Effekseer::Handle						Effect::m_handle		= -1;
::Effekseer::Effect*						Effect::m_effect			= nullptr;

/*
	@brief	�R���X�g���N�^
*/
Effect::Effect()
{
}

/*
	@brief	�f�X�g���N�^
*/
Effect::~Effect()
{
}

/*
	@�G�t�F�N�g�̏�����
*/
void Effect::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// �`��p�C���X�^���X�̐���
	m_renderer = ::EffekseerRendererDX11::Renderer::Create(device, deviceContext, 2000);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	m_manager = ::Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
}

/*
	@brief	���
*/
void Effect::Destory()
{
}

/*
	@brief	efk�t�@�C���̓ǂݍ���
*/
void Effect::Load(string path, float size,float playSpeed)
{
	// string��wchar_t*�ɕύX
	size_t len = path.length() + 1;									// ������̒���
	char* PathC = new char[len];										// �������p��
	memcpy(PathC, path.c_str(), len);								// char*�ɒ������R�s�[

	len = strlen(PathC) + 1;												// ���߂ăT�C�Y�m��
	wchar_t* pathWC = new wchar_t[len];						// wchar_t�̃T�C�Y�m��

	mbstowcs_s(&len, pathWC, len, PathC, _TRUNCATE);;	// char* �� wchar_t�ɕϊ�

	m_manager->SetScale(m_handle, size, size, size);		// �G�t�F�N�g�̃T�C�Y

	// �G�t�F�N�g�̓Ǎ�
	m_effect = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)pathWC);
}

/*
	@�G�t�F�N�g�̕`��
*/
void Effect::Render()
{
	m_manager->Update();					// �G�t�F�N�g�̍X�V�������s��

	m_renderer->BeginRendering();		// �G�t�F�N�g�̕`��J�n�������s���B
	m_manager->Draw();					// �G�t�F�N�g�̕`����s���B
	m_renderer->EndRendering();		// �G�t�F�N�g�̕`��I���������s���B
}

#if 0 ��ɕK�v�ȏ���
/*
	@�G�t�F�N�g���v���C���[�̌����ɍ��킹��Yaw��ݒ�
*/
void Effect::SetRotation(string name, D3DXVECTOR3 pos, float yaw)
{
	::Effekseer::Vector3D r_pos(pos.x, pos.y, pos.z);
	m_manager->SetRotation(m_handle, r_pos, yaw);
}

/*
	@�G�t�F�N�g�̍Đ��X�s�[�h�̐ݒ�
*/
void Effect::PlaySpeed(string name, float playSpeed)
{

}
#endif

/*
	@�G�t�F�N�g�Đ�
*/
void Effect::Play(D3DXVECTOR3 pos, float angle)
{
	m_handle = m_manager->Play(m_effect, pos.x, pos.y, pos.z);
}

/*
	���G�t�F�N�g��~
*/
void Effect::Stop()
{
	m_manager->StopEffect(m_handle);
}

/*
	@�G�t�F�N�g�̃J�����ݒ�
*/
void Effect::SetCamera(D3DXVECTOR3 pivotPos, D3DXVECTOR3 lookAtPos)
{
	Effekseer::Vector3D Pos = ::Effekseer::Vector3D(pivotPos.x, pivotPos.y, pivotPos.z);
	const Effekseer::Vector3D LookAtPos = ::Effekseer::Vector3D(lookAtPos.x, lookAtPos.y, lookAtPos.z);
	const Effekseer::Matrix44 View = ::Effekseer::Matrix44().LookAtLH(Pos, LookAtPos, ::Effekseer::Vector3D(0, 1, 0));
	const Effekseer::Matrix44 Proj = ::Effekseer::Matrix44().PerspectiveFovLH(D3DX_PI / 6, (FLOAT)window_width / (FLOAT)window_height, 0.1f, 1000.0f);

	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(Proj);

	// �J�����s���ݒ�
	m_renderer->SetCameraMatrix(View);
}