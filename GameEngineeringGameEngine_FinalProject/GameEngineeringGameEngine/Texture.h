#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource.h"
#include "TGAReader.h"


class Asset;

class Texture : public Resource {
public :
	Texture();
	virtual ~Texture();
	
	Asset* GetData() { return m_texture; }
	ImageInfo* GetImageInfo() { return &m_imageInfo; }

	void Serialize(std::ostream& _stream)override;
	void DeSerialize(std::istream& _stream) override;
	void ToString()override;
	void Load(string _guid);

	static ObjectPool<Texture>* Pool;

private :
	ImageInfo m_imageInfo;
	Asset* m_texture;
};


#endif // !TEXTURE_H
