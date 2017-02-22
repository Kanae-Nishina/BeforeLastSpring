/*
	@file	DirectX.h
	@brief	DirectXを使用する為のクラス
	@date	2017/02/18
	@author	仁科香苗
*/
#pragma once
#include "Common.h"
#include "SceneRoot.h"
#include "Scene.h"

class DirectX
{
	static HWND								m_wnd;
	static ID3D11Device*					m_pDevice;
	static ID3D11DeviceContext*		m_pDeviceContext;
	static IDXGISwapChain*				m_pSwapChain;
	static ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;
	static ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;
	static ID3D11Texture2D*			m_pBackBuffer_DSTex;

	long m_start;				//開始時のクロック数
	
	void AppInit();			//アプリケーションの初期化
	void Update();			//アプリケーションの更新
	void DestroyD3D();	//解放処理
public:
	DirectX();
	~DirectX();

	HRESULT InitD3D(HWND wnd);																		//DirectXの初期化
	LRESULT MsgProc(HWND wnd, UINT iMsg, WPARAM wParam, LPARAM lParam);	//ウィンドウプロシージャ
	void Loop();																									//アプリケーション処理
};

