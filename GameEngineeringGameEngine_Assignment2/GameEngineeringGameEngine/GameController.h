#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "Renderer.h"
#include "SpriteAnim.h"
#include "SpriteSheet.h"
#include "TTFont.h"
#include "Timing.h"


class GameController : public Singleton<GameController> {


public:
	GameController();
	virtual ~GameController();

	void RunGame();
	void DrawTopHud(int GameTime, bool AutoSaved, Timing* t, Renderer* r, TTFont * font);

private:
	SDL_Event m_sdlEvent;

};





#endif // !GAME_CONTROLLER_H
