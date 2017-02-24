/*
	@file			Sprite.cpp
	@brief		2D�X�v���C�g
	@date		2017/02/22
	@author	�m�ȍ��c
*/
#include "Sprite.h"

float								Sprite::m_fAlpha;
D3D11_BLEND_DESC		Sprite::m_dd;
D3DXMATRIX					Sprite::m_mView;
D3DXMATRIX					Sprite::m_mProj;
D3DXVECTOR4				Sprite::m_vColor;
ID3D11Device*				Sprite::m_pDevice = nullptr;
ID3D11DeviceContext*	Sprite::m_pDeviceContext = nullptr;
ID3D11SamplerState*	Sprite::m_pSampleLinear = nullptr;
ID3D11VertexShader*	Sprite::m_pVertexShader = nullptr;
ID3D11PixelShader*		Sprite::m_pPixelShader = nullptr;
ID3D11InputLayout*		Sprite::m_pVertexLayout = nullptr;
ID3D11Buffer*				Sprite::m_pConstantBuffer = nullptr;
ID3D11BlendState*		Sprite::m_pBlendState = nullptr;

/*
	@brief	�R���X�g���N�^
*/
Sprite::Sprite()
	:m_fScale(1.0f)
{
	ZeroMemory(this, sizeof(Sprite));
}

/*
	@brief	�f�X�g���N�^
*/
Sprite::~Sprite()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_RectBuffer);
	SAFE_RELEASE(m_pTexture);
}

/*
	@brief	������
*/
HRESULT Sprite::Init(ID3D11DeviceContext* pContext)
{
	m_vColor = D3DXVECTOR4(1, 1, 1, 1);
	m_fAlpha = m_vColor.w;
	m_pDeviceContext = pContext;
	m_pDeviceContext->GetDevice(&m_pDevice);

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromResource(NULL, MAKEINTRESOURCE(SPRITE_ID), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
		return FALSE;
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromResource(NULL, MAKEINTRESOURCE(SPRITE_ID), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SPRITE_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}

	//�����F�ݒ�
	D3D11_BLEND_DESC dd;
	ZeroMemory(&dd, sizeof(D3D11_BLEND_DESC));
	dd.IndependentBlendEnable = false;
	dd.AlphaToCoverageEnable = false;
	dd.RenderTarget[0].BlendEnable = true;
	dd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	dd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	dd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	dd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	dd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	dd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	dd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&dd, &m_pBlendState)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/*
	@brief	���
*/
void Sprite::Destory()
{

}

/*
	@brief �e�N�X�`���̃��[�h
*/
HRESULT Sprite::LoadTexture(LPCWSTR name, D3DXVECTOR2 texsize, GrapRect _Rect)
{
	m_Size = texsize;
	GrapRect rect = GrapRect(0.0f, 0.0f, 1.0f / m_Size.x, 1.0f / m_Size.y);

	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(texsize.x, texsize.y, 0), D3DXVECTOR2(_Rect.m_left, _Rect.m_bottom),			//���_1,
		D3DXVECTOR3(texsize.x, 0, 0), D3DXVECTOR2(_Rect.m_left, _Rect.m_top),						//���_2
		D3DXVECTOR3(0,texsize.y, 0), D3DXVECTOR2(_Rect.m_right, _Rect.m_bottom),	//���_3
		D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(_Rect.m_right, _Rect.m_top),				//���_4
	};


	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	//ID3D11Device* device = m_pDevice;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	LPCWSTR texturename = name;
	//�e�N�X�`���[���쐬
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, texturename, NULL, NULL, &m_pTexture, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/*
	@brief	�`��
*/
void Sprite::Render(D3DXVECTOR2 pos, D3DXVECTOR2 scale)
{
	//�g�|���W�[
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//�g�p����V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	//���[���h�ϊ�
	D3DXMATRIX mWorld, mTran, mScale;
	//D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mTran, pos.x, pos.y, -1);
	//�X�P�[���ϊ�
	//D3DXMatrixIdentity(&mScale);
	D3DXMatrixScaling(&mScale, scale.x, scale.y, 1);
	mWorld = mScale*mTran;
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE_CONSTANT_BUFFER cb;
	m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	//���[���h�A�J�����A�ˉe�s���n��
	D3DXMATRIX m = mWorld*m_mView*m_mProj;
	D3DXMatrixTranspose(&m, &m);
	cb.mWVP = m;
	//�J���[��n��
	cb.vColor = m_vColor;
	//�����x��n��
	cb.fAlpha.x = m_fAlpha;

	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//�����F
	UINT ColorKey = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, ColorKey);
	m_pDeviceContext->Draw(4, 0);
}

/*
	@brief	�J�����̃Z�b�g
*/
void Sprite::SetCamera(D3DXMATRIX view, D3DXMATRIX proj)
{
	m_mView = view;
	m_mProj = proj;

	StartBlendState();//�u�����h��L����

					  // �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -1); //�J�����i���_�j�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&m_mView, &vEyePt, &vLookatPt, &vUpVec);

	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DMATRIX mOtho = {
		2.0f / (float)(window_width), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(window_height), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
	m_mProj = mOtho;
}