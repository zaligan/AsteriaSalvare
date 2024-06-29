#include "Game.hpp"

using namespace Util;

struct HitEvent
{
	size_t bulletIndex;

	/// @brief 1:プレイヤー,,2:シールド
	int32 hitType = 0;
};

//TODO:この関数を使用した衝突処理の実装を行う
/// @brief 敵の弾とプレイヤーの衝突イベントを返します
/// @param player プレイヤーの衝突範囲
/// @param shield シールドの衝突範囲
/// @param enemyBullets 敵の弾の配列
/// @return 衝突した弾のインデックスと衝突したオブジェクトの種類
Array<HitEvent> PlayerVsEnemyBullets(const Circle& player, const Circle& shield, const Array<Bullet>& enemyBullets)
{
	Array<HitEvent> result;

	for (size_t i = 0; const auto& bullet : enemyBullets)
	{
		if (bullet.collider.intersects(player))
		{
			result.push_back(HitEvent{ i,1 });
		}
		else if (bullet.collider.intersects(shield))
		{
			result.push_back(HitEvent{ i,2 });
		}
		i++;
	}
	return result;
}


// ゲームシーン
Game::Game(const InitData& init)
	: IScene{ init }
{
	//シーン開始時、時間をリセット
	resetSceneTime();
	startSceneTimeCounter();

	//操作説明を閉じるまで時間を止める
	pauseDeltaTime();
}

