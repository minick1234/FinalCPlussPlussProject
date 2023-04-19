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

void GameController::InitializeLevelTwo() {

}


void GameController::SaveLevelTwo() {

}

void GameController::LoadLevelTwo() {

}

void GameController::LoadLevelTwoHud(int GameTime, int FramesPerSecond, int Saves, int Loads) {
	m_fArial20->Write(m_renderer->GetRenderer(), string("Quit [ESC]").c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 0,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Next Game State [SPACE]").c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 125,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Save [F5]").c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 400,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Load [F7]").c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 525,1080 - m_fArial20->GetPointSize() });

	int UpperOffset = 30;
	m_fArial20->Write(m_renderer->GetRenderer(), string("Frames Per Seconds: " + to_string(FramesPerSecond)).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 0,1080 - m_fArial20->GetPointSize() - UpperOffset });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Game Time: " + to_string(GameTime)).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 280,1080 - m_fArial20->GetPointSize() - UpperOffset });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Saves: " + to_string(Saves)).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 500,1080 - m_fArial20->GetPointSize() - UpperOffset });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Loads: " + to_string(Loads)).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 615,1080 - m_fArial20->GetPointSize() - UpperOffset });

}


void GameController::LoadLevelOneHud() {
	m_fArial20->Write(m_renderer->GetRenderer(), string("Quit [ESC]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 0,1080 - m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Next Nickname [RETURN]").c_str(), SDL_Color{ 255,255,255 }, SDL_Point{ 125,1080 - m_fArial20->GetPointSize() });
}

void GameController::PrintPlayerHud(string playerText, Point PlayerPosition, int GameState, int WinState, int wins, int losses) {
	m_fArial20->Write(m_renderer->GetRenderer(), string(playerText).c_str(), SDL_Color{ 0,255,0 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 });

	if (GameState == 1) {
		m_fArial20->Write(m_renderer->GetRenderer(), string("ROLLING..... ").c_str(), SDL_Color{ 255,0,0 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 + m_fArial20->GetPointSize() });
	}
	else if (GameState == 2 && WinState == 1) {
		m_fArial20->Write(m_renderer->GetRenderer(), string("WINNER!").c_str(), SDL_Color{ 255,0,0 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 + m_fArial20->GetPointSize() });

	}
	else if (GameState == 2 && WinState == 2) {
		m_fArial20->Write(m_renderer->GetRenderer(), string("LOSER!").c_str(), SDL_Color{ 255,0,0 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 + m_fArial20->GetPointSize() });

	}
	else if (GameState == 2 && WinState == 3) {
		m_fArial20->Write(m_renderer->GetRenderer(), string("DRAW!").c_str(), SDL_Color{ 255,0,0 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 + m_fArial20->GetPointSize() });

	}
	else if (GameState == 0) {
		m_fArial20->Write(m_renderer->GetRenderer(), string("Waiting To Start..... ").c_str(), SDL_Color{ 255,0,0 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 + m_fArial20->GetPointSize() });
	}


	m_fArial20->Write(m_renderer->GetRenderer(), string("Wins: " + to_string(wins)).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 + m_fArial20->GetPointSize() + m_fArial20->GetPointSize() });
	m_fArial20->Write(m_renderer->GetRenderer(), string("Losses: " + to_string(losses)).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ (int)PlayerPosition.X, (int)PlayerPosition.Y + 15 + m_fArial20->GetPointSize() + m_fArial20->GetPointSize() + m_fArial20->GetPointSize() });

}


