#include "Game.hpp"

// ゲームシーン
Game::Game(const InitData& init)
	: IScene{ init }
{
	AudioAsset(U"gameBgm").setVolume(0.1);
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
	AudioAsset(U"gameBgm").play();

	//ゲームの状態遷移
	switch (gameState)
	{
	case GameState::play:
		break;

	case GameState::gameOver:
		if (shotInput.down())
		{
			changeScene(State::Title);
		}
		return;

	case GameState::clear:
		if (shotInput.down())
		{
			changeScene(State::Title);
		}
		return;

	default:
		break;
	}

	//ゲームシーンになった時、操作方法を表示
	if(showInstructionsFlag && !getData().testMode)
	{
		if (shotInput.pressed())
		{
			showInstructionsFlag = false;
		}
		return;
	}

	//時間管理
	deltaTime = Scene::DeltaTime();
	sceneTime += deltaTime;

	//状況に応じてゲームの状態を変更
	if (sceneTime > clearTime && !getData().testMode)
	{
		gameState = GameState::clear;
	}
	if (player.getHP() <= 0)
	{
		gameState = GameState::gameOver;
	}


	//----------------プレイヤー----------------

	//操作入力
	moveInput = { 0,0 };
	if (rightInput.pressed())
	{
		moveInput.x += 1.0;
	}
	if (leftInput.pressed())
	{
		moveInput.x -= 1.0;
	}
	if (upInput.pressed())
	{
		moveInput.y += 1.0;
	}
	if (downInput.pressed())
	{
		moveInput.y += -1.0;
	}
	moveInput.x += XInput(0).leftThumbX;
	moveInput.y += XInput(0).leftThumbY;
	
	moveInput = Vec2{ Clamp(moveInput.x ,-1.0,1.0) ,Clamp(moveInput.y ,-1.0,1.0) };
	player.move(moveInput);
	player.update(deltaTime);

	//攻撃orシールド展開
	player.setShieldActive(shieldInput.pressed());

	if (shotInput.pressed() && !shieldInput.pressed())
	{
		player.shot(playerBulletArray);
	}
	

	//弾の更新
	for (auto bulletIter = playerBulletArray.begin(); bulletIter != playerBulletArray.end();)
	{
		//移動
		Vec2 update(bulletIter->direction * PlayerBullet::speed * deltaTime);
		Line trajectory{ bulletIter->collider.center,bulletIter->collider.center + update };
		bulletIter->collider.setCenter(bulletIter->collider.center + update);

		//弾自身が範囲外なら削除
		if (bulletIter->collider.x < -StageInfo::bulletDeleteRange || StageInfo::bulletDeleteRange < bulletIter->collider.x || bulletIter->collider.y < -StageInfo::bulletDeleteRange || bulletIter->collider.y > StageInfo::bulletDeleteRange)
		{
			bulletIter = playerBulletArray.erase(bulletIter);
			continue;
		}

		//弾と敵の衝突処理
		bool isHit = false;
		auto& enemyArray = m_enemyManager.getEnemyArray();
		for (auto enemyIter = enemyArray.begin(); enemyIter != enemyArray.end();)
		{
			if ((Geometry2D::Distance(trajectory, enemyIter->getCenter()) < bulletIter->collider.r + enemyIter->getCollider().r) && enemyIter->getHP() > 0)
			{
				switch (bulletIter->type)
				{
				case BulletType::Normal:
					enemyIter->damage(bulletIter->damage);
					bulletIter = playerBulletArray.erase(bulletIter);
					isHit = true;
					break;

				case BulletType::Enhanced:
					if (!enemyIter->isHitThisBullet(bulletIter->ID))
					{
						enemyIter->damage(bulletIter->damage);
					}
					break;

				case BulletType::Town:
					enemyIter->damage(bulletIter->damage);
					bulletIter = playerBulletArray.erase(bulletIter);
					isHit = true;
					break;

				default:
					break;
				}
			}
			enemyIter++;
			if (isHit)
			{
				break;
			}
		}
		if (!isHit)
		{
			bulletIter++;
		}
	}

	//敵の更新
	m_enemyManager.update(deltaTime);

	//倒した敵からアイテムをドロップ
	m_itemManager.dropEnemyItem(m_enemyManager.getDeadEnemyPosition());

	//ステージ上のアイテムを更新
	m_itemManager.update(deltaTime);
		
	//敵の弾と他オブジェクトの衝突処理
	m_enemyManager.processBulletCollisions([this](Bullet& bullet) -> bool
		{
			//シールドと敵の弾の衝突処理
			if (player.isShieldActive() && bullet.collider.intersects(player.getShieldCollider()))
			{
				player.shieldDamage(bullet.damage);
				player.addEnhancePoint(bullet.damage / 10);
				return true;
			}

			//プレイヤーと敵の弾の衝突処理
			if (bullet.collider.intersects(player.getCollider()))
			{
				if (!getData().testMode)
				{
					player.damage(bullet.damage);
				}
				return true;
			}

			//街と敵の弾の衝突処理
			for (auto i : step(townArray.size()))
			{
				if (bullet.collider.intersects(townArray[i].getCollider()))
				{
					if (!getData().testMode)
					{
						townArray[i].damage(bullet.damage);
					}
					return true;
				}
			}

			//ステージと敵の弾の衝突処理
			return bullet.collider.intersects(m_stage);

			return false;
		});


	//--------------アップグレードアイテム-------------------
	Array<StageItem>& itemArray = m_itemManager.getStageItem();
	for (auto it = itemArray.begin(); it != itemArray.end();)
	{
		//衝突判定（プレイヤー）
		Vec2 rectPos = OffsetCircular({ 0,0 }, it->position);
		Rect collider{ Arg::center(lround(rectPos.x),lround(rectPos.y)) ,20,20 };
		if (collider.intersects(player.getCollider()))
		{
			player.addOnePointUpgrade(it->itemType);
			it = itemArray.erase(it);
			continue;
		}
		it++;
	}
	for (auto& town:townArray)
	{
		if (town.getCollider().intersects(player.getCollider()))
		{
			town.addUpgrade(player.getUpgradeCnt(),ItemType::AttackUpgrade);
			player.resetUpgrade(ItemType::AttackUpgrade);
		}
	}
	
	//town更新
	for (size_t i = 0; i < townArray.size(); ++i)
	{
		townArray.at(i).update(deltaTime);
		townArray[i].shot(playerBulletArray);
	}

	//Town処理
	for (auto& town : townArray)
	{
		if (town.getHP() <= 0)
		{
			gameState = GameState::gameOver;
		}
	}


	//------------------カメラ計算------------------
	
	//引数の座標はゲーム内ではなく、回転の処理をした後、スケールを変える前の画面上座標
	camera.setTargetCenter(Circular{player.getR() + cameraOffsetY,0});
	if (cameraMode)
	{
		if (player.getR()< StageInfo::stageRadius)
		{
			camera.setTargetScale(cameraScale * (1 - 0.65 * ((StageInfo::stageRadius - player.getR()) / StageInfo::stageRadius)));
		}
		else
		{
			camera.setTargetScale(cameraScale);
		}
	}

	mat = Mat3x2::Rotate(-player.getTheta(), {0,0});
	camera.update();
}




