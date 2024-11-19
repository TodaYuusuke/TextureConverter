#include "TextureConverter.h"
#include <dxgidebug.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath) {
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);
}


void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	// ファイルパスをワイド文字列に変換する
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);


}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
	if (mString.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&mString[0]), static_cast<int>(mString.size()), &result[0], sizeNeeded);

	return result;
}
