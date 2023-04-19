#include "Level.h"

Level::Level() {
	Unit::Pool = new ObjectPool<Unit>();
	SoundEffect::Pool = new ObjectPool<SoundEffect>();

	m_mapSizeX = 0;
	m_gameTime = 0;
	m_mapSizeX = 0;
	Background_R_Value = 0;
	Background_G_Value = 0;
	Background_B_Value = 0;
	m_unit.clear();

}
Level::~Level() {
	m_unit.clear();
	delete SoundEffect::Pool;
	delete Unit::Pool;
	AssetController::Instance().Clear(); // free the 10mb
}

void Level::SetLevelNumber(int levelNumber) {
	Level::m_levelNumber = levelNumber;
}
void Level::SetMapSize(int x, int y) {
	Level::m_mapSizeX = x;
	Level::m_mapSizeY = y;

}
void Level::SetBackGroundColor(byte r, byte g, byte b) {
	Level::Background_R_Value = r;
	Level::Background_G_Value = g;
	Level::Background_B_Value = b;


}
void Level::SetGameTime(float GameTime) {
	Level::m_gameTime = GameTime;
}



float Level::GetLevelGameTime() {
	return Level::m_gameTime;
}

int Level::GetLevelMapSizeX() {
	return Level::m_mapSizeX;

}

int Level::GetLevelMapSizeY() {
	return Level::m_mapSizeY;
}


byte Level::GetLevelBackgroundColorR() {
	return Level::Background_R_Value;

}

int Level::GetLevelNumber() {
	return Level::m_levelNumber;
}

byte Level::GetLevelBackgroundColorG() {
	return Level::Background_G_Value;

}

byte Level::GetLevelBackgroundColorB() {
	return Level::Background_B_Value;

}
void Level::SetGameState(int GameStateValue)
{
	Gamestate = GameStateValue;
}
void Level::SetPlayerOneName(string name)
{
	PlayerOneName = name;
}
void Level::SetPlayerTwoName(string name)
{
	PlayerTwoName = name;
}
void Level::SetPlayerOneWins(int wins)
{
	PlayerOneWins = wins;
}
void Level::SetPlayerTwoWins(int wins)
{
	PlayerTwoWins = wins;
}
void Level::SetPlayerTwoLosses(int losses)
{
	PlayerTwoLosses = losses;
}
void Level::SetPlayerOneLosses(int losses)
{
	PlayerOneLosses = losses;
}
void Level::SetPlayerTwoWinState(int winState)
{
	PlayerTwoWinState = winState;
}

void Level::SetPlayerOneWinState(int winState)
{
	PlayerOneWinState = winState;
}
void Level::AssignNonDefaultValues() {
	m_mapSizeX = 128;
	m_mapSizeY = 256;
	m_gameTime = 101.5f;
	for (int count = 0; count < 5; count++) {
		Unit* unit = Unit::Pool->GetResources();
		unit->AssignNonDefaultValues();
		m_unit.push_back(unit);
	}
	Resource::AssignNonDefaultValues();

}

