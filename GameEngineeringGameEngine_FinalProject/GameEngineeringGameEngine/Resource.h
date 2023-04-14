#ifndef RESOURCE_H
#define RESOURCE_H

#include "Serializeable.h"
#include "AssetController.h"

class Resource : public Serializable {
public:
	Resource();;
	virtual ~Resource();

	virtual void Serialize(std::ostream& _stream);
	virtual void DeSerialize(std::istream& _stream);
	virtual void AssignNonDefaultValues();
	virtual void ToString();

	static ObjectPool<Resource>* Pool;


protected:

	template <class T>
	void SerializePointer(std::ostream& _stream, T*& _pointer) {
		byte exists = 1;
		if (_pointer != nullptr) {

			_stream.write(reinterpret_cast<char*>(&exists), sizeof(byte));
			_pointer->Serialize(_stream);
		}
		else {
			exists = 0;
			_stream.write(reinterpret_cast<char*>(&exists), sizeof(byte));
		}
	}

	template <class T>
	void DeserializePointer(std::istream& _stream, T*& _pointer) {
		byte exists = 0;
		_stream.read(reinterpret_cast<char*>(&exists), sizeof(exists));

		if (exists == 1) {
			_pointer = T::Pool->GetResources();
			_pointer->DeSerialize(_stream);
		}
	}

	void DeserializeAsset(std::istream& _stream, Asset*& asset);
	void SerializeAsset(std::ostream& _stream, Asset* asset);

private:
};


#endif // !RESOURCE_H
