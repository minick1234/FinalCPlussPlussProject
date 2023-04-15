#include "GameController.h"
#include "StandardIncludes.h"
#include "Level.h"

GameController::GameController() {
	m_sdlEvent = {};
}
GameController::~GameController() {

}

bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
	if (a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h) {
		return false;
	}
	return true;
}

void GameController::RunGame() {
	AssetController::Instance().Initialize(10000000); //allocate 10mb

	Level* CurrentLevel = new Level();

	float maxClipSpeed = 6.0f;
	float minClipSpeed = 4.7f;
	int OffsetY = 100;
	int OffsetXForRocks = 100;

	Renderer* r = &Renderer::Instance();
	Timing* t = &Timing::Instance();
	r->Initialize(1920, 1080);

	TTFont* font = new TTFont();
	font->Initialize(20);

	CurrentLevel->SetGameTime(t->GetDeltaTime());
	CurrentLevel->SetLevelNumber(1);
	CurrentLevel->SetBackGroundColor(128, 128, 128);
	CurrentLevel->SetMapSize(1920, 1080);

	Point ws = r->GetWindowSize();

	SpriteSheet::Pool = new ObjectPool<SpriteSheet>();
	SpriteAnim::Pool = new ObjectPool<SpriteAnim>();

	SpriteSheet* Warriorsheets[10];
	SpriteSheet* RockSheets[10];

	for (int i = 0; i < sizeof(Warriorsheets) / sizeof(Warriorsheets[0]); i++) {
		Warriorsheets[i] = SpriteSheet::Pool->GetResources();
		Warriorsheets[i]->Load("../Assets/Textures/Warrior.tga");
		Warriorsheets[i]->SetSize(17, 6, 69, 44);
		Warriorsheets[i]->AddAnimation(EN_AN_IDLE, 0, 6, maxClipSpeed);
		Warriorsheets[i]->AddAnimation(EN_AN_RUN, 6, 8, maxClipSpeed);
		Warriorsheets[i]->AddAnimation(EN_AN_DEATH, 27, 10, maxClipSpeed);
		Warriorsheets[i]->point = new Point(0, 10 + (OffsetY * i));
	}

	for (int i = 0; i < sizeof(RockSheets) / sizeof(RockSheets[0]); i++) {
		RockSheets[i] = SpriteSheet::Pool->GetResources();
		RockSheets[i]->Load("../Assets/Textures/Rock.tga");
		RockSheets[i]->SetSize(1, 4, 20, 20);
		RockSheets[i]->AddAnimation(EN_ROCK_FALLING, 0, 4, maxClipSpeed);
		RockSheets[i]->point = new Point(50 + (OffsetXForRocks * i), 0);
	}

	//Assign the speed variable and do the first random Speed value
	float XVelocity[sizeof(Warriorsheets) / sizeof(Warriorsheets[0])] = { 0.00f };
	float rockYVelocity[sizeof(RockSheets) / sizeof(RockSheets[0])] = { 0.00f };

	float speeds[sizeof(Warriorsheets) / sizeof(Warriorsheets[0])];
	float rockSpeeds[sizeof(RockSheets) / sizeof(RockSheets[0])];

	float clipSpeeds[sizeof(Warriorsheets) / sizeof(Warriorsheets[0])] = { 4.7 };
	float rockclipSpeeds[sizeof(RockSheets) / sizeof(RockSheets[0])] = { 4.7 };


	float accelerationTime = 2;
	float lerpTime = accelerationTime;

	float warriorScale = 1.8f;
	float rockScale = 1.0f;

	bool firstTime = false;
	bool DoneOnceTilLevelLoad = false;
	float GameTimeInSeconds = 0.0f;
	bool Level1AutoSaved = false;

	bool WarriorCollided[10] = { false };
	bool RockCollided[10] = { false };
	bool DeathAnimationFinished[10] = { false };

	srand(time(NULL));

	for (int i = 0; i < sizeof(Warriorsheets) / sizeof(Warriorsheets[0]); i++) {
		speeds[i] = rand() % (100 - 80 + 1) + 80;
		cout << speeds[i] << endl;
	}

	for (int i = 0; i < sizeof(RockSheets) / sizeof(RockSheets[0]); i++) {
		rockSpeeds[i] = rand() % (100 - 80 + 1) + 80;
		cout << rockSpeeds[i] << endl;
	}

	while (m_sdlEvent.type != SDL_QUIT) {
		t->Tick();
		SDL_PollEvent(&m_sdlEvent);
		r->SetDrawColour(Color(CurrentLevel->GetLevelBackgroundColorR(), CurrentLevel->GetLevelBackgroundColorG(), CurrentLevel->GetLevelBackgroundColorB(), 255));
		r->ClearScreen();


		if (GameTimeInSeconds >= 5 && !DoneOnceTilLevelLoad) {

			if (!Level1AutoSaved) {

				//save the game here.
				ofstream writeStream("level1.bin", ios::out | ios::binary);
				CurrentLevel->Serialize(writeStream);
				writeStream.close();

				//now that we saved instantly also load the saved file.
				ifstream readStream("level1.bin", ios::in | ios::binary);
				CurrentLevel->DeSerialize(readStream);
				readStream.close();
				Level1AutoSaved = true;
			}
			else {
				//save the game here.
				ofstream writeStream("level2.bin", ios::out | ios::binary);
				CurrentLevel->Serialize(writeStream);
				writeStream.close();

				//now that we saved instantly also load the saved file.
				ifstream readStream("level2.bin", ios::in | ios::binary);
				CurrentLevel->DeSerialize(readStream);
				readStream.close();

			}

			DoneOnceTilLevelLoad = true;
		}

		for (int i = 0; i < sizeof(Warriorsheets) / sizeof(Warriorsheets[0]); i++) {

			float speedDiff = speeds[i] - XVelocity[i];
			float acceleration = speedDiff / lerpTime;
			XVelocity[i] += acceleration * t->GetDeltaTime();
			XVelocity[i] = min(XVelocity[i], speeds[i]);
			float speedLerpAmount = XVelocity[i] / speeds[i];
			clipSpeeds[i] = (minClipSpeed + (maxClipSpeed - minClipSpeed) * speedLerpAmount);
			Warriorsheets[i]->UpdateAnimationSheetsClipSpeed(EN_AN_RUN, clipSpeeds[i]);

			if (CurrentLevel->GetLevelNumber() == 2) {
				float RockspeedDiff = rockSpeeds[i] - rockYVelocity[i];
				float Rockacceleration = RockspeedDiff / lerpTime;
				rockYVelocity[i] += Rockacceleration * t->GetDeltaTime();
				rockYVelocity[i] = min(rockYVelocity[i], rockSpeeds[i]);
				float rockSpeedLerpAmount = rockYVelocity[i] / rockSpeeds[i];
				rockclipSpeeds[i] = (minClipSpeed + (maxClipSpeed - minClipSpeed) * rockSpeedLerpAmount);
				RockSheets[i]->UpdateAnimationSheetsClipSpeed(EN_ROCK_FALLING, rockclipSpeeds[i]);
			}

			//Check if the current warrior has gone outside of the screen
			if (Warriorsheets[i]->GetPointX() + ((Warriorsheets[i]->GetClipSizeX() * warriorScale) / 2) >= 1920) {

				//Check if this is the first or second time we have done this:
				if (CurrentLevel->GetLevelNumber() == 2) {
					//If this is the second time we want to close the gameengine.
					exit(0);
				}
				else {
					//Reset the position of the players.
					for (int j = 0; j < sizeof(Warriorsheets) / sizeof(Warriorsheets[0]); j++) {
						Warriorsheets[j]->SetPoint(0, Warriorsheets[j]->GetPointY());
					}

					ifstream readStream("level2.bin", ios::in | ios::binary);
					CurrentLevel->DeSerialize(readStream);
					readStream.close();

					cout << "The new current levels level number should be 2 but is : " << CurrentLevel->GetLevelNumber() << endl;

					if (CurrentLevel->GetLevelNumber() == 0 || CurrentLevel->GetLevelNumber() == -1) {
						//if we are here the second level doesnt exist therefore we make the changes to current level data and save it all;
						cout << "level 2 doesnt exist so here we are.";
						CurrentLevel->SetGameTime(t->GetDeltaTime());
						CurrentLevel->SetLevelNumber(2);
						CurrentLevel->SetBackGroundColor(0, 128, 0);
						CurrentLevel->SetMapSize(1920, 1080);

						ofstream writeStream("level2.bin", ios::out | ios::binary);
						CurrentLevel->Serialize(writeStream);
						writeStream.close();
						CurrentLevel->Serialize(writeStream);
						CurrentLevel->ToString();
						cout << endl;

					}
					DoneOnceTilLevelLoad = false;
				}
			}

			Warriorsheets[i]->point->X += XVelocity[i] * t->GetDeltaTime();
			RockSheets[i]->point->Y += rockYVelocity[i] * t->GetDeltaTime();

			if (CurrentLevel->GetLevelNumber() == 2) {
				for (int j = 0; j < sizeof(RockSheets) / sizeof(RockSheets[0]); j++) {

					SDL_Rect warriorRect = { Warriorsheets[i]->GetPointX(), Warriorsheets[i]->GetPointY(), static_cast<int>(Warriorsheets[i]->GetClipSizeX() * warriorScale), static_cast<int>(Warriorsheets[i]->GetClipSizeY() * warriorScale) };
					SDL_Rect rockRect = { RockSheets[j]->GetPointX(), RockSheets[j]->GetPointY(), static_cast<int>(RockSheets[j]->GetClipSizeX() * rockScale), static_cast<int>(RockSheets[j]->GetClipSizeY() * rockScale) };

					cout << CheckCollision(warriorRect, rockRect) << endl;
					if (CheckCollision(warriorRect, rockRect))
					{
						cout << "The rocks collision value is :" << RockCollided[j];
						cout << "The warriors collision value is :" << WarriorCollided[i] << endl;
						WarriorCollided[i] = true;
						RockCollided[j] = true;
						break;
					}
				}
			}


			if (!firstTime) {
				r->RenderTexture(Warriorsheets[i], Warriorsheets[i]->Update(EN_AN_IDLE, t->GetDeltaTime()), Rect(0, Warriorsheets[i]->GetPointY(), (Warriorsheets[i]->GetClipSizeX() * warriorScale) + Warriorsheets[i]->GetPointX(), (Warriorsheets[i]->GetClipSizeY() * warriorScale) + Warriorsheets[i]->GetPointY()));

				if (CurrentLevel->GetLevelNumber() == 2)
				{
					r->RenderTexture(RockSheets[i], RockSheets[i]->Update(EN_ROCK_FALLING, t->GetDeltaTime()), Rect(RockSheets[i]->GetPointX(), RockSheets[i]->GetPointY(), (RockSheets[i]->GetClipSizeX() * rockScale) + RockSheets[i]->GetPointX(), (RockSheets[i]->GetClipSizeY() * rockScale) + RockSheets[i]->GetPointY()));
				}
				firstTime = true;
			}
			else
			{
				if (!WarriorCollided[i]) {
					r->RenderTexture(Warriorsheets[i], Warriorsheets[i]->Update(EN_AN_RUN, t->GetDeltaTime()), Rect(Warriorsheets[i]->GetPointX(), Warriorsheets[i]->GetPointY(), (Warriorsheets[i]->GetClipSizeX() * warriorScale) + Warriorsheets[i]->GetPointX(), (Warriorsheets[i]->GetClipSizeY() * warriorScale) + Warriorsheets[i]->GetPointY()));
				}
				else {
					if (!DeathAnimationFinished[i]) {
						r->RenderTexture(Warriorsheets[i], Warriorsheets[i]->Update(EN_AN_DEATH, t->GetDeltaTime()), Rect(Warriorsheets[i]->GetPointX(), Warriorsheets[i]->GetPointY(), (Warriorsheets[i]->GetClipSizeX() * warriorScale) + Warriorsheets[i]->GetPointX(), (Warriorsheets[i]->GetClipSizeY() * warriorScale) + Warriorsheets[i]->GetPointY()));


						if (Warriorsheets[i]->IsAnimationFinished(EN_AN_DEATH)) {
							DeathAnimationFinished[i] = true;
						}
					}

				}
				if (CurrentLevel->GetLevelNumber() == 2)
				{
					if (!RockCollided[i]) {
						r->RenderTexture(RockSheets[i], RockSheets[i]->Update(EN_ROCK_FALLING, t->GetDeltaTime()), Rect(RockSheets[i]->GetPointX(), RockSheets[i]->GetPointY(), (RockSheets[i]->GetClipSizeX() * rockScale) + RockSheets[i]->GetPointX(), (RockSheets[i]->GetClipSizeY() * rockScale) + RockSheets[i]->GetPointY()));
					}
				}
			}
		}

		GameTimeInSeconds += t->GetDeltaTime();
		DrawTopHud(GameTimeInSeconds, Level1AutoSaved, t, r, font);
		SDL_RenderPresent(r->GetRenderer());
	}

	delete SpriteAnim::Pool;
	delete SpriteSheet::Pool;
	font->Shutdown();
	r->Shutdown();
}

void GameController::DrawTopHud(int GameTime, bool autoSaved, Timing* t, Renderer* r, TTFont* font) {
	std::string FPS = "Frames Per Second: " + std::to_string(t->GetFPS());
	font->Write(r->GetRenderer(), FPS.c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 0, 0 });

	std::string GAMETIME = "Game Time: " + std::to_string(GameTime);
	font->Write(r->GetRenderer(), GAMETIME.c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 300, 0 });

	std::string AUTO_SAVED = "";
	if (!autoSaved) {
		AUTO_SAVED = "Auto Saved: No";

	}
	else {
		AUTO_SAVED = "Auto Saved: Yes";
	}

	font->Write(r->GetRenderer(), AUTO_SAVED.c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 500, 0 });
}
