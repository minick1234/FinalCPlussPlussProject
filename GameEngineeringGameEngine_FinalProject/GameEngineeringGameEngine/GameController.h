#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "AudioController.h"

class Renderer;
class TTFont;
class InputController;
class AudioController;
class SoundEffect;
class Song;
class WavDraw;

class GameController : public Singleton<GameController> {


public : 
	GameController();
	virtual ~GameController();

	void RunGame();
	void Initialize();
	void HandleInput(SDL_Event _event);
	void HandleInput(SDL_Event _event, bool &NotValidUserName, bool& playerOne);

	void InitializeLevelOne();
	void InitializeLevelTwo();
	void SaveLevelTwo();
	void LoadLevelTwo();
	void LoadLevelTwoHud(int GameTime, int FramesPerSecond);
	void LoadLevelOneHud();
	void ShutDown();


private :
	SDL_Event m_sdlEvent;
	Renderer* m_renderer;
	TTFont* m_fArial20;
	TTFont* m_fArial40;


	bool m_quit;
	InputController* m_input;
	
	string m_text;
	string m_text2;

	string m_smPos;
	Point m_mPos;
	string m_ctInfo;
	AudioController* m_audio;
	Song* m_song;
	WavDraw* m_wavDraw;
	SoundEffect* m_effects[MaxEffectsPerChannels];
	float m_zoomY;
	float m_zoomX;
};
#endif // !GAME_CONTROLLER_H