void GameController::Initialize() {
	AssetController::Instance().Initialize(25000000);
	m_renderer = &Renderer::Instance();
	m_renderer->Initialize(1920, 1080);
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

void GameController::HandleInputLevelOne(SDL_Event event, bool& NotValidUserName, bool& PlayerOne, bool& LevelComplete) {
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
	else if ((m_input->CT()->Added(m_sdlEvent)) || (m_input->CT()->Removed(m_sdlEvent)) || (m_input->CT()->ProcessButtons(m_sdlEvent)) || (m_input->CT()->ProcessMotion(m_sdlEvent))) {
		m_ctInfo = m_input->CT()->ToString();
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

		if (!PlayerOne) {
			LevelComplete = true;
		}
		else {
			PlayerOne = false;
		}

		NotValidUserName = false;
	}
	else if (m_input->MS()->Moved(m_sdlEvent, m_mPos)) {
		m_smPos = "Mouse Position [" + to_string(m_mPos.X) + ";" + to_string(m_mPos.Y) + "]";
	}
	else {
		m_input->MS()->ProcessButtons(event);
	}
}

void GameController::HandleInputLevelTwo(SDL_Event event, int& GameState, int& randomValue1, int& randomValue2, bool& HasPickedWinner, Point* playerOnePosition, Point* playerTwoPosition, int levelXSize, int levelYSize, int clipXSize, int clipYSize, bool& SavedPressed, bool& LoadPressed)
{
	string temp;
	m_input->MS()->ProcessButtons(event);

	float moveSpeed = 5.0f;

	if (m_sdlEvent.type == SDL_QUIT || m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)) {
		m_quit = true;
	}
	else if (m_input->MS()->Moved(m_sdlEvent, m_mPos)) {
		m_smPos = "Mouse Position [" + to_string(m_mPos.X) + ";" + to_string(m_mPos.Y) + "]";
	}

	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_w)) {
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->Y - moveSpeed, levelYSize, clipYSize, false, false))
		{
			playerOnePosition->Y -= moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_d)) {
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->X + moveSpeed, levelXSize, clipXSize, false, false))
		{
			playerOnePosition->X += moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_s)) {
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->Y + moveSpeed, levelYSize, clipYSize, false, false))
		{
			playerOnePosition->Y += moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_a)) {
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->X - moveSpeed, levelXSize, clipXSize, false, false))
		{
			playerOnePosition->X -= moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_UP)) {
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->Y - moveSpeed, levelYSize, clipYSize, false, false))
		{
			playerTwoPosition->Y -= moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_RIGHT)) {
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->X + moveSpeed, levelXSize, clipXSize, false, false))
		{
			playerTwoPosition->X += moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_DOWN))
	{
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->Y + moveSpeed, levelYSize, clipYSize, false, false))
		{
			playerTwoPosition->Y += moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_LEFT))
	{
		if (!CheckIfWillBeOutOfBounds(playerOnePosition->X - moveSpeed, levelXSize, clipXSize, false, false))
		{
			playerTwoPosition->X -= moveSpeed;
		}
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_F5))
	{
		SavedPressed = true;
	}
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_F7))
	{
		LoadPressed = true;
	}



	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_SPACE)) {
		cout << "the space bar is clicked." << endl;
		if (GameState == 0) {
			m_audio->Play(m_effects[0]);

			GameState = 1;
		}
		else if (GameState == 1) {
			GameState = 2;
			m_audio->Play(m_effects[1]);

			srand(time(NULL) * (rand() % 1000));
			randomValue1 = rand() % 3;
			cout << randomValue1 << endl;

			srand(time(NULL) * (rand() % 1000));
			randomValue2 = rand() % 3;
			cout << randomValue2 << endl;

		}
		else if (GameState == 2) {
			GameState = 0;
			HasPickedWinner = false;
		}
	}
}

bool GameController::CheckIfWillBeOutOfBounds(float PlayerPosition, float ScreenSize, float PlayerSize, bool MouseDrag, bool XCheck) {

	if (MouseDrag && XCheck) {
		if (PlayerPosition < 0) {
			cout << "outside of bounds on the left side" << endl;
			return true;
		}
		else if (PlayerPosition + (PlayerSize - 20) > ScreenSize) {
			cout << "outside of bounds on the right side" << endl;
			return true;
		}
		return false;
	}
	else if (MouseDrag && !XCheck) {
		if (PlayerPosition < 0) {
			cout << "outside of bounds on the Upper side" << endl;
			return true;
		}
		else if (PlayerPosition + (PlayerSize) > ScreenSize) {
			cout << "outside of bounds on the Lower side" << endl;
			return true;
		}
		return false;
	}
	else {
		if (XCheck) {
			if (PlayerPosition - 5 < 0) {
				cout << "outside of bounds left" << endl;
				return true;
			}
			else if (PlayerPosition + (PlayerSize - 10) > ScreenSize) {
				cout << "outside of bounds right" << endl;
				return true;
			}
			return false;
		}
		else {
			if (PlayerPosition < 0) {
				cout << "outside of bounds up" << endl;
				return true;
			}
			else if (PlayerPosition + (PlayerSize - 5) > ScreenSize) {
				cout << "outside of bounds down" << endl;
				return true;
			}
			return false;
		}


	}



}

