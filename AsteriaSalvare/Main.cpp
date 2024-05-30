# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Common.hpp" // シーン間で共有するデータ
# include "Title.hpp" // タイトルシーン
# include "Game.hpp" // ゲームシーン
# include "AssetReader.hpp"	//アセットを読み込むクラス

void Main()
{
	//window設定
	Window::SetStyle(WindowStyle::Sizable);
	Window::SetTitle(U"AsteriaSalvare");
	Scene::SetResizeMode(ResizeMode::Keep);
	Scene::Resize(1920, 1080);
	Window::Resize(Size{ 1280, 720 });

#if not _DEBUG
	Window::SetFullscreen(true);
#endif
	System::Update();

	//シーンマネージャーを作成
	App manager;


	//アセット情報を登録
	AssetReader assetReader(U"Asset/asset-list.tsv");
	if (not assetReader.init())
	{
		throw Error{ U"AssetReaderの初期化に失敗しました。" };
	}
	if (not assetReader.registerAsset())
	{
		throw Error{ U"アセットの登録に失敗しました。" };
	}


	//シーンの追加
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);


	//----開発用----
	//Gameシーンから開始する場合、下の行のコメントを外す
	manager.init(State::Game);

	//調整用無敵モード、通常プレイ時はコメントアウト
	//manager.get()->testMode = true;


	//メインループ
	while (System::Update())
	{
		//シーンに実装された.update() 次に.draw()の順で実行される
		if (not manager.update())
		{
			break;
		}
	}
}