void Game::update()
{
	//デバッグ用
#if _DEBUG
	if (KeyF1.down())
	{
		changeScene(State::Title);
	}
	if (KeyP.pressed())
	{
		return;
	}
#endif

	//BGM再生
	AudioAsset(U"gameBgm").setVolume(Volume::gameBGM);
	AudioAsset(U"gameBgm").play();

	//ゲームの状態遷移
	switch (m_gameState)
	{
	case GameState::play:
		break;

	case GameState::gameOver:
		if (m_shotInput.down())
		{
			changeScene(State::Title);
		}
		return;

	case GameState::clear:
		if (m_shotInput.down())
		{
			changeScene(State::Title);
		}
		return;

	default:
		break;
	}

	//ゲームシーンになった時、操作方法を表示
	if(m_showInstructionsFlag && !getData().testMode)
	{
		if (m_shotInput.pressed())
		{
			m_showInstructionsFlag = false;
			resumeDeltaTime();
		}
		return;
	}


	//状況に応じてゲームの状態を変更
	if (getSceneTime() > m_clearTime && !getData().testMode)
	{
		m_gameState = GameState::clear;
	}
	if (m_player.getHP() <= 0)
	{
		m_gameState = GameState::gameOver;
	}


	//----------------プレイヤー----------------

	//操作入力
	m_moveInput = { 0,0 };
	if (m_rightInput.pressed())
	{
		m_moveInput.x += 1.0;
	}
	if (m_leftInput.pressed())
	{
		m_moveInput.x -= 1.0;
	}
	if (m_upInput.pressed())
	{
		m_moveInput.y += 1.0;
	}
	if (m_downInput.pressed())
	{
		m_moveInput.y += -1.0;
	}
	m_moveInput.x += XInput(0).leftThumbX;
	m_moveInput.y += XInput(0).leftThumbY;
	
	m_moveInput = Vec2{ Clamp(m_moveInput.x ,-1.0,1.0) ,Clamp(m_moveInput.y ,-1.0,1.0) };
	m_player.move(m_moveInput);
	m_player.update();

	//攻撃orシールド展開
	m_player.setShieldActive(m_shieldInput.pressed());

	if (m_shotInput.pressed() && !m_shieldInput.pressed())
	{
		m_player.shot(m_playerBulletArray);
	}
	

	//弾の更新
	for (auto it = m_playerBulletArray.begin(); it != m_playerBulletArray.end();)
	{
		//移動
		Vec2 update(it->direction * PlayerBullet::speed * getDeltaTime());
		Line trajectory{ it->collider.center,it->collider.center + update };
		it->collider.setCenter(it->collider.center + update);

		//弾自身が範囲外なら削除
		if (it->collider.x < -StageInfo::bulletDeleteRange || StageInfo::bulletDeleteRange < it->collider.x || it->collider.y < -StageInfo::bulletDeleteRange || it->collider.y > StageInfo::bulletDeleteRange)
		{
			it = m_playerBulletArray.erase(it);
			continue;
		}
		it++;
	}

	m_enemyManager.processEnemyCollisions([this](Enemy& enemy) -> double
		{
			//プレイヤーと敵の衝突処理
			if (enemy.getCollider().intersects(m_player.getCollider()) && not enemy.isDead())
			{
				if (!getData().testMode)
				{
					m_player.damage(EnemyBullet::damage);
				}

				return PlayerBullet::damage;
			}

			//シールドと敵の衝突処理
			if (enemy.getCollider().intersects(m_player.getShieldCollider()) && not enemy.isDead())
			{
				m_player.shieldDamage(EnemyBullet::damage);
				return PlayerBullet::damage;
			}
	
			//プレイヤーの弾と敵の衝突処理
			for (auto it = m_playerBulletArray.begin(); it != m_playerBulletArray.end();)
			{
				if (enemy.getCollider().intersects(it->collider) && not enemy.isDead())
				{
					switch (it->type)
					{
						case BulletType::Normal:
							it = m_playerBulletArray.erase(it);
							return PlayerBullet::damage;
							
						case BulletType::Enhanced:
							if (!enemy.isHitThisBullet(it->ID))
							{
								it++;
								return PlayerBullet::enhancedBulletDamage;
							}
							it++;
							break;

						case BulletType::Town:
							it = m_playerBulletArray.erase(it);
							return PlayerBullet::damage;

						default:
							break;
					}
				}
				else
				{
					it++;
				}
			}

			return 0.0;
		});

	//敵の更新
	m_enemyManager.update();

	//倒した敵からアイテムをドロップ
	m_itemManager.dropEnemyItem(m_enemyManager.getDeadEnemyPosition());

	//ステージ上のアイテムを更新
	m_itemManager.update();
		
	//敵の弾と他オブジェクトの衝突処理
	m_enemyManager.processBulletCollisions([this](Bullet& bullet) -> bool
		{
			//シールドと敵の弾の衝突処理
			if (m_player.isShieldActive() && bullet.collider.intersects(m_player.getShieldCollider()))
			{
				m_player.shieldDamage(EnemyBullet::damage);
				m_player.addEnhancePoint(EnemyBullet::damage);
				return true;
			}

			//プレイヤーと敵の弾の衝突処理
			if (bullet.collider.intersects(m_player.getCollider()))
			{
				if (!getData().testMode)
				{
					m_player.damage(EnemyBullet::damage);
				}
				return true;
			}

			//街と敵の弾の衝突処理
			for (auto i : step(m_townArray.size()))
			{
				if (bullet.collider.intersects(m_townArray[i].getCollider()))
				{
					if (!getData().testMode)
					{
						m_townArray[i].damage(EnemyBullet::damage);
					}
					return true;
				}
			}

			//ステージと敵の弾の衝突処理
			return bullet.collider.intersects(m_stage);
		});


	//--------------アップグレードアイテム-------------------
	Array<StageItem>& itemArray = m_itemManager.getStageItem();
	for (auto it = itemArray.begin(); it != itemArray.end();)
	{
		//衝突判定（プレイヤー）
		Vec2 rectPos = OffsetCircular({ 0,0 }, it->position);
		Rect collider{ Arg::center(lround(rectPos.x),lround(rectPos.y)) ,20,20 };
		if (collider.intersects(m_player.getCollider()))
		{
			m_player.addOnePointUpgrade(it->itemType);
			it = itemArray.erase(it);
			continue;
		}
		it++;
	}
	for (auto& town:m_townArray)
	{
		if (town.getCollider().intersects(m_player.getCollider()))
		{
			town.addUpgrade(m_player.getUpgradeCnt(),ItemType::AttackUpgrade);
			m_player.resetUpgrade(ItemType::AttackUpgrade);
		}
	}
	
	//town更新
	for (size_t i = 0; i < m_townArray.size(); ++i)
	{
		m_townArray.at(i).update();
		m_townArray[i].shot(m_playerBulletArray);
	}

	//Town処理
	for (auto& town : m_townArray)
	{
		if (town.getHP() <= 0)
		{
			m_gameState = GameState::gameOver;
		}
	}


	//------------------カメラ計算------------------
	
	//引数の座標はゲーム内ではなく、回転の処理をした後、スケールを変える前の画面上座標
	m_camera.setTargetCenter(Circular{m_player.getR() + m_cameraOffsetY,0});

	if (m_player.getR()< StageInfo::stageRadius)
	{
		m_camera.setTargetScale(m_cameraScale * (1 - 0.65 * ((StageInfo::stageRadius - m_player.getR()) / StageInfo::stageRadius)));
	}
	else
	{
		m_camera.setTargetScale(m_cameraScale);
	}
	

	m_mat = Mat3x2::Rotate(-m_player.getTheta(), {0,0});
	m_camera.update();
}




