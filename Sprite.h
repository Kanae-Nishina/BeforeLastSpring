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
	D3DXMATRIX WVP;
	D3DXVECTOR4 Color;
	D3DXVECTOR4 Alpha;
	D3DXVECTOR4 TexScroll;
};

struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
	D3DXVECTOR2 Tex; //テクスチャー座標
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
public:
	Sprite();
	~Sprite();

	static HRESULT Init(ID3D11DeviceContext* pContext);
	static void SetCamera(D3DXMATRIX view, D3DXMATRIX proj);
	static void Destory();
	HRESULT LoadTexture(LPCWSTR name, D3DXVECTOR2 splite, D3DXVECTOR2 texsize, int animSpeed);
	void Render(D3DXVECTOR2 pos, D3DXVECTOR2 scale);

private:
	static ID3D11Device*					m_device;
	static ID3D11DeviceContext*		m_deviceContext;
	static ID3D11SamplerState*		m_sampleLinear;
	static ID3D11VertexShader*		m_vertexShader;
	static ID3D11PixelShader*			m_pixelShader;
	static ID3D11InputLayout*			m_vertexLayout;
	static ID3D11Buffer*					m_constantBuffer;
	static ID3D11BlendState*			m_blendState;
	static D3D11_BLEND_DESC			m_dd;
	static D3DXMATRIX					m_view;
	static D3DXMATRIX					m_proj;
	static D3DXVECTOR4					m_color;
	static float									m_alpha;

	ID3D11Buffer*						m_vertexBuffer;
	ID3D11ShaderResourceView*	m_texture;
	ID3D11Buffer*						m_rectBuffer;
	float										m_scale;				//25pixelを基準 25pixel=1.0f
	int											m_animSpeed;
	int											m_animCount;
	int											m_count;
	D3DXVECTOR2						m_size;
	D3DXVECTOR2						m_texScroll;

	static void	StopBlendState()
	{
		m_dd.RenderTarget[0].BlendEnable = false;
		m_device->CreateBlendState(&m_dd, &m_blendState);
	}
	static void StartBlendState()
	{
		m_dd.RenderTarget[0].BlendEnable = true;
		m_dd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		m_dd.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
		m_dd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		m_dd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		m_dd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		m_dd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_dd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_device->CreateBlendState(&m_dd, &m_blendState);
	}

};