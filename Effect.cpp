/*
	@file			Effect.cpp
	@brief		�G�t�F�N�g
	@date		2017/03/04
	@author	���V�M�F
*/

#include "Effect.h"

map<string, int> EffectIndex; // �G�t�F�N�g�ۊǌ�

::Effekseer::Manager* Effect::m_manager					= nullptr;
::EffekseerRenderer::Renderer* Effect::m_renderer		= nullptr;
::Effekseer::Handle Effect::m_handle							= -1;
::Effekseer::Effect* Effect::m_effect							= nullptr;
ID3D11Device* Effect::m_device									= nullptr;
ID3D11DeviceContext* Effect::m_deviceContext			= nullptr;
D3DXMATRIX	 Effect::m_view;
D3DXMATRIX	 Effect::m_proj;
/*
	@brief	�R���X�g���N�^
*/
Effect::Effect()
{
	ZeroMemory(this, sizeof(Effect));
}

/*
	@brief	�f�X�g���N�^
*/
Effect::~Effect()
{
	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	m_manager->Destroy();

	// ���ɕ`��p�C���X�^���X��j��
	m_renderer->Destory();

	ES_SAFE_RELEASE(m_deviceContext);
	ES_SAFE_RELEASE(m_device);
}

/*
	@�G�t�F�N�g�̏�����
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

	// DirectX11�̏��������s��
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

	// �`��p�C���X�^���X�̐���
	m_renderer = ::EffekseerRendererDX11::Renderer::Create(m_device, m_deviceContext, 2000);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	m_manager = ::Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());

	Set();
}

/*
	@brief	���
*/
void Effect::Destory()
{

}

/*
	@�G�t�F�N�g�̃��[�h
*/
void Effect::Set()
{
	EffectIndex["TEST1"] = Load((const EFK_CHAR*)L"test.efk");
	SetScale("TEST1", 1.0f);
}

/*
	@brief	efk�t�@�C���̓ǂݍ���
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
	@�G�t�F�N�g�̕`��
*/
void Effect::Draw()
{
	// �G�t�F�N�g�̕`��J�n�������s���B
	m_renderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	m_manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	m_renderer->EndRendering();
}

/*
	@�G�t�F�N�g�̃X�P�[���ݒ�
*/
void Effect::SetScale(string name, float scale)
{
	m_manager->SetScale(m_handle, scale, scale, scale);
}

/*
	@�G�t�F�N�g���v���C���[�̌����ɍ��킹��Yaw��ݒ�
*/
void Effect::SetRotation(string name, D3DXVECTOR3 pos, float yaw)
{
	::Effekseer::Vector3D r_pos(pos.x, pos.y, pos.z);
	m_manager->SetRotation(m_handle, r_pos, yaw);
}

/*
	@�G�t�F�N�g�̍Đ��X�s�[�h�̐ݒ�
*/
void Effect::PlaySpeed(string name, float playSpeed)
{

}

/*
	@�G�t�F�N�g�Đ�
*/
void Effect::Play(string name, D3DXVECTOR3 pos)
{
	m_handle = m_manager->Play(m_effect, pos.x, pos.y, pos.z);
}

/*
	���G�t�F�N�g��~
*/
void Effect::Stop(string name)
{
	m_manager->StopEffect(m_handle);
}

/*
	@�G�t�F�N�g�S��~
*/
void Effect::AllStop()
{
	m_manager->StopAllEffects();
}

/*
	@�G�t�F�N�g�̃J�����ݒ�
*/
void Effect::SetCamera(D3DXMATRIX view, D3DXMATRIX proj)
{
#if 0
	Effekseer::Vector3D g_position = ::Effekseer::Vector3D(pos.x, pos.y, pos.z);
	const Effekseer::Vector3D g_at = ::Effekseer::Vector3D(look.x, look.y, look.z);
	const Effekseer::Matrix44 m_View = ::Effekseer::Matrix44().LookAtLH(g_position, g_at, ::Effekseer::Vector3D(0, 1, 0));
	const Effekseer::Matrix44 m_Proj = ::Effekseer::Matrix44().PerspectiveFovLH(D3DX_PI / 6, (FLOAT)window_width / (FLOAT)window_height, 0.1f, 1000.0f);

	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(m_Proj);

	// �J�����s���ݒ�
	m_renderer->SetCameraMatrix(m_View);
#endif // 0
	
	m_view = view;
	m_proj = proj;

	// �r���[�g�����X�t�H�[��
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -1);			 //�J�����i���_�j�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);	//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);		//����ʒu
	D3DXMatrixLookAtLH(&m_view, &vEyePt, &vLookatPt, &vUpVec);

	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DMATRIX mOtho = {
		2.0f / (float)(window_width), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(window_height), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
	m_proj = mOtho;
}