void Game::draw() const
{
	{
		// 2D カメラの設定から Transformer2D を作成
		const auto t0 = m_camera.createTransformer();
		const Transformer2D t1{ m_mat,TransformCursor::Yes };

		//背景
		TextureAsset(U"gameBackGround").scaled(1.0).drawAt(0, 0);

		//ステージ
		m_stage.draw(Palette::Saddlebrown);
		for (int i = 0; i < 100; i++)
		{
			double tileDeg = Math::Pi * 2 / 100 * i;
			TextureAsset(U"ground").scaled(0.07).rotated(tileDeg).drawAt(OffsetCircular({ 0,0 }, StageInfo::stageRadius, tileDeg));
		}

		//街
		for (int32 i:step(m_townArray.size()))
		{
			double townRotate = Math::ToRadians(i * (360/m_townArray.size()));
			switch (m_townArray[i].getTownType())
			{
			case TownType::Normal:
				TextureAsset(U"normalTown").scaled(0.2).rotated(townRotate).drawAt(Circular(StageInfo::stageRadius + m_townPosOffset.r, townRotate + m_townPosOffset.theta));
				break;
			case TownType::Attack:
				TextureAsset(U"attackTown").scaled(0.2).rotated(townRotate).drawAt(Circular(StageInfo::stageRadius + m_townPosOffset.r, townRotate + m_townPosOffset.theta));
				break;
			case TownType::Defense:
				TextureAsset(U"defenseTown").scaled(0.2).rotated(townRotate).drawAt(Circular(StageInfo::stageRadius + m_townPosOffset.r, townRotate + m_townPosOffset.theta));
				break;
			case TownType::Special:
				TextureAsset(U"specialTown").scaled(0.2).rotated(townRotate).drawAt(Circular(StageInfo::stageRadius + m_townPosOffset.r, townRotate + m_townPosOffset.theta));
				break;
			default:
				throw Error(U"街のタイプが正しくないため描画できません");
				break;
			}
		}

		//プレイヤー
		m_player.draw();

		//p弾
		for (auto& bullet : m_playerBulletArray)
		{
			switch (bullet.type)
			{
			case BulletType::Normal:
				TextureAsset(U"playerBullet").rotated(m_player.getTheta()).drawAt(bullet.collider.center);
				break;
			case BulletType::Enhanced:
				TextureAsset(U"playerEnhancedBullet").rotated(m_player.getTheta()-90_deg).drawAt(bullet.collider.center);
				break;
			case BulletType::Town:
				TextureAsset(U"playerBullet").rotated(m_player.getTheta()).drawAt(bullet.collider.center);
				break;
			default:
				TextureAsset(U"townTex").rotated(m_player.getTheta()).drawAt(bullet.collider.center);
				break;
			}
		}
		//敵
		m_enemyManager.enemyDraw();
		//敵弾
		m_enemyManager.enemyBulletDraw();
		//アイテム
		m_itemManager.draw();
		
	}

	//-------UI------------
	//遊び方
	if (m_showInstructionsFlag && !getData().testMode)
	{
		TextureAsset(U"howToPlayTex").scaled(1.3).drawAt(Scene::Center());
	}

	//残り時間
	FontAsset(U"townHPFont")(U"{:.0f}"_fmt(Max(0.0, m_clearTime - getSceneTime()) )).drawAt(80,Scene::Center().x, 80);


	//プレイヤー強化
	for (auto i : step(3))
	{
		Rect{ 810 + 100 * i,950,100,60 }.draw(Palette::Darkgray);
		Rect{ 810 + 100 * i,950,100,60 }.drawFrame(5, Palette::Black);
		switch (i)
		{
		case 0:
			FontAsset(U"townHPFont")(m_player.getUpgradeCnt()[ItemType::AttackUpgrade]).drawAt(860 + 100 * i, 980, Palette::Blue);
			break;
		case 1:
			FontAsset(U"townHPFont")(m_player.getUpgradeCnt()[ItemType::ShieldUpgrade]).drawAt(860 + 100 * i, 980, Palette::Blue);
			break;
		case 2:
			FontAsset(U"townHPFont")(m_player.getUpgradeCnt()[ItemType::SpecialUpgrade]).drawAt(860 + 100 * i, 980, Palette::Blue);
			break;
		default:
			break;
		}
	}
	TextureAsset(U"AttackUpgrade").scaled(0.05).drawAt(830, 980);
	TextureAsset(U"shieldUpgrade").scaled(0.05).drawAt(930, 980);
	TextureAsset(U"specialUpgrade").scaled(0.05).drawAt(1030, 980);

	//街のHP
	double interval = 180;
	Array<String> townNameArr = { U"普通の街 HP",U"攻撃の街 HP" ,U"防御の街 HP" ,U"特殊の街 HP" };
	for (int i = 0; i < m_townArray.size(); i++)
	{
		RoundRect{ i*interval + 600,1020,180,60,10 }.draw(Palette::Gray);
		FontAsset(U"townHPFont")(townNameArr.at(i)).drawAt(i * interval + 690, 1030);

		switch (m_townArray[i].getTownType())
		{
		case TownType::Normal:
			TextureAsset(U"normalTown").scaled(0.08).drawAt(620,1020);
			break;
		case TownType::Attack:
			TextureAsset(U"attackTown").scaled(0.08).drawAt(800, 1020);
			break;
		case TownType::Defense:
			TextureAsset(U"defenseTown").scaled(0.08).drawAt(980, 1020);
			break;
		case TownType::Special:
			TextureAsset(U"specialTown").scaled(0.08).drawAt(1160, 1020);
			break;
		default:
			throw Error(U"街のタイプが正しくないため描画できません");
			break;
		}
	}
	for (size_t i = 0; i < m_townArray.size(); i++)
	{
		const double x = interval * i;
		const double y = 0;
		const RectF rect = RectF{ x, y, 150, 16 }.movedBy(615, 1050);
		m_townArray.at(i).drawHPBar(rect);
	}

	Quad{ {300,1030}, {300,1000}, {550,940}, {550,1030} }.draw(Palette::White);

	//GameOver
	switch (m_gameState)
	{
	case GameState::play:
		break;
	case GameState::gameOver:
		//TODO:見やすいフォントとサイズに変更
		FontAsset(U"gameOver")(U"GMAE OVER").drawAt(Scene::Center(), Palette::Gray);
		FontAsset(U"gameOver")(U"Press J Key").drawAt(40, { Scene::Center().x,Scene::Center().y + 100 }, Palette::Gray);
		break;
	case GameState::clear:
		FontAsset(U"gameClear")(U"CLEAR!").drawAt(Scene::Center(), Palette::Gray);
		FontAsset(U"gameClear")(U"Press J Key").drawAt(40, { Scene::Center().x,Scene::Center().y + 100 }, Palette::Gray);
		break;
	default:
		break;
	}

	//testモード
	if (getData().testMode)
	{
		RoundRect{ 1770,0,150,80,10 }.draw(ColorF{ Palette::Magenta ,0.4 });
		FontAsset(U"townHPFont")(U"test").drawAt(1845, 40);
	}
}
