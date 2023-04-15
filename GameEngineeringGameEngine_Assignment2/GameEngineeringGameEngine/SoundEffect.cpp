#include "SoundEffect.h"

ObjectPool<SoundEffect>* SoundEffect::Pool = nullptr;

SoundEffect::SoundEffect() {
	m_effect = nullptr;
}



SoundEffect::~SoundEffect() {

}

void SoundEffect::AssignNonDefaultValues() {
	m_effect = AssetController::Instance().GetAsset("SoundEffect.wav");
}


void SoundEffect::Serialize(std::ostream& _stream) {
	SerializeAsset(_stream, m_effect);
}

void SoundEffect::DeSerialize(std::istream& _stream) {
	DeserializeAsset(_stream, m_effect);
}

void SoundEffect::ToString() {
	cout << "SOUND EFFECT: " << endl;
	m_effect->ToString();
	Resource::ToString();
}
