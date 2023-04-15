#ifndef SOUND_EFFECT_H
#define SOUND_EFFECT_H

#include "Resource.h"
#include "Asset.h"


class SoundEffect : public Resource {

public:
	SoundEffect();
	virtual ~SoundEffect();


	void Serialize(std::ostream& _stream) override;
	void DeSerialize(std::istream& _stream) override;
	void ToString() override;
	void AssignNonDefaultValues() override;

	static ObjectPool<SoundEffect>* Pool;


private:
	Asset* m_effect;

};






#endif // !SOUND_EFFECT_H
