/*
	@file			Effect.h
	@brief		エフェクト
	@date		2017/03/04
	@author	金澤信芳
*/

#pragma once

// エフェクトに必要なインクルードファイル
#include "../Effekseer/include/Effekseer.h"
#include "../Effekseer/include/EffekseerRendererDX11.h"

// エフェクトに必要なライブラリファイル
#if _DEBUG
#pragma comment(lib, "Effekseer/lib/VS2015/Debug/Effekseer.lib" )
#pragma comment(lib, "Effekseer/lib/VS2015/Debug/EffekseerRendererDX11.lib" )
#else
#pragma comment(lib, "Effekseer/lib/VS2015/Release/Effekseer.lib" )
#pragma comment(lib, "Effekseer/lib/VS2015/Release/EffekseerRendererDX11.lib" )
#endif

#pragma once
#include "../Common.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class Effect
{
public:
	Effect();
	~Effect();

	static void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext); // 初期化
	static void SetCamera(D3DXVECTOR3 pos, D3DXVECTOR3 lookAtPos); // カメラのセット
	static void Destory(); // 解放

	void Load(string path, float size, float playSpeed); // エフェクトのロード
	void Render(); // 描画
	void Play(D3DXVECTOR3 pos, float angle); // 再生
	void Stop(); // 停止

private:
	static ::Effekseer::Manager*					m_manager;
	static ::EffekseerRenderer::Renderer*		m_renderer;
	static ::Effekseer::Handle						m_handle;
	static ::Effekseer::Effect*						m_effect;
};