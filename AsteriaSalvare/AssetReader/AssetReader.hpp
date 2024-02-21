#pragma once
# include <Siv3D.hpp> // Siv3D v0.6.14

/// @brief TSV形式のアセットリストを読み込み、アセットを登録します。
class AssetReader
{
public:
	/// @brief assetPath から AssetReader を構築します。
	/// @param assetPath asset のリストが記述されたファイルのパスです。
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
	/// @brief アセットリストのファイルのパスです。
	FilePath m_assetPath;

	/// @brief assetPath から読み込むTextReaderです。
	TextReader m_reader;
};
