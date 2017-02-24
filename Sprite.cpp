/*
	@file			Sprite.cpp
	@brief		2Dスプライト
	@date		2017/02/22
	@author	仁科香苗
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
	@brief	コンストラクタ
*/
Sprite::Sprite()
	:m_fScale(1.0f)
{
	ZeroMemory(this, sizeof(Sprite));
}

/*
	@brief	デストラクタ
*/
Sprite::~Sprite()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_RectBuffer);
	SAFE_RELEASE(m_pTexture);
}

/*
	@brief	初期化
*/
HRESULT Sprite::Init(ID3D11DeviceContext* pContext)
{
	m_vColor = D3DXVECTOR4(1, 1, 1, 1);
	m_fAlpha = m_vColor.w;
	m_pDeviceContext = pContext;
	m_pDeviceContext->GetDevice(&m_pDevice);

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromResource(NULL, MAKEINTRESOURCE(SPRITE_ID), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
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
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
		return FALSE;
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromResource(NULL, MAKEINTRESOURCE(SPRITE_ID), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
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

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
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

	if (FAILED(m_pDevice->CreateBlendState(&dd, &m_pBlendState)))
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
HRESULT Sprite::LoadTexture(LPCWSTR name, D3DXVECTOR2 texsize, GrapRect _Rect)
{
	m_Size = texsize;
	GrapRect rect = GrapRect(0.0f, 0.0f, 1.0f / m_Size.x, 1.0f / m_Size.y);

	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(texsize.x, texsize.y, 0), D3DXVECTOR2(_Rect.m_left, _Rect.m_bottom),			//頂点1,
		D3DXVECTOR3(texsize.x, 0, 0), D3DXVECTOR2(_Rect.m_left, _Rect.m_top),						//頂点2
		D3DXVECTOR3(0,texsize.y, 0), D3DXVECTOR2(_Rect.m_right, _Rect.m_bottom),	//頂点3
		D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(_Rect.m_right, _Rect.m_top),				//頂点4
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
	//テクスチャーを作成
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice, texturename, NULL, NULL, &m_pTexture, NULL)))
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
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//使用するシェーダーの登録
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//テクスチャーをシェーダーに渡す
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	//ワールド変換
	D3DXMATRIX mWorld, mTran, mScale;
	//D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(&mTran, pos.x, pos.y, -1);
	//スケール変換
	//D3DXMatrixIdentity(&mScale);
	D3DXMatrixScaling(&mScale, scale.x, scale.y, 1);
	mWorld = mScale*mTran;
	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE_CONSTANT_BUFFER cb;
	m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	//ワールド、カメラ、射影行列を渡す
	D3DXMATRIX m = mWorld*m_mView*m_mProj;
	D3DXMatrixTranspose(&m, &m);
	cb.mWVP = m;
	//カラーを渡す
	cb.vColor = m_vColor;
	//透明度を渡す
	cb.fAlpha.x = m_fAlpha;

	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//抜け色
	UINT ColorKey = 0xffffffff;
	m_pDeviceContext->OMSetBlendState(m_pBlendState, NULL, ColorKey);
	m_pDeviceContext->Draw(4, 0);
}

/*
	@brief	カメラのセット
*/
void Sprite::SetCamera(D3DXMATRIX view, D3DXMATRIX proj)
{
	m_mView = view;
	m_mProj = proj;

	StartBlendState();//ブレンドを有効に

					  // ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -1); //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&m_mView, &vEyePt, &vLookatPt, &vUpVec);

	// プロジェクショントランスフォーム（射影変換）
	D3DMATRIX mOtho = {
		2.0f / (float)(window_width), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(window_height), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
	m_mProj = mOtho;
}