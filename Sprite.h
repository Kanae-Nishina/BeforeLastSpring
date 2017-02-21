/*
	@file	Sprite.h
	@brief	�X�v���C�g
	@date	2017/02/20
	@author	�m�ȍ��c
*/
#pragma once
#include "Common.h"
#include "resource.h"

struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
	D3DXVECTOR4 vColor;
	D3DXVECTOR4 fAlpha;
	D3DXVECTOR4 texScroll;
};

struct SimpleVertex
{
	D3DXVECTOR3 Pos;	//�ʒu
	D3DXVECTOR2 vTex;	//�e�N�X�`���[���W
};

struct GrapRect
{
	GrapRect(float top, float bottom, float left, float right) 
	{
		m_top = top;
		m_bottom = bottom;
		m_left = left;
		m_right = right;
	}
	~GrapRect() {}

	float m_top;
	float m_bottom;
	float m_left;
	float m_right;
};

class Sprite
{
private:
	static ID3D11Device*		m_pDevice;
	static ID3D11DeviceContext* m_pDeviceContext;
	static ID3D11SamplerState*	m_pSampleLinear;
	static ID3D11VertexShader*	m_pVertexShader;
	static ID3D11PixelShader*	m_pPixelShader;
	static ID3D11InputLayout*	m_pVertexLayout;
	static ID3D11Buffer*		m_pConstantBuffer;
	static ID3D11BlendState*	m_pBlendState;
	static D3DXVECTOR4			m_color;
	static D3D11_BLEND_DESC		m_dd;
	static D3DXMATRIX			m_view;
	static D3DXMATRIX			m_proj;
	
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11ShaderResourceView*	m_pTexture;
	ID3D11Buffer*				m_RectBuffer;

	D3DXVECTOR2 m_size;				//�A�j���[�V�����e�N�X�`���̉��A�c�T�C�Y
	float		m_scale;			//�X�P�[��
	float		m_yaw;				//y����]
	float		m_pitch;			//x����]
	float		m_roll;				//z����]
	float		m_alpha;			//�A���t�@�l
	int			m_animSpeed;		//�A�j���[�V�����̃X�s�[�h
	int			m_animCount;		//�A�j���[�V�����J�E���g

	static void	StopBlendState() 
	{
		m_dd.RenderTarget[0].BlendEnable = false;
		m_pDevice->CreateBlendState(&m_dd, &m_pBlendState);
	}
	static void StartBlendState() 
	{
		m_dd.RenderTarget[0].BlendEnable = true;
		m_pDevice->CreateBlendState(&m_dd, &m_pBlendState);
	}
public:
	Sprite();
	~Sprite();

	static HRESULT Init(ID3D11DeviceContext* deviceContext);
	static void Destroy();
	static void SetCamera(D3DXMATRIX view,D3DXMATRIX proj);
	HRESULT LoadTexture(LPCWSTR texname, D3DXVECTOR2 texsize, GrapRect _Rect, int animSpeed);
	void Render(D3DXVECTOR2 pos, D3DXVECTOR3 radian,float scale);
};