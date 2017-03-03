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

	HRESULT			InitSound();
	int						LoadSound(char* szFileName);
	void					PlaySound(int iSoundIndex, bool LoopFlag);
	void					SoundSet();

	void					SE_play(string SE_mode);			// SE再生
	void					SE_stop(string SE_mode);			// SE停止
	void					BGM_play(string BGM_mode);		// BGM再生
	void					BGM_stop(string BGM_mode);		// BGM停止
	
	IXAudio2*								m_XAudio2;
	IXAudio2MasteringVoice*		m_MasteringVoice;
	IXAudio2SourceVoice*			m_SourceVoice[max_wav];
	
	BYTE*			m_WavBuffer[max_wav];				//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD			m_WavSize[max_wav];					//波形データのサイズ

	int					m_SoundCurrentIndex;					//音の現在の再生番号
	bool				m_Flag;											//BGMがなってるか

private:
	Sound();
	virtual ~Sound();
};

