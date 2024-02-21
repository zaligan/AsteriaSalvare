# include <Siv3D.hpp> // Siv3D v0.6.14
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
	



	// メインループ
	while (System::Update())
	{
		TextureAsset(U"backPic").draw();
	}
}
