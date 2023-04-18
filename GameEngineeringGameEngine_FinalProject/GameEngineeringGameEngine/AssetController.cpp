#include "AssetController.h"
#include "FileController.h"

StackAllocator* AssetController::Stack = nullptr;

AssetController::AssetController() {

}

AssetController::~AssetController() {
	Clear();
}

void AssetController::Clear() {
	for (auto const& x : m_assets) {
		Asset::Pool->ReleaseResource(x.second);
	}

	if (Asset::Pool != nullptr) {
		delete Asset::Pool;
		Asset::Pool = nullptr;
	}

	Stack->ClearMemory();
	m_assets.clear();
}

void AssetController::Initialize(int _stackSize) {
	Stack = new StackAllocator();
	AssetController::Stack->AllocateStack(_stackSize);
	Asset::Pool = new ObjectPool<Asset>();

}

Asset* AssetController::GetAsset(string _guid) {
	if (m_assets.count(_guid) != 0) {
		return m_assets[_guid];
	}

	Asset* asset = Asset::Pool->GetResources();
	asset->SetGUID(_guid);
	asset->SetDataSize(FileController::Instance().GetFileSize(_guid));
	asset->SetData(Stack->GetMemory(asset->GetDataSize()));
	FileController::Instance().ReadFile(_guid, asset->GetData(), asset->GetDataSize());

	m_assets[_guid] = asset;
	cout << Stack->GetBytesUsed() << endl;
	return asset;

}