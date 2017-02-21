/*
	@file	Sprite.h
	@brief	スプライト
	@date	2017/02/20
	@author	仁科香苗
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
	D3DXVECTOR3 Pos;	//位置
	D3DXVECTOR2 vTex;	//テクスチャー座標
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

	D3DXVECTOR2 m_size;				//アニメーションテクスチャの横、縦サイズ
	float		m_scale;			//スケール
	float		m_yaw;				//y軸回転
	float		m_pitch;			//x軸回転
	float		m_roll;				//z軸回転
	float		m_alpha;			//アルファ値
	int			m_animSpeed;		//アニメーションのスピード
	int			m_animCount;		//アニメーションカウント

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