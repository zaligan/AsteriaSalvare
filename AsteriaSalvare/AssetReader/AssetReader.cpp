#include "AssetReader.hpp"

AssetReader::AssetReader(const FilePath& assetPath) :
	m_assetPath{ assetPath }{}

bool AssetReader::init()
{
	m_reader.open(m_assetPath);
	if (not m_reader)
	{
		throw Error{ U"'{}' could not be opened."_fmt(m_assetPath)};
	}
	return true;
}

bool AssetReader::registerAsset()
{
	String line;

	//ファイルの終端まで改行区切りでループ
	while (m_reader.readLine(line))
	{
		//行をタブで分割した文字列の配列
		const Array<String> items = line.split(U'\t');

		//1行に含まれるアセットの情報が不正な場合
		if ((items[0] == U"Font" && items.size() != 4) || (items[0] != U"Font" && items.size() != 3))
		{
			throw Error{ U"１行に含まれるアセット情報が不正です。`{}`"_fmt(m_assetPath)};
		}

		//アセットの型
		const String& type = items[0];
		//アセットに付ける名前
		const String& assetName = items[1];
		//アセットとして登録するファイルのパス
		const String& path = items[2];

		//Textureアセットの登録
		if (type == U"Texture")
		{
			TextureAsset::Register(assetName, path);
			continue;
		}

		//Audioアセットの登録
		if (type == U"Audio")
		{
			AudioAsset::Register(assetName, path);
			continue;
		}

		//Fontアセットの登録
		if (type == U"Font")
		{
			//items[3]はフォントの大きさ
			const int32 fontSize = Parse<int32>(items[3]);
			FontAsset::Register(assetName, fontSize, path);
			continue;
		}
	}

	return true;
}
