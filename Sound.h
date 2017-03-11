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

using namespace std;

struct SoundData
{
	BYTE*							wavBuffer;			//�g�`�f�[�^�i�t�H�[�}�b�g�����܂܂Ȃ��A�����ɔg�`�f�[�^�̂݁j
	DWORD							wavSize;				//�g�`�f�[�^�̃T�C�Y
	IXAudio2SourceVoice*	sourceVoice;
};

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

	HRESULT			Init();															// �T�E���h�̏�����

	void					PlaySE(string name);										// SE�Đ�
	void					StopSE(string name);										// SE��~
	void					PlayBGM(string name);									// BGM�Đ�
	void					StopBGM(string name);									// BGM��~
	
private:
	Sound();
	~Sound();
	void					Set();															// �T�E���h�̃��[�h
	void					PlaySound(int soundIndex, bool loopFlag);		// �T�E���h�̍Đ�
	int						Load(char* szFileName);								// wav�t�@�C���̓ǂݍ���

	
	int						m_soundCurrentIndex;									//���̌��݂̍Đ��ԍ�
	bool					m_flag;															//BGM���Ȃ��Ă邩

	IXAudio2*								m_audio2;
	IXAudio2MasteringVoice*		m_masteringVoice;

	vector<SoundData>				m_soundData;
};