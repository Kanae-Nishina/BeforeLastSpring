/*
	@file			Sprite.h
	@brief		2Dスプライト
	@date		2017/02/22
	@author	仁科香苗
*/
#pragma once
#include "Common.h"
#include "resource.h"

struct SPRITE_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
	D3DXVECTOR4 vColor;
	D3DXVECTOR4 fAlpha;
	D3DXVECTOR4 TexScroll;
};

struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
	D3DXVECTOR2 vTex; //テクスチャー座標
};

struct GrapRect
{
	GrapRect(float top, float bottom, float left, float right) {
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
	static ID3D11Device*					m_pDevice;
	static ID3D11DeviceContext*		m_pDeviceContext;
	static ID3D11SamplerState*		m_pSampleLinear;
	static ID3D11VertexShader*		m_pVertexShader;
	static ID3D11PixelShader*			m_pPixelShader;
	static ID3D11InputLayout*			m_pVertexLayout;
	static ID3D11Buffer*					m_pConstantBuffer;
	static ID3D11BlendState*			m_pBlendState;
	static D3D11_BLEND_DESC			m_dd;
	static D3DXMATRIX					m_mView;
	static D3DXMATRIX					m_mProj;
	static D3DXVECTOR4					m_vColor;
	static float									m_fAlpha;

	ID3D11Buffer*						m_pVertexBuffer;
	ID3D11ShaderResourceView*	m_pTexture;
	ID3D11Buffer*						m_RectBuffer;
	float										m_fScale;				//25pixelを基準 25pixel=1.0f
	int											m_iAnimSpeed;
	int											m_iAnimCount;
	D3DXVECTOR2						m_Size;

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

	static HRESULT Init(ID3D11DeviceContext* pContext);
	static void SetCamera(D3DXMATRIX view, D3DXMATRIX proj);
	static void Destory();
	HRESULT LoadTexture(LPCWSTR name, D3DXVECTOR2 texsize, GrapRect _Rect);
	void Render(D3DXVECTOR2 pos, D3DXVECTOR2 scale);
};