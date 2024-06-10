#pragma once
#include "Anime.hpp"
#include "Common.hpp"
#include "EnemyManager.hpp"
#include "ItemManager.hpp"
#include "HPBar.h"
#include "Player.hpp"
#include "StageInfo.hpp"
#include "Town.hpp"

/// @brief ゲームシーン
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
	GameState m_gameState = GameState::play;

	/// @brief シーン開始時,操作方法を表示します
	bool m_showInstructionsFlag = true;

	/// @brief 操作の入力方法(右)です
	InputGroup m_rightInput = (KeyD | KeyRight);

	/// @brief 操作の入力方法(左)です
	InputGroup m_leftInput = (KeyA | KeyLeft);

	/// @brief 操作の入力方法(上)です
	InputGroup m_upInput = (KeyW | KeyUp);

	/// @brief 操作の入力方法(下)です
	InputGroup m_downInput = (KeyS | KeyDown);

	/// @brief 操作の入力方法(攻撃)です
	InputGroup m_shotInput = (KeyJ | XInput(0).buttonA);

	/// @brief 操作の入力方法(シールド)です
	InputGroup m_shieldInput = (KeyK | XInput(0).buttonB);

	/// @brief 入力された移動方向です
	Vec2 m_moveInput = { 0,0 };

	/// @brief 円形ステージです
	static constexpr Circle m_stage{ 0, 0, StageInfo::stageRadius };

	/// @brief クリアまでの制限時間
	static constexpr double m_clearTime = 110.0;


	/// @brief プレイヤーを作成します
	Player m_player;

	/// @brief プレイヤーの弾を持つ配列
	Array <Bullet> m_playerBulletArray;

	/// @brief 街の配列
	Array <Town> m_townArray =
	{
		Town{TownType::Normal,Circular{StageInfo::stageRadius,0}},
		Town{TownType::Attack,Circular{StageInfo::stageRadius,90_deg}},
		Town{TownType::Defense,Circular{StageInfo::stageRadius,180_deg}},
		Town{TownType::Special,Circular{StageInfo::stageRadius,270_deg}}
	};

	/// @brief 衝突範囲とテクスチャを合わせるオフセット値です
	static constexpr Circular m_townPosOffset{ 30,0 };

	/// @brief 敵と敵の弾の管理をします
	EnemyManger m_enemyManager{ m_player, m_clearTime };

	/// @brief アイテムの管理をします
	ItemManager m_itemManager;

	/// @brief カメラのズーム倍率です
	static constexpr double m_cameraScale = 2.0;

	/// @brief カメラが映す中心のオフセット値です
	static constexpr double m_cameraOffsetY = 90;

	/// @brief プレイヤーを追従するためのカメラです
	Camera2D m_camera{ Vec2{ 0, 0 }, m_cameraScale,CameraControl::None_ };

	/// @brief ステージを回転して描画するための座標変換行列です
	Mat3x2 m_mat = Mat3x2::Identity();
};
