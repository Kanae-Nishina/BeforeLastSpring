/*
	@file			Sound.h
	@brief		サウンド
	@date		2017/03/02
	@author	金澤信芳
*/

#pragma once
#include "Common.h"
#include <Xaudio2.h>
#include <map>
#include <vector>
#include <string>

const int max_wav = 50; // サウンドの最大数

using namespace std;

class Sound
{
public:
	Sound(const Sound&)					= delete;
	Sound& operator						= (const Sound&) = delete;
	Sound(Sound&&)						= delete;
	Sound& operator=(Sound&&)		= delete;
	
	static Sound& getInstance()
	{
		static Sound inst;
		return inst;
	}

	HRESULT			Init();															// サウンドの初期化

	void					PlaySE(string name);										// SE再生
	void					StopSE(string name);										// SE停止
	void					PlayBGM(string name);									// BGM再生
	void					StopBGM(string name);									// BGM停止
	
private:
	Sound();
	~Sound();
	void					Set();															// サウンドのロード
	void					PlaySound(int soundIndex, bool loopFlag);		// サウンドの再生
	int						Load(char* szFileName);								// wavファイルの読み込み

	BYTE*				m_wavBuffer[max_wav];								//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD				m_wavSize[max_wav];									//波形データのサイズ

	int						m_soundCurrentIndex;									//音の現在の再生番号
	bool					m_flag;															//BGMがなってるか

	IXAudio2*								m_audio2;
	IXAudio2MasteringVoice*		m_masteringVoice;
	IXAudio2SourceVoice*			m_sourceVoice[max_wav];

};