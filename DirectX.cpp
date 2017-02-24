/*
	@file	DirectX.h
	@brief	DirectX���g�p����ׂ̃N���X
	@date	2017/02/18
	@author	�m�ȍ��c
*/
#include "DirectX.h"

/*
	@brief	�R���X�g���N�^
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
	@brief	�f�X�g���N�^
*/
DirectX::~DirectX()
{
}


/*
	@brief	DirectX�̏�����
*/
HRESULT DirectX::InitD3D(HWND wnd)
{
	m_hWnd = wnd;
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
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
	//�e��e�N�X�`���[�ƁA����ɕt�т���e��r���[���쐬

	//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//���̃e�N�X�`���[�ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//�f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
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
	//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = window_width;
	vp.Height = window_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//���X�^���C�Y�ݒ�
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
	@brief	�E�B���h�E�v���V�[�W��
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
	@brief	�A�v���P�[�V�����̏�����
*/
void DirectX::AppInit()
{
	//�X�v���C�g�̏�����
	Sprite::Init(m_pDeviceContext);

	//�V�[���̍쐬
	m_pSceneManaer = new SceneRoot;
	m_pSceneManaer->Init();
}

/*
	@brief	�A�v���P�[�V�����̍X�V
*/
void DirectX::Update()
{
	//�V�[���̍X�V
	SceneBase* scene = nullptr;
	scene = m_pSceneManaer->Update(m_pSceneManaer);


	//�`��
	//��ʃN���A
	float ClearColor[4] = { 0,0,0,0 };
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//�J�����̐ݒ�
	SetCamera();

	//�V�[���̕`��
	m_pSceneManaer->Render();
	//��ʂ̍X�V
	m_pSwapChain->Present(0, 0);
}

/*
	@brief	�`��ׂ̈̃J�����ݒ�
*/
void  DirectX::SetCamera()
{
	Sprite::SetCamera(Camera::GetView(), Camera::GetProj());
}

/*
	@brief	�A�v���P�[�V��������
*/
void DirectX::Loop()
{
	//�A�v���P�[�V�����̏�����
	AppInit();

	// ���b�Z�[�W���[�v
	MSG msg = { 0 };
	m_lStartClock = timeGetTime();		//�N���b�N�̎擾
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
			Update();	//�X�V

			//FPS����
			while (timeGetTime() - m_lStartClock < 1000 / fps)
			{
				Sleep(1);
			}
			m_lStartClock = timeGetTime();
		}
	}

	//�A�v���P�[�V�����I��
	DestroyD3D();
}

/*
	@brief	�������
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