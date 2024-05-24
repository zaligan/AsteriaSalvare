# include "EnemyManager.hpp";

EnemyManger::EnemyManger(const Player& player) : m_player(player)
{
}

void EnemyManger::update(double deltaTime)
{
	m_deltaTime = deltaTime;
	m_sceneTime += deltaTime;

	//敵の生成
	spawnEnemy();

	//敵の更新
	for (auto& enemy : m_enemyArray)
	{
		enemy.update();
	}

	//敵の弾の更新
	bulletUpdate();
}

void EnemyManger::enemyDraw() const
{
	for (auto& enemy : m_enemyArray)
	{
		enemy.draw();
	}
}

void EnemyManger::enemyBulletDraw() const
{
	for (auto& eBullet : m_enemyBulletArray)
	{
		TextureAsset(U"enemyBullet").drawAt(eBullet.collider.center);
	}
}

Array<Vec2> EnemyManger::removeDeadEnemies()
{
	return Array<Vec2>();
}

void EnemyManger::spawnEnemy()
{
	//ランダムスポーン
	m_spawnTimer += m_deltaTime;
	if (m_spawnTimer > m_spawnIntervalSeconds)
	{
		m_spawnTimer -= m_spawnIntervalSeconds;
		for (int i : step(enemySpawnCalk(m_sceneTime)))
		{
			const double r = Random(m_minSpawnR, m_maxSpawnR);
			const double theta = Math::ToRadians(Random(m_minSpawnTheta, m_maxSpawnTheta));
			m_enemyArray << Enemy{ r, theta };
		}
	}
}

void EnemyManger::bulletUpdate()
{
	//移動処理
	for (auto& bullet : m_enemyBulletArray)
	{
		//TODO:updateという名前を分かりやすいものに変更する
		Vec2 update(bullet.direction * EnemyBullet::speed * m_deltaTime);
		bullet.collider.setCenter(bullet.collider.center + update);
	}

	//範囲外の弾は削除
	m_enemyBulletArray.remove_if([](const Bullet& b) {return (b.collider.center.x < -StageInfo::bulletDeleteRange) || (b.collider.center.x > StageInfo::bulletDeleteRange) || (b.collider.center.y < -StageInfo::bulletDeleteRange) || (b.collider.center.y > StageInfo::bulletDeleteRange); });
}
