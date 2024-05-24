#pragma once
# include "Anime.hpp"
# include "Common.hpp"
# include "Enemy.hpp"
# include "EnemyManager.hpp"
# include "ItemManager.hpp"
# include "HPBar.h"
# include "Player.hpp"
# include "StageInfo.hpp"
# include "Town.hpp"

// ゲームシーン
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;

private:

	/// @brief ゲームの進行状況を表します
	enum GameState
	{
		play,
		gameOver,
		clear
	};
	GameState gameState = play;

	/// @brief シーン開始時,操作方法を表示します
	bool showInstructionsFlag = true;

	/// @brief 操作の入力方法です
	InputGroup rightInput = (KeyD | KeyRight);
	InputGroup leftInput = (KeyA | KeyLeft);
	InputGroup upInput = (KeyW | KeyUp);
	InputGroup downInput = (KeyS | KeyDown);
	InputGroup shotInput = (KeyJ | XInput(0).buttonA);
	InputGroup shieldInput = (KeyK | XInput(0).buttonB);
	//移動方向ベクトル
	Vec2 moveInput = { 0,0 };

	/// @brief 円形ステージ
	static constexpr Circle earth{ 0, 0, StageInfo::earthR };

	const Font font{ FontMethod::SDF,52,Typeface::Bold };

	/// @brief クリアまでの制限時間
	static constexpr double clearTime = 110.0;

	/// @brief 1フレームの時間
	double deltaTime = 0.0;

	/// @brief 操作説明を閉じてからの合計時間
	double sceneTime = 0.0;

	/// @brief プレイヤーの大きさ
	static constexpr double playerSize = 1.3;

	/// @brief プレイヤーを作成します
	Player player{ playerSize };

	/// @brief プレイヤーの弾を持つ配列
	Array <Bullet> pBulletArr;

	Array <Town> townArr =
	{
		Town{TownType::Nomal,Circular{StageInfo::earthR,0}},
		Town{TownType::Nomal,Circular{StageInfo::earthR,90_deg}},
		Town{TownType::Nomal,Circular{StageInfo::earthR,180_deg}},
		Town{TownType::Nomal,Circular{StageInfo::earthR,270_deg}}
	};

	/// @brief 衝突範囲とテクスチャを合わせるオフセット値です
	static constexpr Circular townPosOffset{ 30,0 };

	double itemSpeed = 30.0;

	EnemyManger m_enemyManager{ player };

	ItemManager m_itemManager;

	// 2D カメラ
	const double cameraScale = 2.0;
	static constexpr bool cameraMode = true;
	static constexpr double cameraOffsetY = 90;

	Camera2D camera{ Vec2{ 0, 0 }, cameraScale,CameraControl::None_ };

	Mat3x2 mat = Mat3x2::Identity();
};
