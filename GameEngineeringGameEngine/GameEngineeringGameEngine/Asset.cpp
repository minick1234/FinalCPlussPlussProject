#include "Asset.h"

ObjectPool<Asset>* Asset::Pool;

Asset::Asset() {
	m_GUID = "";
	m_dataSize = 0;
	m_data = nullptr;
}

void Asset::ToString() {
	cout << "Asset GUID: " << m_GUID << endl;
	cout << "Assets data size: " << m_dataSize << endl;

}
