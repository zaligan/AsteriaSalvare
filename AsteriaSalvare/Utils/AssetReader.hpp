#pragma once
#include <Siv3D.hpp> // Siv3D v0.6.14

/// @brief TSV形式のテキストファイルを読み込み、アセットを登録します。
class AssetReader
{
public:
	/// @brief assetPath から AssetReader を構築します。
	/// @param assetPath 登録するアセット情報が記述されたファイルのパスです。
	AssetReader(const FilePath& assetPath);

	/// @brief 初期化します。メイン関数の先頭で１度だけ呼び出してください。
	/// @return 初期化に成功した場合 true を返します。
	[[nodiscard]]
	bool init();

	/// @brief アセットを登録します。
	/// @return アセットの登録に成功した場合 true を返します。
	[[nodiscard]]
	bool registerAsset();

private:
	/// @brief 参照するアセットリストのファイルのパスです。
	FilePath m_assetPath;

	/// @brief m_assetPath のファイルを読み込む TextReaderです。
	TextReader m_reader;
};
