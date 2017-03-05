/*
	@file			Sound.cpp
	@brief		サウンド
	@date		2017/03/02
	@author	金澤信芳
*/

#include "Sound.h"

map<string, int> SoundIndex; // サウンド保管庫

/*
	@brief	コンストラクタ
*/
Sound::Sound()
{
	ZeroMemory(this, sizeof(Sound));
	m_Flag = true;
}

/*
	@brief	デストラクタ
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
	@brief　サウンドの初期化
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
	@brief	サウンドの準備
*/
void Sound::SoundSet()
{
	InitSound(); // サウンドの初期化

	SoundIndex["TEST1"] = LoadSound("Sound/BGM/DonkeyDonk.wav");
	SoundIndex["TEST2"] = LoadSound("Sound/BGM/AO-1.wav");

}

/*
	@brief	wavファイルの読み込み
*/
int Sound::LoadSound(char* szFileName)
{
	static int iIndex = -1;
	iIndex++;
	HMMIO						hMmio = NULL;																					//WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
	DWORD						dwWavSize = 0;																					//WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
	WAVEFORMATEX*		pwfex;																								//WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
	MMCKINFO				ckInfo;																								//　チャンク情報
	MMCKINFO				riffckInfo;																							// 最上部チャンク（RIFFチャンク）保存用
	PCMWAVEFORMAT		pcmWaveForm;

	hMmio =					mmioOpenA(szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);		//WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
	mmioDescend(hMmio, &riffckInfo, NULL, 0);																		//ファイルポインタをRIFFチャンクの先頭にセットする
	ckInfo.ckid =				mmioFOURCC('f', 'm', 't', ' ');																// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

	//フォーマットを読み込む
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);

	// WAVファイル内の音データの読み込み	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);											//データチャンクにセット
	dwWavSize = ckInfo.cksize;
	m_WavBuffer[iIndex] = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_WavBuffer[iIndex], dwWavSize);

	//ソースボイスにデータを詰め込む	
	if (FAILED(m_XAudio2->CreateSourceVoice(&m_SourceVoice[iIndex], pwfex)))
	{
		MessageBox(0, L"ソースボイス作成失敗", 0, MB_OK);
		return E_FAIL;
	}
	m_WavSize[iIndex] = dwWavSize;

	return iIndex;
}

/*
	@brief	サウンドの再生
*/
void  Sound::PlaySound(int SoundIndex, bool LoopFlag)
{
	m_SourceVoice[SoundIndex]->Stop(0, 1);
	m_SourceVoice[SoundIndex]->FlushSourceBuffers();
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_WavBuffer[SoundIndex];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_WavSize[SoundIndex];

	if (LoopFlag) // ループの判定
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	if (FAILED(m_SourceVoice[SoundIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, L"ソースボイスにサブミット失敗", 0, MB_OK);
		return;
	}

	m_SourceVoice[SoundIndex]->Start(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	BGMの再生
*/
void Sound::BGM_play(string BGM_mode)
{
	PlaySound(SoundIndex[BGM_mode], true);
}

/*
	@brief	BGMの停止
*/
void Sound::BGM_stop(string BGM_mode)
{
	m_SourceVoice[SoundIndex[BGM_mode]]->Stop(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	SEの再生
*/
void Sound::SE_play(string SE_mode)
{
	PlaySound(SoundIndex[SE_mode], false);
}

/*
	@brief	SEの停止
*/
void Sound::SE_stop(string SE_mode)
{
	m_SourceVoice[SoundIndex[SE_mode]]->Stop(0, XAUDIO2_COMMIT_NOW);
}