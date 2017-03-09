/*
	@file			Effect.h
	@brief		�G�t�F�N�g
	@date		2017/03/04
	@author	���V�M�F
*/

#pragma once

// �G�t�F�N�g�ɕK�v�ȃC���N���[�h�t�@�C��
#include "../Effekseer/include/Effekseer.h"
#include "../Effekseer/include/EffekseerRendererDX11.h"

// �G�t�F�N�g�ɕK�v�ȃ��C�u�����t�@�C��
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

	static void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext); // ������
	static void SetCamera(D3DXVECTOR3 pos, D3DXVECTOR3 lookAtPos); // �J�����̃Z�b�g
	static void Destory(); // ���

	void Load(string path, float size, float playSpeed); // �G�t�F�N�g�̃��[�h
	void Render(); // �`��
	void Play(D3DXVECTOR3 pos, float angle); // �Đ�
	void Stop(); // ��~

private:
	static ::Effekseer::Manager*					m_manager;
	static ::EffekseerRenderer::Renderer*		m_renderer;
	static ::Effekseer::Handle						m_handle;
	static ::Effekseer::Effect*						m_effect;
};