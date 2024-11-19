#include "TextureConverter.h"
#include <dxgidebug.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath, int numOptions, char* options[]) {
	// テクスチャファイルを読み込む
	LoadWICTextureFromFile(filePath);
	// DDS変換
	SaveDDSTextureToFile(numOptions, options);
}

void TextureConverter::OutputUsage() {
	printf("画像ファイルをWIC形式からDDS形式に変換します。\n");
	printf("\n");
	printf("TextureConverter [ドライブ:][パス]ファイル名 [-ml level]\n");
	printf("\n");
	printf("[ドライブ:][パス]ファイル名: 変換したいWIC形式の画像ファイルを指定します。\n");
	printf("[-ml level]: ミップレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します。\n");
}


void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	printf("画像ファイルをロード ... 開始\n");
	// ファイルパスをワイド文字列に変換する
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	// WICテクスチャのロード
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata_, scratchImage_);
	assert(SUCCEEDED(hr));

	// フォルダパスとファイル名を分離する
	SeparateFilePath(wFilePath);
	printf("画像ファイルをロード ... 完了\n");
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
void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[]) {
	printf("DDSファイルに変換 ... 開始\n");
	HRESULT hr;
	
	// ミップマップレベル指定を検索
	int mipLevel = 0;
	for (int i = 0; i < numOptions; i++) {
		if (std::string(options[i]) == "-ml") {
			// ミップレベル指定
			mipLevel = std::stoi(options[i + 1]);
			break;
		}
	}
	printf("ミップレベル: %d\n", mipLevel);

	// ミップマップ生成
	DirectX::ScratchImage mipChain;
	hr = DirectX::GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, mipLevel, mipChain
	);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = std::move(mipChain);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 圧縮形式に変換（BC7の制約として、画像の横幅と縦幅が4の倍数でなければならない）
	DirectX::ScratchImage converted;
	hr = DirectX::Compress(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DXGI_FORMAT_BC7_UNORM_SRGB,
		DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT | DirectX::TEX_COMPRESS_PARALLEL,
		1.0f, converted
	);
	if (SUCCEEDED(hr)) {
		scratchImage_ = std::move(converted);
		metadata_ = scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャをSRGBとして扱う
	metadata_.format = DirectX::MakeSRGB(metadata_.format);
	// 出力ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDSファイル書き出し
	hr = DirectX::SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
	printf("DDSファイルに変換 ... 完了\n");
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
