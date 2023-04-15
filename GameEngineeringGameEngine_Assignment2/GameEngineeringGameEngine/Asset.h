#ifndef ASSET_H
#define ASSET_H

#include "StandardIncludes.h"
#include "ObjectPool.h"

class Asset {
public:
	Asset();
	virtual ~Asset() {}

	string GetGUID() { return m_GUID; }
	int GetDataSize() { return m_dataSize; }
	void SetGUID(string _guid) { m_GUID = _guid; }
	void SetDataSize(int _size) { m_dataSize = _size; }
	byte* GetData() { return m_data; }
	void SetData(byte* _data) { m_data = _data; }
	void Reset() {
		m_dataSize = 0;
		m_data = nullptr;
		m_GUID = "";
	};


	static ObjectPool<Asset>* Pool;
	void ToString();

private:
	string m_GUID;
	int m_dataSize;
	byte* m_data;

};


#endif // !ASSET_H
