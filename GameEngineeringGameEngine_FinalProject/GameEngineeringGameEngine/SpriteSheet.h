#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Texture.h"
#include "SpriteAnim.h"
#include "BasicStructs.h"

enum AnimationNames {
	EN_AN_IDLE = 0,
	EN_AN_RUN,
	EN_AN_TWO_COMBO_ATTACK,
	EN_AN_DEATH,
	EN_AN_HURT,
	EN_AN_JUMP_UP_FALL,
	EN_AN_EDGE_GRAB,
	EN_AN_EDGE_IDLE,
	EN_AN_WALL_SIDE,
	EN_AN_CROUCH,
	EN_AN_DASH,
	EN_AN_DASH_ATTACH,
	EN_AN_SLIDE,
	EN_AN_LADDER_GRAB,
	EN_AN_RPS_ROCK,
	EN_AN_RPS_PAPER,
	EN_AN_RPS_SCISSORS,
	EN_AN_RPS_IDLE,
	EN_AN_RPS_ROLL
};


class SpriteSheet : public Texture {
public:
	SpriteSheet();
	~SpriteSheet();

	void Serialize(std::ostream& _stream)override;
	void DeSerialize(std::istream& _stream)override;
	void ToString() override;

	void SetSize(int _rows, int _columns, int _clipSizeX, int _clipSizeY);
	void AddAnimation(AnimationNames _name, short clipStart, short _clipCount, float _clipSpeed);
	Rect Update(AnimationNames _name, float _deltaTime);
	int GetCurrentClip(AnimationNames _name);

	bool IsAnimationFinished(AnimationNames _name);

	Point* point;
	void SetPoint(int x, int y) { point->X = x; point->Y = y; }
	Point GetPoint() { return *point; }
	float GetPointX() { return point->X; }
	float GetPointY() { return point->Y; }
	void SetPointX(int x) { point->X = x; }
	void SetPointY(int y) { point->Y = y; }

	void UpdateAnimationSheetsClipSpeed(AnimationNames _name, float newClipSpeed);
	float AnimationSheetsAnimationClipSpeed(AnimationNames _name) { return m_animations[_name]->GetClipSpeed(); }
	int GetClipSizeX() { return m_clipSizeX; }
	int GetClipSizeY() { return m_clipSizeY; }

	static ObjectPool<SpriteSheet>* Pool;

private:
	int m_rows;
	int m_columns;
	int m_clipSizeX;
	int m_clipSizeY;
	map<AnimationNames, SpriteAnim*> m_animations;
};

#endif // !SPRITESHEET_H
