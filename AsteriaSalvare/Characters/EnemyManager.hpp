#pragma once
# include <Siv3D.hpp>
# include "Enemy.hpp"
# include "Player.hpp"
# include "ItemManager.hpp"

class EnemyManger
{
public:
	EnemyManger(const Player& player);

	void update(double deltaTime);

	void enemyDraw() const; 

	void enemyBulletDraw() const;

	Array<Enemy>& getEnemyArray()
	{
		return m_enemyArray;
	}

	Array<Bullet>& getEnemyBulletArray()
	{
		return m_enemyBulletArray;
	}

	/// @brief 死亡している敵を配列から削除します
	/// @return 削除した敵の座標
	Array<Vec2> removeDeadEnemies();

private:
	/// @brief 敵の生成を行います
	void spawnEnemy();

	void bulletUpdate();

	

	/// @brief 経過時間に応じてスポーンさせる敵の数を計算します
	/// @param currentTime ゲームシーン開始からの経過時間
	/// @return 一度のスポーンで出現させる敵の数
	int32 enemySpawnCalk(double currentTime)
	{
		return 5;
	}

	const Player& m_player;

	double m_deltaTime = 0;
	double m_sceneTime = 0;

	//Enemy
	Array <Enemy> m_enemyArray;
	Array <Bullet> m_enemyBulletArray;

	//敵のスポーン時間を管理します
	double m_spawnTimer = 0;

	static constexpr int32 m_baseSpawnNum = 1;

	//敵をスポーンさせる時間間隔です
	static constexpr double m_spawnIntervalSeconds = 1.0;

	//敵が出現する地表からの距離範囲です
	static constexpr double m_maxSpawnR = 330;
	static constexpr double m_minSpawnR = 310;

	//敵が出現する角度成分(度数法)です
	static constexpr double m_maxSpawnTheta = 360;
	static constexpr double m_minSpawnTheta = 0;
};








