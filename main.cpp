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
	assert(argc >= NumArgument);	// 引数の個数チェック

	// COMライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// テクスチャコンバータ
	TextureConverter converter;
	// テクスチャ変換
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	// COMライブラリの終了
	CoUninitialize();

	return 0;
}