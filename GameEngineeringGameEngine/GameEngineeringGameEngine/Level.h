#ifndef LEVEL_H
#define LEVEL_H

#include "Unit.h"

class Level : public Resource {

public : 
	Level();
	~Level();

	void Serialize(std::ostream& _stream) override;
	void DeSerialize(std::istream& _stream) override;
	void AssignNonDefaultValues() override;
	void ToString() override;

	void SetLevelNumber(int levelNumber);
	void SetMapSize(int x, int y);
	void SetBackGroundColor(byte r, byte g, byte b);
	void SetGameTime(float GameTime);
	float GetLevelGameTime();
	int GetLevelMapSizeX();
	int GetLevelMapSizeY();
	byte GetLevelBackgroundColorR();
	int GetLevelNumber();
	byte GetLevelBackgroundColorG();
	byte GetLevelBackgroundColorB();



private :
	//Main Level params
	int m_levelNumber = -1;
	int m_mapSizeX;
	int m_mapSizeY;
	
	//BackgroundColorValues 
	byte Background_R_Value;
	byte Background_G_Value;
	byte Background_B_Value;

	//Other params
	float m_gameTime;
	vector<Unit*> m_unit;
};


#endif // !LEVEL_H
