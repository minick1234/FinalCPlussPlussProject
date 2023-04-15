#ifndef SPRITEANIM_H
#define SPRITEANIM_H

#include "Resource.h"

class SpriteAnim : Resource {
public:

	SpriteAnim();
	virtual ~SpriteAnim();

	short GetClipCurrent() { return (short)m_clipCurrent; }

	virtual void Serialize(std::ostream& _stream);
	virtual void DeSerialize(std::istream& _stream);
	virtual void ToString();

	void ClearMemory();
	void Create(short _clipStart, short _clipCount, float _clipSpeed);
	void Update(float _deltaTime);

	void ChangeClipSpeed(float newClipSpeed) { m_clipSpeed = newClipSpeed; }
	bool IsAnimationFinished();
	float GetClipSpeed() {
		return m_clipSpeed;
	}


	static ObjectPool<SpriteAnim>* Pool;

private:
	short m_clipStart;
	short m_clipCount;
	short m_clipEnd;
	float m_clipCurrent;
	float m_clipSpeed;
};



#endif // !SPRITEANIM_H
