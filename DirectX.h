/*
	@file	DirectX.h
	@brief	DirectX���g�p����ׂ̃N���X
	@date	2017/02/18
	@author	�m�ȍ��c

	@date		2017/03/03
	@author	���V�M�F(�T�E���h�A�G�t�F�N�g�̃C���N���[�h�ǋL)
*/
#pragma once
#include "Common.h"
#include "SceneRoot.h"
#include "Scene.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sound.h"
#include "Effect/Effect.h"

class DirectX
{
public:
	DirectX();
	~DirectX();

	HRESULT InitD3D(HWND wnd);																		//DirectX�̏�����
	LRESULT MsgProc(HWND wnd, UINT iMsg, WPARAM wParam, LPARAM lParam);	//�E�B���h�E�v���V�[�W��
	void Loop();
private:
	HWND								m_wnd;
	ID3D11Device*					m_device;
	ID3D11DeviceContext*		m_deviceContext;
	IDXGISwapChain*				m_swapChain;
	ID3D11RenderTargetView*	m_backBuffer_TexRTV;
	ID3D11DepthStencilView*	m_backBuffer_DSTexDSV;
	ID3D11Texture2D*				m_backBuffer_DSTex;

	SceneRoot*						m_sceneManager;		//�V�[���}�l�[�W���[
	long									m_startClock;			//�J�n���̃N���b�N��

	void AppInit();							//�A�v���P�[�V�����̏�����
	void Update();							//�A�v���P�[�V�����̍X�V
	void SetCamera();						//�`��ׂ̈̃}�g���b�N�X�ݒ�
	void DestroyD3D();					//�������

	Camera* m_camera;
};

