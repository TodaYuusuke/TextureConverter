#pragma once
#include <string>

#include "DirectXTex/DirectXTex.h"

/// <summary>
/// テクスチャをDDSに変換するクラス
/// </summary>
class TextureConverter final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void ConvertTextureWICToDDS(const std::string& filePath);


private: // ** メンバ変数 ** //


private: // ** プライベートなメンバ関数 ** //

	/// <summary>
	/// テクスチャファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString">マルチバイト文字列</param>
	/// <returns>ワイド文字列</returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);
};