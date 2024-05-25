#pragma once
# include "Anime.hpp"
# include "Common.hpp"
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

	/// @brief シーンの更新を行います
	void update() override;

	/// @brief シーンの描画を行います
	void draw() const override;

private:

	/// @brief ゲームの進行状況を表します
	enum class GameState
	{
		play,
		gameOver,
		clear
	};
	/// @brief 現在のゲームの進行状況です
	GameState gameState = GameState::play;

	/// @brief シーン開始時,操作方法を表示します
	bool showInstructionsFlag = true;

	/// @brief 操作の入力方法です
	InputGroup rightInput = (KeyD | KeyRight);
	InputGroup leftInput = (KeyA | KeyLeft);
	InputGroup upInput = (KeyW | KeyUp);
	InputGroup downInput = (KeyS | KeyDown);
	InputGroup shotInput = (KeyJ | XInput(0).buttonA);
	InputGroup shieldInput = (KeyK | XInput(0).buttonB);

	/// @brief 入力された移動方向です
	Vec2 moveInput = { 0,0 };

	/// @brief 円形ステージです
	static constexpr Circle stage{ 0, 0, StageInfo::stageRadius };


	const Font font{ FontMethod::SDF,52,Typeface::Bold };

	/// @brief クリアまでの制限時間
	static constexpr double clearTime = 110.0;

	/// @brief 1フレームの時間
	double deltaTime = 0.0;

	/// @brief 操作説明を閉じてからの合計時間
	double sceneTime = 0.0;


	/// @brief プレイヤーを作成します
	Player player;

	/// @brief プレイヤーの弾を持つ配列
	Array <Bullet> playerBulletArray;

	Array <Town> townArray =
	{
		Town{TownType::Nomal,Circular{StageInfo::stageRadius,0}},
		Town{TownType::Nomal,Circular{StageInfo::stageRadius,90_deg}},
		Town{TownType::Nomal,Circular{StageInfo::stageRadius,180_deg}},
		Town{TownType::Nomal,Circular{StageInfo::stageRadius,270_deg}}
	};

	/// @brief 衝突範囲とテクスチャを合わせるオフセット値です
	static constexpr Circular townPosOffset{ 30,0 };

	EnemyManger m_enemyManager{ player };

	ItemManager m_itemManager;

	// 2D カメラ
	static constexpr double cameraScale = 2.0;
	static constexpr bool cameraMode = true;
	static constexpr double cameraOffsetY = 90;

	Camera2D camera{ Vec2{ 0, 0 }, cameraScale,CameraControl::None_ };

	Mat3x2 mat = Mat3x2::Identity();
};
