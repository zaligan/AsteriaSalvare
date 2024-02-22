# include <Siv3D.hpp> // Siv3D v0.6.14
# include "Common.hpp"
# include "Title.hpp"
# include "Game.hpp"
# include "AssetReader/AssetReader.hpp"

void Main()
{
	//window設定
	Window::SetStyle(WindowStyle::Sizable);
	Window::SetTitle(U"AsteriaSalvare");
	Scene::Resize(1920, 1080);
	Window::Resize(Size{ 1280, 720 });

	// Releaseビルド時にフルスクリーンにする
#if not _DEBUG
	Window::SetFullscreen(true);
#endif

	System::Update();
	
	// AssetReaderにアセットリストのパスを渡します。
	AssetReader assetReader{ U"Asset/asset-list.txt" };

	// AssetReaderの初期化
	if (not assetReader.init())
	{
		throw Error{ U"AssetReaderの初期化に失敗しました。" };
	}

	// アセットの登録
	if (not assetReader.registerAsset())
	{
		throw Error{ U"アセットの登録に失敗しました。" };
	}

	//シーンマネージャーを作成
	App manager;

	//シーンの追加
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);


	// メインループ
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
