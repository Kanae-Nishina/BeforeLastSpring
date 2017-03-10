/*
	@file			Effect.cpp
	@brief		エフェクト
	@date		2017/03/04
	@author	金澤信芳
*/

#include "Effect.h"

::Effekseer::Manager*					Effect::m_manager	= nullptr;
::EffekseerRenderer::Renderer*		Effect::m_renderer		= nullptr;
::Effekseer::Handle						Effect::m_handle		= -1;
::Effekseer::Effect*						Effect::m_effect			= nullptr;

/*
	@brief	コンストラクタ
*/
Effect::Effect()
{
}

/*
	@brief	デストラクタ
*/
Effect::~Effect()
{
}

/*
	@エフェクトの初期化
*/
void Effect::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// 描画用インスタンスの生成
	m_renderer = ::EffekseerRendererDX11::Renderer::Create(device, deviceContext, 2000);

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
}

/*
	@brief	解放
*/
void Effect::Destory()
{
}

/*
	@brief	efkファイルの読み込み
*/
void Effect::Load(string path, float size,float playSpeed)
{
	// string→wchar_t*に変更
	size_t len = path.length() + 1;									// 文字列の長さ
	char* PathC = new char[len];										// 長さ分用意
	memcpy(PathC, path.c_str(), len);								// char*に長さ分コピー

	len = strlen(PathC) + 1;												// 改めてサイズ確保
	wchar_t* pathWC = new wchar_t[len];						// wchar_tのサイズ確保

	mbstowcs_s(&len, pathWC, len, PathC, _TRUNCATE);;	// char* を wchar_tに変換

	m_manager->SetScale(m_handle, size, size, size);		// エフェクトのサイズ

	// エフェクトの読込
	m_effect = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)pathWC);
}

/*
	@エフェクトの描画
*/
void Effect::Render()
{
	m_manager->Update();					// エフェクトの更新処理を行う

	m_renderer->BeginRendering();		// エフェクトの描画開始処理を行う。
	m_manager->Draw();					// エフェクトの描画を行う。
	m_renderer->EndRendering();		// エフェクトの描画終了処理を行う。
}

#if 0 後に必要な処理
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
#endif

/*
	@エフェクト再生
*/
void Effect::Play(D3DXVECTOR3 pos, float angle)
{
	m_handle = m_manager->Play(m_effect, pos.x, pos.y, pos.z);
}

/*
	＠エフェクト停止
*/
void Effect::Stop()
{
	m_manager->StopEffect(m_handle);
}

/*
	@エフェクトのカメラ設定
*/
void Effect::SetCamera(D3DXVECTOR3 pivotPos, D3DXVECTOR3 lookAtPos)
{
	Effekseer::Vector3D Pos = ::Effekseer::Vector3D(pivotPos.x, pivotPos.y, pivotPos.z);
	const Effekseer::Vector3D LookAtPos = ::Effekseer::Vector3D(lookAtPos.x, lookAtPos.y, lookAtPos.z);
	const Effekseer::Matrix44 View = ::Effekseer::Matrix44().LookAtLH(Pos, LookAtPos, ::Effekseer::Vector3D(0, 1, 0));
	const Effekseer::Matrix44 Proj = ::Effekseer::Matrix44().PerspectiveFovLH(D3DX_PI / 6, (FLOAT)window_width / (FLOAT)window_height, 0.1f, 1000.0f);

	// 投影行列を設定
	m_renderer->SetProjectionMatrix(Proj);

	// カメラ行列を設定
	m_renderer->SetCameraMatrix(View);
}