void GameController::RunGame() {
	Initialize();
	Timing* t = &Timing::Instance();
	Level* CurrentLevel = new Level();
	CurrentLevel->SetGameTime(t->GetDeltaTime());
	CurrentLevel->SetLevelNumber(1);
	CurrentLevel->SetMapSize(1920, 1080);

	Level* Level2 = new Level();
	Level* Level1 = new Level();

	float GameTime = 0.0f;
	int SaveCount = 0;
	int LoadCount = 0;
	bool LevelComplete = false;


	SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
	SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

	SpriteSheet* Level1Background;
	Level1Background = SpriteSheet::Pool->GetResources();
	Level1Background->Load("../Assets/Textures/Background.tga");
	Level1Background->SetSize(1, 0, m_renderer->GetWindowSize().X, m_renderer->GetWindowSize().Y);
	Level1Background->point = new Point(0, 0);

	Point ws = m_renderer->GetWindowSize();

	bool PlayerOne = true;
	bool MainMenuFirstTime = false;
	bool NotValidName = false;

	float PlayerAnimationSpeed = 8.0f;
	float PlayerScale = 1.0f;

	int GameState = 0;
	int PlayerOneWinState = 0;
	int PlayerTwoWinState = 0;

	int PlayerOneWinCount = 0;
	int PlayerTwoWinCount = 0;

	int PlayerOneLossesCount = 0;
	int PlayerTwoLossesCount = 0;
	int randomValue1 = 0;
	int randomValue2 = 0;

	bool HasPickedWinner = false;
	bool SaveGame = false;
	bool LoadGame = false;

	SpriteSheet* PlayerOneSheet;
	PlayerOneSheet = SpriteSheet::Pool->GetResources();

	PlayerOneSheet->Load("../Assets/Textures/RockPaperScissors.tga");
	PlayerOneSheet->SetSize(4, 4, 280, 170);
	PlayerOneSheet->AddAnimation(EN_AN_RPS_IDLE, 0, 0, PlayerAnimationSpeed);
	PlayerOneSheet->AddAnimation(EN_AN_RPS_ROLL, 0, 4, PlayerAnimationSpeed);
	PlayerOneSheet->AddAnimation(EN_AN_RPS_PAPER, 4, 4, PlayerAnimationSpeed);
	PlayerOneSheet->AddAnimation(EN_AN_RPS_SCISSORS, 8, 4, PlayerAnimationSpeed);
	PlayerOneSheet->AddAnimation(EN_AN_RPS_ROCK, 12, 4, PlayerAnimationSpeed);
	PlayerOneSheet->point = new Point(20, 30);


	SpriteSheet* PlayerTwoSheet;
	PlayerTwoSheet = SpriteSheet::Pool->GetResources();
	PlayerTwoSheet->Load("../Assets/Textures/RockPaperScissors.tga");
	PlayerTwoSheet->SetSize(4, 4, 280, 170);
	PlayerTwoSheet->AddAnimation(EN_AN_RPS_IDLE, 0, 0, PlayerAnimationSpeed);
	PlayerTwoSheet->AddAnimation(EN_AN_RPS_ROLL, 0, 4, PlayerAnimationSpeed);
	PlayerTwoSheet->AddAnimation(EN_AN_RPS_PAPER, 4, 4, PlayerAnimationSpeed);
	PlayerTwoSheet->AddAnimation(EN_AN_RPS_SCISSORS, 8, 4, PlayerAnimationSpeed);
	PlayerTwoSheet->AddAnimation(EN_AN_RPS_ROCK, 12, 4, PlayerAnimationSpeed);
	PlayerTwoSheet->point = new Point(20, 330);


	while (!m_quit)
	{

		if (CurrentLevel->GetLevelNumber() == 1)
		{
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


			if (LevelComplete) {
				CurrentLevel->SetLevelNumber(2);
				m_renderer->ClearScreen();
				continue;
			}

			while (SDL_PollEvent(&m_sdlEvent) != 0) {
				HandleInputLevelOne(m_sdlEvent, NotValidName, PlayerOne, LevelComplete);
			}

			m_renderer->RenderTexture(Level1Background, Rect(0, 0, 1920, 1080));
			m_fArial40->Write(m_renderer->GetRenderer(), m_text.c_str(), SDL_Color{ 196,180,84 }, SDL_Point{ 50,70 });
			m_fArial40->Write(m_renderer->GetRenderer(), m_text2.c_str(), SDL_Color{ 196,180,84 }, SDL_Point{ 50, 200 });

			if (NotValidName) {
				m_fArial40->Write(m_renderer->GetRenderer(), string("Sorry but the username must be between 1 and 21 characters inclusive!").c_str(), SDL_Color{ 255,0,0 }, SDL_Point{ 27, 300 });
			}

			GameTime += t->GetDeltaTime();
			LoadLevelOneHud();
		}
		else if (CurrentLevel->GetLevelNumber() == 2)
		{
			t->Tick();

			if (SaveGame) {
				SaveGame = false;
				//save the game here.
				ofstream writeStream("saveGame.bin", ios::out | ios::binary);
				CurrentLevel->Serialize(writeStream);
				writeStream.close();
				SaveCount++;
			}

			if (LoadGame) {
				LoadGame = false;
				ifstream readStream("saveGame.bin", ios::in | ios::binary);
				CurrentLevel->DeSerialize(readStream);
				readStream.close();
				LoadCount++;
			}


			m_renderer->ClearScreen();

			while (SDL_PollEvent(&m_sdlEvent) != 0) {
				HandleInputLevelTwo(m_sdlEvent, GameState, randomValue1, randomValue2, HasPickedWinner, PlayerOneSheet->point, PlayerTwoSheet->point, CurrentLevel->GetLevelMapSizeX(), CurrentLevel->GetLevelMapSizeY(), PlayerOneSheet->GetClipSizeX(), PlayerOneSheet->GetClipSizeY(), SaveGame, LoadGame);

				if (m_input->MS()->GetButLDown())
				{
					if (!CheckIfWillBeOutOfBounds(m_mPos.X - (PlayerOneSheet->GetClipSizeX() / 2), CurrentLevel->GetLevelMapSizeX(), PlayerOneSheet->GetClipSizeX(), true, true))
					{
						PlayerOneSheet->point->X = m_mPos.X - (PlayerOneSheet->GetClipSizeX() / 2);
					}

					if (!CheckIfWillBeOutOfBounds(m_mPos.Y - (PlayerOneSheet->GetClipSizeY() / 2), CurrentLevel->GetLevelMapSizeY(), PlayerOneSheet->GetClipSizeY(), true, false))
					{
						PlayerOneSheet->point->Y = m_mPos.Y - (PlayerOneSheet->GetClipSizeY() / 2);
					}
				}

				else if (m_input->MS()->GetButRDown()) {
					if (!CheckIfWillBeOutOfBounds(m_mPos.X - (PlayerTwoSheet->GetClipSizeX() / 2), CurrentLevel->GetLevelMapSizeX(), PlayerTwoSheet->GetClipSizeX(), true, true))
					{
						PlayerTwoSheet->point->X = m_mPos.X - (PlayerTwoSheet->GetClipSizeX() / 2);
					}
					if (!CheckIfWillBeOutOfBounds(m_mPos.Y - (PlayerTwoSheet->GetClipSizeY() / 2), CurrentLevel->GetLevelMapSizeY(), PlayerTwoSheet->GetClipSizeY(), true, false))
					{
						PlayerTwoSheet->point->Y = m_mPos.Y - (PlayerTwoSheet->GetClipSizeY() / 2);
					}
				}

				if ((m_input->CT()->ProcessMotionForLeftStick(m_sdlEvent))) {

				}
				if ((m_input->CT()->ProcessMotionForRightStick(m_sdlEvent))) {

				}
			}

			//Checking for deadzones on the controller.
			if (abs(m_input->CT()->LeftStickXValue) > 8000)
			{
				if (!CheckIfWillBeOutOfBounds(PlayerOneSheet->point->X + (m_input->CT()->LeftStickXValue / (32767) * 300 * t->GetDeltaTime()), CurrentLevel->GetLevelMapSizeX(), PlayerOneSheet->GetClipSizeX(), false, true))
				{
					PlayerOneSheet->point->X += m_input->CT()->LeftStickXValue / (32767) * 300 * t->GetDeltaTime();
				}
			}
			else if (abs(m_input->CT()->LeftStickXValue) < 2)
			{
				PlayerOneSheet->point->X = PlayerOneSheet->GetPointX();
			}
			else if (abs(m_input->CT()->LeftStickYValue) > 8000)
			{

				if (!CheckIfWillBeOutOfBounds(PlayerOneSheet->point->Y + (m_input->CT()->LeftStickYValue / (32767) * 300 * t->GetDeltaTime()), CurrentLevel->GetLevelMapSizeY(), PlayerOneSheet->GetClipSizeY(), false, false)) {
					{
						PlayerOneSheet->point->Y += m_input->CT()->LeftStickYValue / (32767) * 300 * t->GetDeltaTime();
					}
				}
			}
			else if (abs(m_input->CT()->LeftStickYValue) < 2)
			{
				PlayerOneSheet->point->Y = PlayerOneSheet->GetPointY();
			}

			if (abs(m_input->CT()->RightStickXValue) > 8000)
			{
				if (!CheckIfWillBeOutOfBounds(PlayerTwoSheet->point->X + (m_input->CT()->RightStickXValue / (32767) * 300 * t->GetDeltaTime()), CurrentLevel->GetLevelMapSizeX(), PlayerTwoSheet->GetClipSizeX(), false, true))
				{
					PlayerTwoSheet->point->X += m_input->CT()->RightStickXValue / (32767) * 300 * t->GetDeltaTime();
				}
			}
			else if (abs(m_input->CT()->RightStickXValue) < 2) {

				PlayerTwoSheet->point->X = PlayerTwoSheet->GetPointX();
			}
			else if (abs(m_input->CT()->RightStickYValue) > 8000)
			{
				if (!CheckIfWillBeOutOfBounds(PlayerTwoSheet->point->Y + (m_input->CT()->RightStickYValue / (32767) * 300 * t->GetDeltaTime()), CurrentLevel->GetLevelMapSizeY(), PlayerTwoSheet->GetClipSizeY(), false, false))
				{
					PlayerTwoSheet->point->Y += m_input->CT()->RightStickYValue / (32767) * 300 * t->GetDeltaTime();
				}
			}
			else if (abs(m_input->CT()->RightStickYValue) < 2) {

				PlayerTwoSheet->point->Y = PlayerTwoSheet->GetPointY();
			}

			if (GameState == 0) {
				m_renderer->RenderTexture(PlayerOneSheet, PlayerOneSheet->Update(EN_AN_RPS_IDLE, t->GetDeltaTime()), Rect(PlayerOneSheet->GetPointX(), PlayerOneSheet->GetPointY(), PlayerOneSheet->GetClipSizeX() + PlayerOneSheet->GetPointX(), PlayerOneSheet->GetClipSizeY() + PlayerOneSheet->GetPointY()));
				m_renderer->RenderTexture(PlayerTwoSheet, PlayerTwoSheet->Update(EN_AN_RPS_IDLE, t->GetDeltaTime()), Rect(PlayerTwoSheet->GetPointX(), PlayerTwoSheet->GetPointY(), (PlayerTwoSheet->GetClipSizeX()) + PlayerTwoSheet->GetPointX(), (PlayerTwoSheet->GetClipSizeY() * PlayerScale) + PlayerTwoSheet->GetPointY()));

			}
			else if (GameState == 1) {
				m_renderer->RenderTexture(PlayerOneSheet, PlayerOneSheet->Update(EN_AN_RPS_ROLL, t->GetDeltaTime()), Rect(PlayerOneSheet->GetPointX(), PlayerOneSheet->GetPointY(), PlayerOneSheet->GetClipSizeX() + PlayerOneSheet->GetPointX(), PlayerOneSheet->GetClipSizeY() + PlayerOneSheet->GetPointY()));
				m_renderer->RenderTexture(PlayerTwoSheet, PlayerTwoSheet->Update(EN_AN_RPS_ROLL, t->GetDeltaTime()), Rect(PlayerTwoSheet->GetPointX(), PlayerTwoSheet->GetPointY(), (PlayerTwoSheet->GetClipSizeX()) + PlayerTwoSheet->GetPointX(), (PlayerTwoSheet->GetClipSizeY() * PlayerScale) + PlayerTwoSheet->GetPointY()));

			}
			else if (GameState == 2)
			{
				//DO THE PLAYER ONE STUFF
				if (randomValue1 == 0) {
					m_renderer->RenderTexture(PlayerOneSheet, PlayerOneSheet->Update(EN_AN_RPS_ROCK, t->GetDeltaTime()), Rect(PlayerOneSheet->GetPointX(), PlayerOneSheet->GetPointY(), PlayerOneSheet->GetClipSizeX() + PlayerOneSheet->GetPointX(), PlayerOneSheet->GetClipSizeY() + PlayerOneSheet->GetPointY()));

				}
				else if (randomValue1 == 1) {
					m_renderer->RenderTexture(PlayerOneSheet, PlayerOneSheet->Update(EN_AN_RPS_PAPER, t->GetDeltaTime()), Rect(PlayerOneSheet->GetPointX(), PlayerOneSheet->GetPointY(), PlayerOneSheet->GetClipSizeX() + PlayerOneSheet->GetPointX(), PlayerOneSheet->GetClipSizeY() + PlayerOneSheet->GetPointY()));

				}
				else if (randomValue1 == 2) {
					m_renderer->RenderTexture(PlayerOneSheet, PlayerOneSheet->Update(EN_AN_RPS_SCISSORS, t->GetDeltaTime()), Rect(PlayerOneSheet->GetPointX(), PlayerOneSheet->GetPointY(), PlayerOneSheet->GetClipSizeX() + PlayerOneSheet->GetPointX(), PlayerOneSheet->GetClipSizeY() + PlayerOneSheet->GetPointY()));
				}

				//DO THE SECOND PLAYER STUFF
				if (randomValue2 == 0) {
					m_renderer->RenderTexture(PlayerTwoSheet, PlayerTwoSheet->Update(EN_AN_RPS_ROCK, t->GetDeltaTime()), Rect(PlayerTwoSheet->GetPointX(), PlayerTwoSheet->GetPointY(), (PlayerTwoSheet->GetClipSizeX()) + PlayerTwoSheet->GetPointX(), (PlayerTwoSheet->GetClipSizeY() * PlayerScale) + PlayerTwoSheet->GetPointY()));
				}
				else if (randomValue2 == 1) {
					m_renderer->RenderTexture(PlayerTwoSheet, PlayerTwoSheet->Update(EN_AN_RPS_PAPER, t->GetDeltaTime()), Rect(PlayerTwoSheet->GetPointX(), PlayerTwoSheet->GetPointY(), (PlayerTwoSheet->GetClipSizeX()) + PlayerTwoSheet->GetPointX(), (PlayerTwoSheet->GetClipSizeY() * PlayerScale) + PlayerTwoSheet->GetPointY()));

				}
				else if (randomValue2 == 2) {
					m_renderer->RenderTexture(PlayerTwoSheet, PlayerTwoSheet->Update(EN_AN_RPS_SCISSORS, t->GetDeltaTime()), Rect(PlayerTwoSheet->GetPointX(), PlayerTwoSheet->GetPointY(), (PlayerTwoSheet->GetClipSizeX()) + PlayerTwoSheet->GetPointX(), (PlayerTwoSheet->GetClipSizeY() * PlayerScale) + PlayerTwoSheet->GetPointY()));

				}

				if (!HasPickedWinner)
				{
					HasPickedWinner = true;

					if (randomValue1 == 0 && randomValue2 == 0 || randomValue1 == 1 && randomValue2 == 1 || randomValue1 == 2 && randomValue2 == 2) {
						PlayerOneWinState = 3;
						PlayerTwoWinState = 3;
					}
					else if (randomValue1 == 0 && randomValue2 == 1) {
						PlayerOneWinState = 2;
						PlayerTwoWinState = 1;
						PlayerTwoWinCount++;
						PlayerOneLossesCount++;
					}
					else if (randomValue1 == 0 && randomValue2 == 2) {
						PlayerOneWinState = 1;
						PlayerTwoWinState = 2;
						PlayerOneWinCount++;
						PlayerTwoLossesCount++;
					}
					else if (randomValue1 == 1 && randomValue2 == 0) {
						PlayerOneWinState = 1;
						PlayerTwoWinState = 2;
						PlayerOneWinCount++;
						PlayerTwoLossesCount++;

					}
					else if (randomValue1 == 1 && randomValue2 == 2) {
						PlayerOneWinState = 2;
						PlayerTwoWinState = 1;
						PlayerTwoWinCount++;
						PlayerOneLossesCount++;
					}
					else if (randomValue1 == 2 && randomValue2 == 1) {
						PlayerOneWinState = 1;
						PlayerTwoWinState = 2;
						PlayerOneWinCount++;
						PlayerTwoLossesCount++;

					}
					else if (randomValue1 == 2 && randomValue2 == 0) {
						PlayerOneWinState = 2;
						PlayerTwoWinState = 1;
						PlayerOneLossesCount++;
						PlayerTwoWinCount++;
					}
				}
			}

			GameTime += t->GetDeltaTime();
			LoadLevelTwoHud(GameTime, t->GetFPS(), SaveCount, LoadCount);
			PrintPlayerHud(m_text, PlayerOneSheet->GetPoint(), GameState, PlayerOneWinState, PlayerOneWinCount, PlayerOneLossesCount);
			PrintPlayerHud(m_text2, PlayerTwoSheet->GetPoint(), GameState, PlayerTwoWinState, PlayerTwoWinCount, PlayerTwoLossesCount);
		}
		SDL_RenderPresent(m_renderer->GetRenderer());
	}
}