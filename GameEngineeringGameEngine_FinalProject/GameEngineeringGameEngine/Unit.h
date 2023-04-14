#ifndef UNIT_H
#define UNIT_H

#include "Resource.h"
#include "SoundEffect.h"
#include "SpriteAnim.h"
#include "SpriteSheet.h"


class Unit : public Resource {
public :
	Unit();
	virtual ~Unit();

	void Serialize(std::ostream& _stream) override;
	void DeSerialize(std::istream& _stream) override;
	void ToString() override;
	void AssignNonDefaultValues() override;

	static ObjectPool<Unit>* Pool;

private : 
	SoundEffect* m_soundEffect;
};

#endif //UNIT_H
