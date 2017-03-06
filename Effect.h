/*
	@file			Effect.h
	@brief		エフェクト
	@date		2017/03/04
	@author	金澤信芳
*/

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

	static void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static void SetCamera(D3DXMATRIX view, D3DXMATRIX proj);
	static void Destory();

	static void Set();
	static int Load(const EFK_CHAR* path);

	static void Draw();

	static void Play(string name, D3DXVECTOR3 pos);
	static void Stop(string name);
	static void AllStop();

private:
	static void SetScale(string name, float scale);
	static void PlaySpeed(string name, float playSpeed);
	static void SetRotation(string name, D3DXVECTOR3 pos, float yaw);

	static ::Effekseer::Manager*					m_manager;
	static ::EffekseerRenderer::Renderer*		m_renderer;
	static ::Effekseer::Handle						m_handle;
	static ::Effekseer::Effect*						m_effect;

	static ID3D11Device*								m_device;
	static ID3D11DeviceContext*					m_deviceContext;
	static D3DXMATRIX								m_view;
	static D3DXMATRIX								m_proj;
};