void Game::draw() const
{
	{
		// 2D カメラの設定から Transformer2D を作成
		const auto t0 = camera.createTransformer();
		const Transformer2D t1{ mat,TransformCursor::Yes };

		TextureAsset(U"gameBackGround").scaled(1.0).drawAt(0, 0);
		//ステージ
		m_stage.draw(Palette::Saddlebrown);
		for (int i = 0; i < 100; i++)
		{
			double tileDeg = Math::Pi * 2 / 100 * i;
			TextureAsset(U"ground").scaled(0.07).rotated(tileDeg).drawAt(OffsetCircular({ 0,0 }, StageInfo::stageRadius, tileDeg));
		}

		//街
		for (int32 i:step(townArray.size()))
		{
			double townRotate = Math::ToRadians(i * (360/townArray.size()));
			switch (townArray[i].getTownType())
			{
			case TownType::Nomal:
				TextureAsset(U"town").scaled(0.2).rotated(townRotate).drawAt(Circular(StageInfo::stageRadius + townPosOffset.r, townRotate + townPosOffset.theta));
				break;
			case TownType::Attack:
				break;
			case TownType::Defense:
				break;
			case TownType::Special:
				break;
			default:
				break;
			}
		}

		//プレイヤー
		player.draw();

		//p弾
		for (auto& bullet : playerBulletArray)
		{
			switch (bullet.type)
			{
			case BulletType::Normal:
				TextureAsset(U"playerBullet").rotated(player.getTheta()).drawAt(bullet.collider.center);
				break;
			case BulletType::Enhanced:
				TextureAsset(U"playerEnhancedBullet").rotated(player.getTheta()-90_deg).drawAt(bullet.collider.center);
				break;
			case BulletType::Town:
				TextureAsset(U"playerBullet").rotated(player.getTheta()).drawAt(bullet.collider.center);
				break;
			default:
				TextureAsset(U"townTex").rotated(player.getTheta()).drawAt(bullet.collider.center);
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
	if (showInstructionsFlag && !getData().testMode)
	{
		TextureAsset(U"howToPlayTex").scaled(1.3).drawAt(Scene::Center());
	}

	//残り時間
	FontAsset(U"townHPFont")(U"{:.0f}"_fmt(Max(0.0, clearTime - sceneTime) )).drawAt(80,Scene::Center().x, 80);

	//街のHP
	double interval = 180;
	Array<String> townNameArr = { U"普通の街 HP",U"攻撃の街 HP" ,U"防御の街 HP" ,U"特殊の街 HP" };
	for (int i = 0; i < townArray.size(); i++)
	{
		RoundRect{ i*interval + 600,1020,180,60,10 }.draw(Palette::Gray);
		FontAsset(U"townHPFont")(townNameArr.at(i)).drawAt(i * interval + 690, 1030);
	}
	for (size_t i = 0; i < townArray.size(); i++)
	{
		const double x = interval * i;
		const double y = 0;
		const RectF rect = RectF{ x, y, 150, 16 }.movedBy(615, 1050);
		townArray.at(i).drawHPBar(rect);
	}

	//プレイヤー強化
	for (auto i : step(3))
	{
		Rect{ 810 + 100 * i,950,100,60 }.draw(Palette::Darkgray);
		Rect{ 810 + 100 * i,950,100,60 }.drawFrame(5, Palette::Black);
		switch (i)
		{
		case 0:
			FontAsset(U"townHPFont")(player.getUpgradeCnt()[ItemType::AttackUpgrade]).drawAt(860 + 100 * i, 980, Palette::Blue);
			break;
		case 1:
			FontAsset(U"townHPFont")(player.getUpgradeCnt()[ItemType::ShieldUpgrade]).drawAt(860 + 100 * i, 980, Palette::Blue);
			break;
		case 2:
			FontAsset(U"townHPFont")(player.getUpgradeCnt()[ItemType::SpecialUpgrade]).drawAt(860 + 100 * i, 980, Palette::Blue);
			break;
		default:
			break;
		}
	}
	TextureAsset(U"AttackUpgrade").scaled(0.05).drawAt(830, 980);
	TextureAsset(U"shieldUpgrade").scaled(0.05).drawAt(930, 980);
	TextureAsset(U"specialUpgrade").scaled(0.05).drawAt(1030, 980);

	//GameOver
	switch (gameState)
	{
	case GameState::play:
		break;
	case GameState::gameOver:
		font(U"GMAE OVER").drawAt(Scene::Center(), Palette::Gray);
		font(U"Press J Key").drawAt(40, { Scene::Center().x,Scene::Center().y + 100 }, Palette::Gray);
		break;
	case GameState::clear:
		font(U"CLEAR!").drawAt(Scene::Center(), Palette::Gray);
		font(U"Press J Key").drawAt(40, { Scene::Center().x,Scene::Center().y + 100 }, Palette::Gray);
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
