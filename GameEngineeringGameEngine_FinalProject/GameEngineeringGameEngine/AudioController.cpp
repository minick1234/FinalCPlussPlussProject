#include "AudioController.h"
#include "Song.h"
#include "SoundEffect.h"



AudioController::AudioController() {
	M_ASSERT((Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0), "Failed to initialize Mix Audio");
	SoundEffect::Pool = new ObjectPool<SoundEffect>();
	m_currentSong = nullptr;
}

AudioController::~AudioController() {
	Shutdown();
}

SoundEffect* AudioController::LoadEffect(string _guid) {
	SoundEffect* effect = SoundEffect::Pool->GetResources();
	effect->Load(_guid);
	return effect;
}

Song* AudioController::LoadSong(string _guid) {

	SoundEffect* song = SoundEffect::Pool->GetResources();
	song->Load(_guid);
	return static_cast<Song*>(song);
}

void AudioController::Play(SoundEffect* effect) {

	int channel;
	channel = Mix_PlayChannel(-1,GetSDLSFX(effect),0);
	if (channel == -1)return;
	m_currentEffects[channel] = effect->GetData()->GetGUID();
	Mix_ChannelFinished(AudioController::CatchChannelDone);
}

void AudioController::Play(Song* song) {
	m_currentSong = GetSDLMusic(song);
	M_ASSERT(Mix_PlayMusic(m_currentSong, -1) != -1, "Failed to play song!");
	m_musicTitle = string(Mix_GetMusicTitle(m_currentSong));
	m_musicLength = to_string((int)Mix_MusicDuration(m_currentSong));
}

void AudioController::CatchChannelDone(int channel) {
	AudioController::Instance().m_currentEffects[channel] = "";
}

Mix_Chunk* AudioController::GetSDLSFX(SoundEffect * effect) {
	Asset* asset = effect->GetData();
	string guid = asset->GetGUID();

	if (m_effects.count(guid) == 0) {
		SDL_RWops* rw = SDL_RWFromMem((void*) asset->GetData(), asset->GetDataSize());
		Mix_Chunk* audio = Mix_LoadWAV_RW(rw,0);
		m_effects[guid] = audio;
	}
	return m_effects[guid];
}

Mix_Music* AudioController::GetSDLMusic(Song * song) {
	Asset* asset = song->GetData();
	string guid = asset->GetGUID();

	if (m_songs.count(guid) == 0) {
		SDL_RWops* rw = SDL_RWFromMem((void*)asset->GetData(),asset->GetDataSize());
		Mix_Music* audio = Mix_LoadMUS_RW(rw,0);
		m_songs[guid] = audio;
	}
	return m_songs[guid];
}

void AudioController::StopMusic() {
	m_currentSong = nullptr;
	m_musicTitle = "";
	m_musicLength = "";
	Mix_HaltMusic();
}

void AudioController::PauseMusic() {
	Mix_PauseMusic();
}

void AudioController::ResumeMusic() {
	Mix_ResumeMusic();
}

void AudioController::Shutdown() {
	for (auto ie = m_effects.begin(); ie != m_effects.end(); ie++) {
		Mix_FreeChunk(ie->second);
	}
	m_effects.clear();

	for (auto is = m_songs.begin(); is != m_songs.end(); is++) {
		Mix_FreeMusic(is->second);
	}
	m_songs.clear();

	delete SoundEffect::Pool;
	SoundEffect::Pool = nullptr;

	Mix_Quit();
}


double AudioController::MusicPosition() {
	if (m_currentSong == nullptr) return 0;

	return Mix_GetMusicPosition(m_currentSong);
}
