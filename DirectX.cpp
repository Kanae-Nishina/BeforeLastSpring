/*
	@file	DirectX.h
	@brief	DirectXを使用する為のクラス
	@date	2017/02/18
	@author	仁科香苗
*/
#include "DirectX.h"

/*
	@brief	コンストラクタ
*/
DirectX::DirectX()
	:m_pSceneManaer(nullptr)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pBackBuffer_TexRTV(nullptr)
	, m_pBackBuffer_DSTexDSV(nullptr)
	, m_pBackBuffer_DSTex(nullptr)
{
}

/*
	@brief	デストラクタ
*/
DirectX::~DirectX()
{
}


/*
	@brief	DirectXの初期化
*/
HRESULT DirectX::InitD3D(HWND wnd)
{
	m_hWnd = wnd;
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = window_width;
	sd.BufferDesc.Height = window_height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}
	//各種テクスチャーと、それに付帯する各種ビューを作成

	//バックバッファーテクスチャーを取得（既にあるので作成ではない）
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//デプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = window_width;
	descDepth.Height = window_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pBackBuffer_DSTex);
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = window_width;
	vp.Height = window_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	return S_OK;
}

/*
	@brief	ウィンドウプロシージャ
*/
LRESULT DirectX::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

/*
	@brief	アプリケーションの初期化
*/
void DirectX::AppInit()
{
	//スプライトの初期化
	Sprite::Init(m_pDeviceContext);

	//シーンの作成
	m_pSceneManaer = new SceneRoot;
	m_pSceneManaer->Init();
}

/*
	@brief	アプリケーションの更新
*/
void DirectX::Update()
{
	//シーンの更新
	SceneBase* scene = nullptr;
	scene = m_pSceneManaer->Update(m_pSceneManaer);


	//描画
	//画面クリア
	float ClearColor[4] = { 0,0,0,0 };
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//カメラの設定
	SetCamera();

	//シーンの描画
	m_pSceneManaer->Render();
	//画面の更新
	m_pSwapChain->Present(0, 0);
}

/*
	@brief	描画の為のカメラ設定
*/
void  DirectX::SetCamera()
{
	Sprite::SetCamera(Camera::GetView(), Camera::GetProj());
}

/*
	@brief	アプリケーション処理
*/
void DirectX::Loop()
{
	//アプリケーションの初期化
	AppInit();

	// メッセージループ
	MSG msg = { 0 };
	m_lStartClock = timeGetTime();		//クロックの取得
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();	//更新

			//FPS調整
			while (timeGetTime() - m_lStartClock < 1000 / fps)
			{
				Sleep(1);
			}
			m_lStartClock = timeGetTime();
		}
	}

	//アプリケーション終了
	DestroyD3D();
}

/*
	@brief	解放処理
*/
void DirectX::DestroyD3D()
{
	SAFE_DELETE(m_pSceneManaer);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDeviceContext);
}