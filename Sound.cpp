/*
	@file			Sound.cpp
	@brief		�T�E���h
	@date		2017/03/02
	@author	���V�M�F
*/

#include "Sound.h"

map<string, int> SoundIndex; // �T�E���h�ۊǌ�

/*
	@brief	�R���X�g���N�^
*/
Sound::Sound()
{
	ZeroMemory(this, sizeof(Sound));
	m_Flag = true;
}

/*
	@brief	�f�X�g���N�^
*/
Sound::~Sound()
{
	for (int i = 0; i < max_wav; i++)
	{
		if (m_SourceVoice[i]) m_SourceVoice[i]->DestroyVoice();
	}
	for (int i = 0; i<max_wav; i++) SAFE_DELETE(m_WavBuffer[i]);
	SAFE_RELEASE(m_XAudio2);
}

/*
	@brief�@�T�E���h�̏�����
*/
HRESULT Sound::InitSound()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(XAudio2Create(&m_XAudio2, 0)))
	{
		CoUninitialize();
		return E_FAIL;
	}
	if (FAILED(m_XAudio2->CreateMasteringVoice(&m_MasteringVoice)))
	{
		CoUninitialize();
		return E_FAIL;
	}
	return S_OK;
}

/*
	@brief	�T�E���h�̏���
*/
void Sound::SoundSet()
{
	InitSound(); // �T�E���h�̏�����

	SoundIndex["TEST1"] = LoadSound("Sound/BGM/DonkeyDonk.wav");
	SoundIndex["TEST2"] = LoadSound("Sound/BGM/AO-1.wav");

}

/*
	@brief	wav�t�@�C���̓ǂݍ���
*/
int Sound::LoadSound(char* szFileName)
{
	static int iIndex = -1;
	iIndex++;
	HMMIO						hMmio = NULL;																					//Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API)
	DWORD						dwWavSize = 0;																					//WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
	WAVEFORMATEX*		pwfex;																								//WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
	MMCKINFO				ckInfo;																								//�@�`�����N���
	MMCKINFO				riffckInfo;																							// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT		pcmWaveForm;

	hMmio =					mmioOpenA(szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);		//WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
	mmioDescend(hMmio, &riffckInfo, NULL, 0);																		//�t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
	ckInfo.ckid =				mmioFOURCC('f', 'm', 't', ' ');																// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

	//�t�H�[�}�b�g��ǂݍ���
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);

	// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);											//�f�[�^�`�����N�ɃZ�b�g
	dwWavSize = ckInfo.cksize;
	m_WavBuffer[iIndex] = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_WavBuffer[iIndex], dwWavSize);

	//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
	if (FAILED(m_XAudio2->CreateSourceVoice(&m_SourceVoice[iIndex], pwfex)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�쐬���s", 0, MB_OK);
		return E_FAIL;
	}
	m_WavSize[iIndex] = dwWavSize;

	return iIndex;
}

/*
	@brief	�T�E���h�̍Đ�
*/
void  Sound::PlaySound(int SoundIndex, bool LoopFlag)
{
	m_SourceVoice[SoundIndex]->Stop(0, 1);
	m_SourceVoice[SoundIndex]->FlushSourceBuffers();
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_WavBuffer[SoundIndex];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_WavSize[SoundIndex];

	if (LoopFlag) // ���[�v�̔���
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	if (FAILED(m_SourceVoice[SoundIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�ɃT�u�~�b�g���s", 0, MB_OK);
		return;
	}

	m_SourceVoice[SoundIndex]->Start(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	BGM�̍Đ�
*/
void Sound::BGM_play(string BGM_mode)
{
	PlaySound(SoundIndex[BGM_mode], true);
}

/*
	@brief	BGM�̒�~
*/
void Sound::BGM_stop(string BGM_mode)
{
	m_SourceVoice[SoundIndex[BGM_mode]]->Stop(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	SE�̍Đ�
*/
void Sound::SE_play(string SE_mode)
{
	PlaySound(SoundIndex[SE_mode], false);
}

/*
	@brief	SE�̒�~
*/
void Sound::SE_stop(string SE_mode)
{
	m_SourceVoice[SoundIndex[SE_mode]]->Stop(0, XAUDIO2_COMMIT_NOW);
}