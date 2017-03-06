/*
	@file			Effect.cpp
	@brief		エフェクト
	@date		2017/03/04
	@author	金澤信芳
*/

#include "Effect.h"

map<string, int> EffectIndex; // エフェクト保管庫

::Effekseer::Manager* Effect::m_manager					= nullptr;
::EffekseerRenderer::Renderer* Effect::m_renderer		= nullptr;
::Effekseer::Handle Effect::m_handle							= -1;
::Effekseer::Effect* Effect::m_effect							= nullptr;
ID3D11Device* Effect::m_device									= nullptr;
ID3D11DeviceContext* Effect::m_deviceContext			= nullptr;
D3DXMATRIX	 Effect::m_view;
D3DXMATRIX	 Effect::m_proj;
/*
	@brief	コンストラクタ
*/
Effect::Effect()
{
	ZeroMemory(this, sizeof(Effect));
}

/*
	@brief	デストラクタ
*/
Effect::~Effect()
{
	// 先にエフェクト管理用インスタンスを破棄
	m_manager->Destroy();

	// 次に描画用インスタンスを破棄
	m_renderer->Destory();

	ES_SAFE_RELEASE(m_deviceContext);
	ES_SAFE_RELEASE(m_device);
}

/*
	@エフェクトの初期化
*/
void Effect::Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_device																	= device;
	m_deviceContext														= deviceContext;
	HWND g_window_handle											= hwnd;
	static IDXGIDevice1* g_dxgiDevice								= NULL;
	static IDXGIAdapter* g_adapter									= NULL;
	static IDXGIFactory* g_dxgiFactory								= NULL;
	static ID3D11Texture2D* g_depthBuffer						= NULL;

	static IDXGISwapChain* g_swapChain							= NULL;
	static ID3D11RenderTargetView* g_renderTargetView	= NULL;
	static ID3D11DepthStencilView* g_depthStencilView		= NULL;
	static ID3D11Texture2D* g_backBuffer						= NULL;

	// DirectX11の初期化を行う
	if (FAILED(m_device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&g_dxgiDevice)))
	{
		return;
	}

	if (FAILED(g_dxgiDevice->GetAdapter(&g_adapter)))
	{
		return;
	}

	g_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&g_dxgiFactory);
	if (g_dxgiFactory == NULL)
	{
		return;
	}

	DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
	hDXGISwapChainDesc.BufferDesc.Width = window_width;
	hDXGISwapChainDesc.BufferDesc.Height = window_height;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	hDXGISwapChainDesc.SampleDesc.Count = 1;
	hDXGISwapChainDesc.SampleDesc.Quality = 0;
	hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	hDXGISwapChainDesc.BufferCount = 1;
	hDXGISwapChainDesc.OutputWindow = (HWND)g_window_handle;
	hDXGISwapChainDesc.Windowed = TRUE;
	hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(g_dxgiFactory->CreateSwapChain(m_device, &hDXGISwapChainDesc, &g_swapChain)))
	{
		return;
	}

	if (FAILED(g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&g_backBuffer)))
	{
		return;
	}

	if (FAILED(m_device->CreateRenderTargetView(g_backBuffer, NULL, &g_renderTargetView)))
	{
		return;
	}

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float)window_width;
	vp.Height = (float)window_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_deviceContext->RSSetViewports(1, &vp);

	// 描画用インスタンスの生成
	m_renderer = ::EffekseerRendererDX11::Renderer::Create(m_device, m_deviceContext, 2000);

	// エフェクト管理用インスタンスの生成
	m_manager = ::Effekseer::Manager::Create(2000);

	// 描画用インスタンスから描画機能を設定
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());

	Set();
}

/*
	@brief	解放
*/
void Effect::Destory()
{

}

/*
	@エフェクトのロード
*/
void Effect::Set()
{
	EffectIndex["TEST1"] = Load((const EFK_CHAR*)L"test.efk");
	SetScale("TEST1", 1.0f);
}

/*
	@brief	efkファイルの読み込み
*/
int Effect::Load(const EFK_CHAR* path)
{
	//assert(Effekseer::Effect::Create(m_manager, path) != NULL);
	static int no = -1;
	++no;

	m_effect = Effekseer::Effect::Create(m_manager, path);

	return no;
}

/*
	@エフェクトの描画
*/
void Effect::Draw()
{
	// エフェクトの描画開始処理を行う。
	m_renderer->BeginRendering();

	// エフェクトの描画を行う。
	m_manager->Draw();

	// エフェクトの描画終了処理を行う。
	m_renderer->EndRendering();
}

/*
	@エフェクトのスケール設定
*/
void Effect::SetScale(string name, float scale)
{
	m_manager->SetScale(m_handle, scale, scale, scale);
}

/*
	@エフェクトをプレイヤーの向きに合わせてYawを設定
*/
void Effect::SetRotation(string name, D3DXVECTOR3 pos, float yaw)
{
	::Effekseer::Vector3D r_pos(pos.x, pos.y, pos.z);
	m_manager->SetRotation(m_handle, r_pos, yaw);
}

/*
	@エフェクトの再生スピードの設定
*/
void Effect::PlaySpeed(string name, float playSpeed)
{

}

/*
	@エフェクト再生
*/
void Effect::Play(string name, D3DXVECTOR3 pos)
{
	m_handle = m_manager->Play(m_effect, pos.x, pos.y, pos.z);
}

/*
	＠エフェクト停止
*/
void Effect::Stop(string name)
{
	m_manager->StopEffect(m_handle);
}

/*
	@エフェクト全停止
*/
void Effect::AllStop()
{
	m_manager->StopAllEffects();
}

/*
	@エフェクトのカメラ設定
*/
void Effect::SetCamera(D3DXMATRIX view, D3DXMATRIX proj)
{
#if 0
	Effekseer::Vector3D g_position = ::Effekseer::Vector3D(pos.x, pos.y, pos.z);
	const Effekseer::Vector3D g_at = ::Effekseer::Vector3D(look.x, look.y, look.z);
	const Effekseer::Matrix44 m_View = ::Effekseer::Matrix44().LookAtLH(g_position, g_at, ::Effekseer::Vector3D(0, 1, 0));
	const Effekseer::Matrix44 m_Proj = ::Effekseer::Matrix44().PerspectiveFovLH(D3DX_PI / 6, (FLOAT)window_width / (FLOAT)window_height, 0.1f, 1000.0f);

	// 投影行列を設定
	m_renderer->SetProjectionMatrix(m_Proj);

	// カメラ行列を設定
	m_renderer->SetCameraMatrix(m_View);
#endif // 0
	
	m_view = view;
	m_proj = proj;

	// ビュートランスフォーム
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -1);			 //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);	//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);		//上方位置
	D3DXMatrixLookAtLH(&m_view, &vEyePt, &vLookatPt, &vUpVec);

	// プロジェクショントランスフォーム（射影変換）
	D3DMATRIX mOtho = {
		2.0f / (float)(window_width), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(window_height), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
	m_proj = mOtho;
}