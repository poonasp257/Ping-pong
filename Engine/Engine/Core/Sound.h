#ifndef SOUND_H
#define SOUND_H

class Sound {
private:
	struct WaveHeaderType {
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

private:
	IDirectSound8		*m_DirectSound;
	IDirectSoundBuffer	*m_primaryBuffer;
	IDirectSoundBuffer8	*m_secondaryBuffer;

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();
	bool LoadWaveFile(const char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

public:
	Sound();
	~Sound();
	
	bool Initialize(HWND, const char* fileName);

	bool PlayWaveFile(long volume, bool loop);
	bool StopWaveFile();
};
#endif