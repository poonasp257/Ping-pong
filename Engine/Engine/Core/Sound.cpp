#include "stdafx.h"

Sound::Sound() {
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	m_secondaryBuffer = 0;
}

Sound::~Sound() {
	ShutdownWaveFile(&m_secondaryBuffer);
	ShutdownDirectSound();
}

bool Sound::Initialize(HWND hwnd, const char* fileName) {
	bool result;

	result = InitializeDirectSound(hwnd);
	if (!result) return false;

	result = LoadWaveFile(fileName, &m_secondaryBuffer);
	if (!result) return false;

	return true;
}

bool Sound::InitializeDirectSound(HWND hwnd) {
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result)) return false;

	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result)) return false;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result)) return false;

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result)) return false;

	return true;
}

void Sound::ShutdownDirectSound() {
	if (m_primaryBuffer) {
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}
	
	if (m_DirectSound) {
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

bool Sound::LoadWaveFile(const char* filename, IDirectSoundBuffer8** secondaryBuffer) {
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1) return false;

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}
	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}
	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}
	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}
	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}
	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}
	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result)) return false;

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result)) return false;

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);
	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData) return false;
	
	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize) return false;

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0) return false;

	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize,
		NULL, 0, 0);
	if (FAILED(result)) return false;

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);
	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result)) return false;

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;
	return true;
}

void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer) {
	// Release the secondary sound buffer.
	if (*secondaryBuffer) {
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

bool Sound::PlayWaveFile(long volume, bool loop) {	
	HRESULT result;
	// Set position at the beginning of the sound buffer.
	result = m_secondaryBuffer->SetCurrentPosition(0);
	if (FAILED(result)) return false;

	// Set volume of the buffer to 100%.
	result = m_secondaryBuffer->SetVolume(volume);
	if (FAILED(result)) return false;

	// Play the contents of the secondary sound buffer.
	result = m_secondaryBuffer->Play(0, 0, (loop ? 1 : 0));
	if (FAILED(result)) return false;

	return true;
}

bool Sound::StopWaveFile() {
	DWORD flag;
	m_secondaryBuffer->GetStatus(&flag);
	if (flag == DSBSTATUS_PLAYING) { return false; }

	HRESULT result;
	// Set position at the beginning of the sound buffer.
	result = m_secondaryBuffer->SetCurrentPosition(0);
	if (FAILED(result)) return false;

	// Set volume of the buffer to 100%.
	result = m_secondaryBuffer->SetVolume(DSBVOLUME_MIN);
	if (FAILED(result)) return false;

	// Play the contents of the secondary sound buffer.
	result = m_secondaryBuffer->Stop();
	if (FAILED(result)) return false;

	return true;
}