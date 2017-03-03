/*
	@file			Sprite.cpp
	@brief		2Dスプライト
	@date		2017/02/22
	@author	仁科香苗
*/
#include "Sprite.h"


float								Sprite::m_alpha;
D3D11_BLEND_DESC		Sprite::m_dd;
D3DXMATRIX					Sprite::m_view;
D3DXMATRIX					Sprite::m_proj;
D3DXVECTOR4				Sprite::m_color;
ID3D11Device*				Sprite::m_device = nullptr;
ID3D11DeviceContext*	Sprite::m_deviceContext = nullptr;
ID3D11SamplerState*	Sprite::m_sampleLinear = nullptr;
ID3D11VertexShader*	Sprite::m_vertexShader = nullptr;
ID3D11PixelShader*		Sprite::m_pixelShader = nullptr;
ID3D11InputLayout*		Sprite::m_vertexLayout = nullptr;
ID3D11Buffer*				Sprite::m_constantBuffer = nullptr;
ID3D11BlendState*		Sprite::m_blendState = nullptr;

/*
	@brief	コンストラクタ
*/
Sprite::Sprite()
	:m_scale(1.0f)
	, m_count(0)
	,m_texScroll(0,0)
{
	ZeroMemory(this, sizeof(Sprite));
}

/*
	@brief	デストラクタ
*/
Sprite::~Sprite()
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_rectBuffer);
	SAFE_RELEASE(m_texture);
}

/*
	@brief	初期化
*/
HRESULT Sprite::Init(ID3D11DeviceContext* pContext)
{
	m_color = D3DXVECTOR4(1, 1, 1, 1);
	m_alpha = m_color.w;
	m_deviceContext = pContext;
	m_deviceContext->GetDevice(&m_device);

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_device->CreateSamplerState(&SamDesc, &m_sampleLinear);

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromResource(NULL, MAKEINTRESOURCE(SPRITE_ID), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"sprite.hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_device->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_vertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"sprite:バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(m_device->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_vertexLayout)))
		return FALSE;
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromResource(NULL, MAKEINTRESOURCE(SPRITE_ID), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"sprite.hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_device->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"sprite:ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SPRITE_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_device->CreateBuffer(&cb, NULL, &m_constantBuffer)))
	{
		return E_FAIL;
	}

	//抜け色設定
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

	if (FAILED(m_device->CreateBlendState(&dd, &m_blendState)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/*
	@brief	解放
*/
void Sprite::Destory()
{

}

/*
	@brief テクスチャのロード
*/
HRESULT Sprite::LoadTexture(LPCWSTR name,D3DXVECTOR2 splite, D3DXVECTOR2 texsize, int animSpeed)
{
	m_size = splite;
	m_animSpeed = animSpeed;
	//画像表示位置指定　デフォルトは全域
	D3DXVECTOR4 rect = D3DXVECTOR4(0.0f, 0.0f, 1.0f / m_size.x, 1.0f / m_size.y);
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(0, texsize.y, 0), D3DXVECTOR2(rect.x, rect.w),//頂点1,
		D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(rect.x, rect.y),//頂点2
		D3DXVECTOR3(texsize.x, texsize.y, 0), D3DXVECTOR2(rect.z, rect.w), //頂点3
		D3DXVECTOR3(texsize.x, 0, 0), D3DXVECTOR2(rect.z, rect.y), //頂点4
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_device->CreateBuffer(&bd, &InitData, &m_vertexBuffer)))
	{
		return E_FAIL;
	}

	LPCWSTR texturename = name;
	//テクスチャーを作成
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_device, texturename, NULL, NULL, &m_texture, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/*
	@brief	描画
*/
void Sprite::Render(D3DXVECTOR2 pos, D3DXVECTOR2 scale)
{
	//トポロジー
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点インプットレイアウトをセット
	m_deviceContext->IASetInputLayout(m_vertexLayout);

	//使用するシェーダーの登録
	m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	//このコンスタントバッファーを使うシェーダーの登録
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_deviceContext->PSSetConstantBuffers(0, 1, &m_constantBuffer);

	//テクスチャーをシェーダーに渡す
	m_deviceContext->PSSetSamplers(0, 1, &m_sampleLinear);
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture);

	//ワールド変換
	D3DXMATRIX mWorld, mTran, mScale;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mTran, pos.x, pos.y, -1);
	//スケール変換
	//D3DXMatrixIdentity(&mScale);
	D3DXMatrixScaling(&mScale, scale.x, scale.y, 1);
	mWorld = mScale*mTran;
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_deviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = mWorld*m_view*m_proj;
		D3DXMatrixTranspose(&m, &m);
		cb.WVP = m;
		//カラーを渡す
		cb.Color = m_color;
		//透明度を渡す
		cb.Alpha.x = m_alpha;

		//テクスチャースクロールの増分を渡す
		if (m_count < m_animSpeed)
		{
			m_count++;
		}
		if (m_count >= m_animSpeed)
		{
			m_texScroll.x += 1.0f / m_size.x;
			m_count = 0;
			m_animCount++;
			if ((m_animCount % (int)m_size.x) == 0)
			{
				m_texScroll.y += 1.0f / m_size.y;
				m_texScroll.x = 0.0f;
			}
			if ((m_animCount % (int)(m_size.x * m_size.y)) == 0)
			{
				m_animCount = 0;
				m_texScroll.x = 0.0f;
				m_texScroll.y = 0.0f;
			}
		}
		cb.TexScroll = D3DXVECTOR4(m_texScroll.x, m_texScroll.y, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_deviceContext->Unmap(m_constantBuffer, 0);
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//抜け色
	UINT ColorKey = 0xffffffff;
	m_deviceContext->OMSetBlendState(m_blendState, NULL, ColorKey);
	m_deviceContext->Draw(4, 0);
}

/*
	@brief	カメラのセット
*/
void Sprite::SetCamera(D3DXMATRIX view, D3DXMATRIX proj)
{
	m_view = view;
	m_proj = proj;

	StartBlendState();		//ブレンドを有効に

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