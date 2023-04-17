#include "GameController.h"
#include "Renderer.h"

#include "SpriteAnim.h"
#include "SpriteSheet.h"
#include "TTFont.h"
#include "Timing.h"
#include "InputController.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Controller.h"
#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"
#include "WavDraw.h"
#include "Level.h"

GameController::GameController() {
	m_sdlEvent = {};
	m_renderer = nullptr;
	m_fArial20 = nullptr;
	m_fArial40 = nullptr;
	m_quit = false;
	m_input = nullptr;
	m_audio = nullptr;
	memset(m_effects, 0, sizeof(SoundEffect*) * MaxEffectsPerChannels);
	m_song = nullptr;

	m_text = "";
	m_text2 = "";

	m_smPos = "";
	m_wavDraw = nullptr;
	m_zoomY = 5;
	m_mPos = {};
	m_ctInfo = "";
}

GameController::~GameController() {
	ShutDown();
}

void GameController::ShutDown() {
	delete m_fArial20;
	delete m_wavDraw;
	delete m_fArial40;
}

void GameController::InitializeLevelOne() {

}

void GameController::InitializeLevelTwo() {

}


void GameController::SaveLevelTwo() {

}

void GameController::LoadLevelTwo() {

}

void GameController::LoadLevelTwoHud(int GameTime, int FramesPerSecond) {
	m_fArial20->Write(m_renderer->GetRenderer(), string("Quit [ESC]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 0,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Next Game State [SPACE]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 125,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Save [F5]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 400,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Load [F7]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 525,1080 - m_fArial20->GetPointSize() });

	int UpperOffset = 30;
	m_fArial20->Write(m_renderer->GetRenderer(), string("Frames Per Seconds: " + to_string(FramesPerSecond)).c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 0,1080 - m_fArial20->GetPointSize() - UpperOffset });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Game Time: " + to_string(GameTime)).c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 280,1080 - m_fArial20->GetPointSize() - UpperOffset });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Saves: ").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 500,1080 - m_fArial20->GetPointSize() - UpperOffset });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Loads: ").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 615,1080 - m_fArial20->GetPointSize() - UpperOffset });

}


void GameController::LoadLevelOneHud() {
	m_fArial20->Write(m_renderer->GetRenderer(), string("Quit [ESC]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 0,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Next Nickname [RETURN]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 125,1080 - m_fArial20->GetPointSize() });
}

void GameController::Initialize() {
	AssetController::Instance().Initialize(10000000);
	m_renderer = &Renderer::Instance();
	m_renderer->Initialize(1920, 900);
	m_input = &InputController::Instance();
	m_fArial20 = new TTFont();
	m_fArial20->Initialize(20);


	m_fArial40 = new TTFont();
	m_fArial40->Initialize(40);

	m_audio = &AudioController::Instance();

	m_effects[0] = m_audio->LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
	m_effects[1] = m_audio->LoadEffect("../Assets/Audio/Effects/DistantGunshot.mp3");
	m_song = m_audio->LoadSong("../Assets/Audio/Music/Track1.mp3");
}

void GameController::HandleInput(SDL_Event event, bool& NotValidUserName, bool& PlayerOne) {
	string temp;

	if (m_sdlEvent.type == SDL_QUIT || m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)) {
		m_quit = true;
	}
	else if ((temp = m_input->KB()->TextInput(event)) != "") {

		if (PlayerOne) {
			m_text += temp;

		}
		else {
			m_text2 += temp;

		}
	}
	//if backspace is pressed we want to remove from the text input buffer.
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_BACKSPACE) && !m_text.empty()) {
		if (PlayerOne) {
			m_text.erase(m_text.length() - 1);
		}
		else {

			m_text2.erase(m_text.length() - 1);
		}

	}
	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_RETURN)) {
		//Check if we are currently on the first or second players name

		//depending on the name we can then do the certain check too see if it is empty and display the correct message.
		if (PlayerOne) {

			if (m_text.length() <= 0 || m_text.length() > 21) {
				cout << "in here because this is not a valid name for this first user user" << endl;
				NotValidUserName = true;
				return;
			}
		}

		if (!PlayerOne) {

			if (m_text2.length() <= 0 || m_text2.length() > 21) {
				cout << "in here because this is not a valid name for this user" << endl; \
					NotValidUserName = true;
				return;
			}

		}

		//If valid we continue to the next player name 

		//otherwise if both players have been set we continue to the next level now.
		NotValidUserName = false;
		PlayerOne = false;
	}
	else if (m_input->MS()->Moved(m_sdlEvent, m_mPos)) {
		m_smPos = "Mouse Position [" + to_string(m_mPos.X) + ";" + to_string(m_mPos.Y) + "]";
	}
	else if ((m_input->CT()->Added(m_sdlEvent)) || (m_input->CT()->Removed(m_sdlEvent)) || (m_input->CT()->ProcessButtons(m_sdlEvent)) || (m_input->CT()->ProcessMotion(m_sdlEvent))) {
		m_ctInfo = m_input->CT()->ToString();
	}
	else {
		m_input->MS()->ProcessButtons(event);
	}
}


void GameController::RunGame() {
	Initialize();
	Timing* t = &Timing::Instance();
	Level* CurrLevel = new Level();

	float GameTime = 0.0f;
	int SaveCount = 0;
	int LoadCount = 0;

	//Load in the TGA background for the first level;
	SpriteSheet::Pool = new ObjectPool<SpriteSheet>(); // these will eventually have to be moved to units when posssible.
	SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

	SpriteSheet* Level1Background;
	Level1Background = SpriteSheet::Pool->GetResources();
	Level1Background->Load("../Assets/Textures/Background.tga");
	Level1Background->SetSize(1, 1, 1920, 1080);
	Level1Background->point = new Point(1920 / 2, 1080 / 2);

	Point ws = m_renderer->GetWindowSize();

	bool PlayerOne = true;
	bool MainMenuFirstTime = false;
	bool NotValidName = false;


	while (!m_quit) {

		if (!MainMenuFirstTime) {
			cout << "Now starting the song by default" << endl;
			m_audio->Play(m_song);
			MainMenuFirstTime = true;
		}

		//Seed all the random values so they are not the same.
		srand(time(NULL));

		t->Tick();
		m_renderer->SetDrawColour(Color(255, 255, 255, 255));
		m_renderer->ClearScreen();

		while (SDL_PollEvent(&m_sdlEvent) != 0) {
			HandleInput(m_sdlEvent, NotValidName, PlayerOne);
		}

		m_renderer->RenderTexture(Level1Background, Rect(0, 0, 1920, 1080));
		m_fArial40->Write(m_renderer->GetRenderer(), m_text.c_str(), SDL_Color{ 196,180,84 }, SDL_Point{ 50,70 });
		m_fArial40->Write(m_renderer->GetRenderer(), m_text2.c_str(), SDL_Color{ 196,180,84 }, SDL_Point{ 50, 200 });

		if (NotValidName) {
			m_fArial40->Write(m_renderer->GetRenderer(), string("Sorry but the username must be between 1 and 20 characters inclusive!").c_str(), SDL_Color{ 255,0,0 }, SDL_Point{ 27, 300 });
		}

		GameTime += t->GetDeltaTime();
		LoadLevelTwoHud(GameTime, t->GetFPS());
		//LoadLevelOneHud();

		SDL_RenderPresent(m_renderer->GetRenderer());
	}
}
