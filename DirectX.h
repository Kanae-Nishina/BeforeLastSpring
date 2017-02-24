/*
	@file	DirectX.h
	@brief	DirectX���g�p����ׂ̃N���X
	@date	2017/02/18
	@author	�m�ȍ��c
*/
#pragma once
#include "Common.h"
#include "SceneRoot.h"
#include "Scene.h"
#include "Camera.h"
#include "Sprite.h"

class DirectX
{
	HWND								m_hWnd;
	ID3D11Device*					m_pDevice;
	ID3D11DeviceContext*		m_pDeviceContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D*				m_pBackBuffer_DSTex;

	SceneRoot* m_pSceneManaer;		//�V�[���}�l�[�W���[
	long m_lStartClock;								//�J�n���̃N���b�N��

	void AppInit();							//�A�v���P�[�V�����̏�����
	void Update();							//�A�v���P�[�V�����̍X�V
	void SetCamera();						//�`��ׂ̈̃}�g���b�N�X�ݒ�
	void DestroyD3D();					//�������
public:
	DirectX();
	~DirectX();

	HRESULT InitD3D(HWND wnd);																		//DirectX�̏�����
	LRESULT MsgProc(HWND wnd, UINT iMsg, WPARAM wParam, LPARAM lParam);	//�E�B���h�E�v���V�[�W��
	void Loop();																									//�A�v���P�[�V��������
};

