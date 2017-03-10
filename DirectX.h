/*
	@file	DirectX.h
	@brief	DirectXを使用する為のクラス
	@date	2017/02/18
	@author	仁科香苗

	@date		2017/03/03
	@author	金澤信芳(サウンド、エフェクトのインクルード追記)
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

	HRESULT InitD3D(HWND wnd);																		//DirectXの初期化
	LRESULT MsgProc(HWND wnd, UINT iMsg, WPARAM wParam, LPARAM lParam);	//ウィンドウプロシージャ
	void Loop();
private:
	HWND								m_wnd;
	ID3D11Device*					m_device;
	ID3D11DeviceContext*		m_deviceContext;
	IDXGISwapChain*				m_swapChain;
	ID3D11RenderTargetView*	m_backBuffer_TexRTV;
	ID3D11DepthStencilView*	m_backBuffer_DSTexDSV;
	ID3D11Texture2D*				m_backBuffer_DSTex;

	SceneRoot*						m_sceneManager;		//シーンマネージャー
	long									m_startClock;			//開始時のクロック数

	void AppInit();							//アプリケーションの初期化
	void Update();							//アプリケーションの更新
	void SetCamera();						//描画の為のマトリックス設定
	void DestroyD3D();					//解放処理

	Camera* m_camera;
};

