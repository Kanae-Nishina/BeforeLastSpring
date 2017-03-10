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
	: m_flag(true)
{
	ZeroMemory(this, sizeof(Sound));
}

/*
	@brief	デストラクタ
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
	@brief　サウンドの初期化
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
	@brief	サウンドのロード
*/
void Sound::Set()
{
	SoundIndex["TestSE"] = Load("Sound/SE/openhihat.wav");
	SoundIndex["TestBGM"] = Load("Sound/BGM/AO-1.wav");
	m_sourceVoice[SoundIndex["TestBGM"]]->SetVolume(1.0f, 0);

}

/*
	@brief	wavファイルの読み込み
*/
int Sound::Load(char* szFileName)
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
	m_wavBuffer[iIndex] = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_wavBuffer[iIndex], dwWavSize);

	//ソースボイスにデータを詰め込む	
	if (FAILED(m_audio2->CreateSourceVoice(&m_sourceVoice[iIndex], pwfex)))
	{
		MessageBox(0, L"ソースボイス作成失敗", 0, MB_OK);
		return E_FAIL;
	}
	m_wavSize[iIndex] = dwWavSize;

	return iIndex;
}

/*
	@brief	サウンドの再生
*/
void  Sound::PlaySound(int soundIndex, bool loopFlag)
{
	m_sourceVoice[soundIndex]->Stop(0, 1);
	m_sourceVoice[soundIndex]->FlushSourceBuffers();
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_wavBuffer[soundIndex];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_wavSize[soundIndex];

	if (loopFlag) // ループの判定
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	if (FAILED(m_sourceVoice[soundIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, L"ソースボイスにサブミット失敗", 0, MB_OK);
		return;
	}

	m_sourceVoice[soundIndex]->Start(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	BGMの再生
*/
void Sound::PlayBGM(string name)
{
	PlaySound(SoundIndex[name], true);
}

/*
	@brief	BGMの停止
*/
void Sound::StopBGM(string name)
{
	m_sourceVoice[SoundIndex[name]]->Stop(0, XAUDIO2_COMMIT_NOW);
}

/*
	@brief	SEの再生
*/
void Sound::PlaySE(string name)
{
	PlaySound(SoundIndex[name], false);
}

/*
	@brief	SEの停止
*/
void Sound::StopSE(string name)
{
	m_sourceVoice[SoundIndex[name]]->Stop(0, XAUDIO2_COMMIT_NOW);
}