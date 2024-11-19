#include "TextureConverter.h"
#include <dxgidebug.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath) {
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);
}


void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	// ファイルパスをワイド文字列に変換する
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	// WICテクスチャのロード
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(hr));
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
	// 文字列が空なら終了
	if (mString.empty()) { return std::wstring(); }

	// ワイド文字列に変換した際の文字数を計算
	auto sizeNeeded = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);
	// 文字列の長さが0ら終了
	if (sizeNeeded == 0) { return std::wstring(); }
	
	// 結果を生成
	std::wstring result;
	result.resize(sizeNeeded);
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &result[0], sizeNeeded);

	return result;
}
