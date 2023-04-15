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

GameController::GameController() {
	m_sdlEvent = {};
	m_renderer = nullptr;
	m_fArial20 = nullptr;
	m_quit = false;
	m_input = nullptr;
	m_audio = nullptr;
	memset(m_effects, 0, sizeof(SoundEffect*) * MaxEffectsPerChannels);
	m_song = nullptr;
	m_text = "";
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
}

void GameController::Initialize() {
	AssetController::Instance().Initialize(10000000);
	m_renderer = &Renderer::Instance();
	m_renderer->Initialize(1920, 1080);
	m_input = &InputController::Instance();
	m_fArial20 = new TTFont();
	m_fArial20->Initialize(20);
	m_audio = &AudioController::Instance();

	m_wavDraw = new WavDraw();


	m_effects[0] = m_audio->LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
	m_effects[1] = m_audio->LoadEffect("../Assets/Audio/Effects/BeeFlyingLoop.mp3");
	m_effects[2] = m_audio->LoadEffect("../Assets/Audio/Effects/DistantGunshot.mp3");
	m_effects[3] = m_audio->LoadEffect("../Assets/Audio/Effects/DrinkSipSwallow.mp3");

	m_song = m_audio->LoadSong("../Assets/Audio/Music/Track1.mp3");
}


void GameController::HandleInput(SDL_Event event) {
	string temp;

	if (m_sdlEvent.type == SDL_QUIT || m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)) {
		m_quit = true;
	}
	else if ((temp = m_input->KB()->TextInput(event)) != "") {
		m_text += temp;
	}
	//if backspace is pressed we want to remove from the text input buffer.
	else if (m_input->KB()->KeyDown(m_sdlEvent, SDLK_BACKSPACE)) {
		m_text.erase(m_text.length() - 1);
	}

	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_RETURN)) {
		m_text = "";
	}
	else if (m_input->MS()->Moved(m_sdlEvent, m_mPos)) {
		m_smPos = "Mouse Position [" + to_string(m_mPos.X) + ";" + to_string(m_mPos.Y) + "]";
	}
	else if ((m_input->CT()->Added(m_sdlEvent)) || (m_input->CT()->Removed(m_sdlEvent)) || (m_input->CT()->ProcessButtons(m_sdlEvent)) || (m_input->CT()->ProcessMotion(m_sdlEvent))) {
		m_ctInfo = m_input->CT()->ToString();
	}
	else if (m_input->KB()->KeyUp(event, SDLK_p)) {
		m_audio->Play(m_effects[rand() % 4]);

	}
	else if (m_input->KB()->KeyUp(event, SDLK_a)) {
		m_audio->Play(m_song);

	}
	else if (m_input->KB()->KeyUp(event, SDLK_s)) {
		m_audio->PauseMusic();

	}
	else if (m_input->KB()->KeyUp(event, SDLK_d)) {
		m_audio->ResumeMusic();

	}
	else if (m_input->KB()->KeyUp(event, SDLK_f)) {
		m_audio->StopMusic();
	}
	else if (m_input->KB()->KeyUp(event, SDLK_x)) {
		m_zoomY += 0.5f;
	}
	else if (m_input->KB()->KeyUp(event, SDLK_z)) {
		m_zoomY -= 0.5f;
	}
	else {
		m_input->MS()->ProcessButtons(event);
	}


}

void GameController::RunGame() {
	Initialize();

	while (!m_quit) {
		m_renderer->SetDrawColour(Color(255, 255, 255, 255));
		m_renderer->ClearScreen();

		while (SDL_PollEvent(&m_sdlEvent) != 0) {
			HandleInput(m_sdlEvent);

		}
		
		string song = "Current Song: " + m_audio->GetMusicTitle();
		if (m_audio->GetMusicLength() != "") {
			song += " " + to_string((int)m_audio->MusicPosition()) + "/" + m_audio->GetMusicLength();
		}
		m_fArial20->Write(m_renderer->GetRenderer(), song.c_str(), SDL_Color{0,0,255}, SDL_Point{10,10});
	
		for (int count = 0; count < MaxEffectsPerChannels; count++) {
			string eff = "Effect: " + to_string(count) + ": ";
			eff += m_audio->GetCurrentEffects()[count];
			m_fArial20->Write(m_renderer->GetRenderer(), eff.c_str(), SDL_Color{ 255,0,255 }, SDL_Point{10, 30 + (count * 20)});
		}



		m_wavDraw->DrawWave(m_effects[0]->GetData(), m_renderer, m_zoomY);
		m_fArial20->Write(m_renderer->GetRenderer(), ("Left: " + to_string(m_input->MS()->GetButLDown())).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 250,240 });
		m_fArial20->Write(m_renderer->GetRenderer(), ("Middle: " + to_string(m_input->MS()->GetButMDown())).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 250,260 });
		m_fArial20->Write(m_renderer->GetRenderer(), ("Right: " + to_string(m_input->MS()->GetButRDown())).c_str(), SDL_Color{ 0,0,255 }, SDL_Point{ 250,280 });
		SDL_RenderPresent(m_renderer->GetRenderer());
	}
}
