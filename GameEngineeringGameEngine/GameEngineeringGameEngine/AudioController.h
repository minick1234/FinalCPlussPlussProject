#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "StandardIncludes.h"

constexpr int MaxEffectsPerChannels = 16;


class SoundEffect;
class Song;

class AudioController : public Singleton<AudioController> {
public:
	AudioController();
	virtual	~AudioController();

	string* GetCurrentEffects() { return m_currentEffects; }
	string GetMusicTitle() { return m_musicTitle; }
	string GetMusicLength() { return m_musicLength; }

	SoundEffect* LoadEffect(string _guid);
	Song* LoadSong(string _guid);
	void Play(SoundEffect* _effect);
	void Play(Song* _song);
	void StopMusic();
	void PauseMusic();
	void ResumeMusic();
	double MusicPosition();

	void Shutdown();

	static void CatchChannelDone(int _channel);

private:

	Mix_Chunk* GetSDLSFX(SoundEffect * effect);
	Mix_Music* GetSDLMusic(Song * song);

	map<string, Mix_Chunk*> m_effects;
	map<string, Mix_Music*> m_songs;
	string m_currentEffects[MaxEffectsPerChannels];

	string m_musicLength;
	string m_musicTitle;
	Mix_Music* m_currentSong;

};


#endif // !AUDIO_CONTROLLER_H
