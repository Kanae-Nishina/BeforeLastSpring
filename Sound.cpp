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
	: m_flag(true)
{
	ZeroMemory(this, sizeof(Sound));
}

/*
	@brief	�f�X�g���N�^
*/
Sound::~Sound()
{
	for (int i = 0; i < max_wav; i++)
	{
		if (m_sourceVoice[i]) m_sourceVoice[i]->DestroyVoice();
	}
	for (int i = 0; i<max_wav; i++) SAFE_DELETE(m_wavBuffer[i]);
	SAFE_RELEASE(m_audio2);
}

/*
	@brief�@�T�E���h�̏�����
*/
HRESULT Sound::Init()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(XAudio2Create(&m_audio2, 0)))
	{
		CoUninitialize();
		return E_FAIL;
	}
	if (FAILED(m_audio2->CreateMasteringVoice(&m_masteringVoice)))
	{
		CoUninitialize();
		return E_FAIL;
	}

	Set();

	return S_OK;
}

/*
	@brief	�T�E���h�̃��[�h
*/
void Sound::Set()
{
	SoundIndex["TestSE"] = Load("Sound/SE/openhihat.wav");
	SoundIndex["TestBGM"] = Load("Sound/BGM/AO-1.wav");
	m_sourceVoice[SoundIndex["TestBGM"]]->SetVolume(1.0f, 0);

}

/*
	@brief	wav�t�@�C���̓ǂݍ���
*/
int Sound::Load(char* szFileName)
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
	m_wavBuffer[iIndex] = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_wavBuffer[iIndex], dwWavSize);

	//�\�[�X�{�C�X�Ƀf�[�^���l�ߍ���	
	if (FAILED(m_audio2->CreateSourceVoice(&m_sourceVoice[iIndex], pwfex)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�쐬���s", 0, MB_OK);
		return E_FAIL;
	}
	m_wavSize[iIndex] = dwWavSize;

	return iIndex;
}

/*
	@brief	�T�E���h�̍Đ�
*/
void  Sound::PlaySound(int soundIndex, bool loopFlag)
{
	m_sourceVoice[soundIndex]->Stop(0, 1);
	m_sourceVoice[soundIndex]->FlushSourceBuffers();
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_wavBuffer[soundIndex];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_wavSize[soundIndex];

	if (loopFlag) // ���[�v�̔���
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	if (FAILED(m_sourceVoice[soundIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�ɃT�u�~�b�g���s", 0, MB_OK);
		return;
	}

	m_sourceVoice[soundIndex]->Start(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	BGM�̍Đ�
*/
void Sound::PlayBGM(string name)
{
	PlaySound(SoundIndex[name], true);
}

/*
	@brief	BGM�̒�~
*/
void Sound::StopBGM(string name)
{
	m_sourceVoice[SoundIndex[name]]->Stop(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	SE�̍Đ�
*/
void Sound::PlaySE(string name)
{
	PlaySound(SoundIndex[name], false);
}

/*
	@brief	SE�̒�~
*/
void Sound::StopSE(string name)
{
	m_sourceVoice[SoundIndex[name]]->Stop(0, XAUDIO2_COMMIT_NOW);
}