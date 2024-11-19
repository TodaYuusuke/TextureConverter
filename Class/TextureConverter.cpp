#include "TextureConverter.h"
#include <dxgidebug.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath) {
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);

	// DDS変換
	SaveDDSTextureToFile();
}


void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	// ファイルパスをワイド文字列に変換する
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	// WICテクスチャのロード
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(hr));

	// フォルダパスとファイル名を分離する
	SeparateFilePath(wFilePath);
}
void TextureConverter::SeparateFilePath(const std::wstring& filePath) {
	size_t pos1;
	std::wstring exceptExt;

	// 区切り文字 '.' が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		// 区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}

	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}
	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字がないのでファイル名のみとして扱う
	directoryPath_ = L"";
	fileName_ = exceptExt;
}
void TextureConverter::SaveDDSTextureToFile() {
	HRESULT hr;
	
	// ミップマップ生成
	DirectX::ScratchImage mipChain;
	hr = DirectX::GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain
	);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャをSRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);
	// 出力ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDSファイル書き出し
	hr = DirectX::SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
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
