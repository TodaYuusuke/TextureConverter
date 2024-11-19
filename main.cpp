#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "Class/TextureConverter.h"

enum Argument {
	kApplicationPath,	// アプリケーションのパス
	kFilePath,			// 渡されたファイルのパス

	NumArgument
};

int main(int argc, char* argv[]) {
	// コマンドライン引数指定なし
	if (argc < NumArgument) {
		TextureConverter::OutputUsage();
		return 0;
	}

	// COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// オプションの数
	int numOptions = argc - NumArgument;
	/// オプション配列（ダブルポインタ）
	char** options = argv + NumArgument;

	// テクスチャコンバータ
	TextureConverter converter;
	// テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath], numOptions, options);

	// COMライブラリの終了
	CoUninitialize();

	return 0;
}