void Level::Serialize(std::ostream& _stream) {
	_stream.write(reinterpret_cast<char*> (&m_mapSizeY), sizeof(m_mapSizeY));
	_stream.write(reinterpret_cast<char*> (&m_mapSizeX), sizeof(m_mapSizeX));
	_stream.write(reinterpret_cast<char*> (&m_gameTime), sizeof(m_gameTime));
	_stream.write(reinterpret_cast<char*> (&m_levelNumber), sizeof(m_levelNumber));
	_stream.write(reinterpret_cast<char*> (&Background_B_Value), sizeof(Background_B_Value));
	_stream.write(reinterpret_cast<char*> (&Background_R_Value), sizeof(Background_R_Value));
	_stream.write(reinterpret_cast<char*> (&Background_G_Value), sizeof(Background_G_Value));
	
	_stream.write(reinterpret_cast<char*> (&Gamestate), sizeof(Gamestate));
	_stream.write(reinterpret_cast<char*> (&PlayerOneName), sizeof(PlayerOneName));
	_stream.write(reinterpret_cast<char*> (&PlayerOneWins), sizeof(PlayerOneWins));
	_stream.write(reinterpret_cast<char*> (&PlayerOneLosses), sizeof(PlayerOneLosses));
	_stream.write(reinterpret_cast<char*> (&PlayerOneWinState), sizeof(PlayerOneWinState));
	_stream.write(reinterpret_cast<char*> (&PlayerTwoName), sizeof(PlayerTwoName));
	_stream.write(reinterpret_cast<char*> (&PlayerTwoWins), sizeof(PlayerTwoWins));
	_stream.write(reinterpret_cast<char*> (&PlayerTwoLosses), sizeof(PlayerTwoLosses));
	_stream.write(reinterpret_cast<char*> (&PlayerTwoWinState), sizeof(PlayerTwoWinState));




	int numberOfUnits = m_unit.size();
	_stream.write(reinterpret_cast<char*>(&numberOfUnits), sizeof(numberOfUnits));
	for (int count = 0; count < numberOfUnits; count++) {
		SerializePointer(_stream, m_unit[count]);
	}

	Resource::Serialize(_stream);
}

void Level::DeSerialize(std::istream& _stream) {
	_stream.read(reinterpret_cast<char*>(&m_mapSizeX), sizeof(m_mapSizeX));
	_stream.read(reinterpret_cast<char*>(&m_mapSizeY), sizeof(m_mapSizeY));
	_stream.read(reinterpret_cast<char*>(&m_gameTime), sizeof(m_gameTime));
	_stream.read(reinterpret_cast<char*> (&m_levelNumber), sizeof(m_levelNumber));
	_stream.read(reinterpret_cast<char*> (&Background_B_Value), sizeof(Background_B_Value));
	_stream.read(reinterpret_cast<char*> (&Background_R_Value), sizeof(Background_R_Value));
	_stream.read(reinterpret_cast<char*> (&Background_G_Value), sizeof(Background_G_Value));


	_stream.read(reinterpret_cast<char*> (&Gamestate), sizeof(Gamestate));
	_stream.read(reinterpret_cast<char*> (&PlayerOneName), sizeof(PlayerOneName));
	_stream.read(reinterpret_cast<char*> (&PlayerOneWins), sizeof(PlayerOneWins));
	_stream.read(reinterpret_cast<char*> (&PlayerOneLosses), sizeof(PlayerOneLosses));
	_stream.read(reinterpret_cast<char*> (&PlayerOneWinState), sizeof(PlayerOneWinState));
	_stream.read(reinterpret_cast<char*> (&PlayerTwoName), sizeof(PlayerTwoName));
	_stream.read(reinterpret_cast<char*> (&PlayerTwoWins), sizeof(PlayerTwoWins));
	_stream.read(reinterpret_cast<char*> (&PlayerTwoLosses), sizeof(PlayerTwoLosses));
	_stream.read(reinterpret_cast<char*> (&PlayerTwoWinState), sizeof(PlayerTwoWinState));

	int numberOfUnits;
	_stream.read(reinterpret_cast<char*>(&numberOfUnits), sizeof(numberOfUnits));

	for (int count = 0; count < numberOfUnits; count++) {
		Unit* unit;
		DeserializePointer(_stream, unit);
		m_unit.push_back(unit);
	}

	Resource::DeSerialize(_stream);
}

void Level::ToString() {
	cout << "LEVEL" << endl;
	cout << "MapsizeX" << m_mapSizeX << endl;
	cout << "MapSizeY" << m_mapSizeY << endl;
	cout << "GameTime" << m_gameTime << endl;

	for (int count = 0; count < m_unit.size(); count++) {
		m_unit[count]->ToString();
	}

	Resource::ToString();
}