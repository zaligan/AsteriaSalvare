# include "EnemyManager.hpp"

EnemyManger::EnemyManger(const Player& player, double clearTime) : m_player(player), m_clearTime(clearTime)
{
}

void EnemyManger::update(double deltaTime)
{
	m_deltaTime = deltaTime;
	m_sceneTime += deltaTime;

	//敵の生成
	spawnEnemy();

	//1体ずつ更新
	for (auto& enemy : m_enemyArray)
	{
		enemy.update();
		enemy.shot(m_enemyBulletArray, m_player.getCenter());
	}

	//死亡している敵を削除
	removeDeadEnemies();

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
	for (auto& bullet : m_enemyBulletArray)
	{
		TextureAsset(U"enemyBullet").drawAt(bullet.collider.center);
	}
}

Array<Enemy>& EnemyManger::getEnemyArray()
{
	return m_enemyArray;
}

void EnemyManger::processBulletCollisions(std::function<bool(Bullet&)> func)
{
	for (auto it = m_enemyBulletArray.begin(); it != m_enemyBulletArray.end();)
	{
		//弾が他オブジェクトと衝突した時の処理
		if (func(*it))
		{
			it = m_enemyBulletArray.erase(it);
		}
		else
		{
			it++;
		}
	}
}

Array<Vec2> EnemyManger::getDeadEnemyPosition()
{
	return m_deadEnemyPosition;
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
	//移動
	for (auto& bullet : m_enemyBulletArray)
	{
		Vec2 move(bullet.direction * EnemyBullet::speed * m_deltaTime);
		bullet.collider.setCenter(bullet.collider.center + move);
	}

	//範囲外の弾は削除
	m_enemyBulletArray.remove_if([](const Bullet& b) {return (b.collider.center.x < -StageInfo::bulletDeleteRange) || (b.collider.center.x > StageInfo::bulletDeleteRange) || (b.collider.center.y < -StageInfo::bulletDeleteRange) || (b.collider.center.y > StageInfo::bulletDeleteRange); });
}

void EnemyManger::removeDeadEnemies()
{
	m_deadEnemyPosition.clear();

	for (auto it = m_enemyArray.begin(); it != m_enemyArray.end();)
	{
		if (it->isDead())
		{
			m_deadEnemyPosition << it->getCenter();
			it = m_enemyArray.erase(it);
		}
		else
		{
			++it;
		}
	}
}

int32 EnemyManger::enemySpawnCalk(double currentTime)
{
	return static_cast<int32>(pow(m_baseSpawnNum, currentTime / 20));
}
