/*
	@file			Sound.h
	@brief		�T�E���h
	@date		2017/03/02
	@author	���V�M�F
*/

#pragma once
#include "Common.h"
#include <Xaudio2.h>
#include <map>
#include <vector>
#include <string>

const int max_wav = 50; // �T�E���h�̍ő吔

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

	void					SE_play(string SE_mode);			// SE�Đ�
	void					SE_stop(string SE_mode);			// SE��~
	void					BGM_play(string BGM_mode);		// BGM�Đ�
	void					BGM_stop(string BGM_mode);		// BGM��~
	
	IXAudio2*								m_XAudio2;
	IXAudio2MasteringVoice*		m_MasteringVoice;
	IXAudio2SourceVoice*			m_SourceVoice[max_wav];
	
	BYTE*			m_WavBuffer[max_wav];				//�g�`�f�[�^�i�t�H�[�}�b�g�����܂܂Ȃ��A�����ɔg�`�f�[�^�̂݁j
	DWORD			m_WavSize[max_wav];					//�g�`�f�[�^�̃T�C�Y

	int					m_SoundCurrentIndex;					//���̌��݂̍Đ��ԍ�
	bool				m_Flag;											//BGM���Ȃ��Ă邩

private:
	Sound();
	virtual ~Sound();
};

