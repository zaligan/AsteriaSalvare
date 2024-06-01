#pragma once
#include <Siv3D.hpp>
#include "Enemy.hpp"
#include "Player.hpp"
#include "ItemManager.hpp"

/// @brief 敵と敵の弾を管理します
class EnemyManger
{
public:
	EnemyManger(const Player& player, double clearTime);

	/// @brief EnemyManagerを更新します
	/// @param deltaTime 前フレームからの経過時間を渡します
	void update(double deltaTime);

	/// @brief 敵を描画します
	void enemyDraw() const; 

	/// @brief 敵の弾を描画します
	void enemyBulletDraw() const;

	/// @brief 敵が他オブジェクトと衝突した時の処理を行います
	/// @param func 衝突時に他オブジェクト側で行う処理をラムダ式で渡します,戻り値は敵に与えるダメージ量です 
	void processEnemyCollisions(std::function<double(Enemy&)> func);

	/// @brief 弾が他オブジェクトと衝突した時の処理を行います
	/// @param func 衝突時に他オブジェクト側で行う処理をラムダ式で渡します,戻り値が true なら弾を削除します
	void processBulletCollisions(std::function<bool(Bullet&)> func);

	Array<Vec2> getDeadEnemyPosition();

private:
	/// @brief 敵の生成を行います
	void spawnEnemy();

	/// @brief 敵の弾の更新を行います
	void bulletUpdate();

	/// @brief 死亡している敵を配列から削除します
	void removeDeadEnemies();

	/// @brief 経過時間に応じてスポーンさせる敵の数を計算します
	/// @param currentTime ゲームシーン開始からの経過時間
	/// @return 一度のスポーンで出現させる敵の数
	int32 enemySpawnCalk(double currentTime);

	/// @brief プレイヤーの参照です
	const Player& m_player;

	/// @brief クリアまでの制限時間です
	double m_clearTime;

	/// @brief 前フレームからの経過時間です
	double m_deltaTime = 0;

	/// @brief ゲームシーン開始からの経過時間です
	double m_sceneTime = 0;

	/// @brief 敵の配列です
	Array <Enemy> m_enemyArray;

	/// @brief 敵の弾の配列です
	Array <Bullet> m_enemyBulletArray;

	/// @brief 敵の死亡位置を管理する配列です
	Array <Vec2> m_deadEnemyPosition;

	/// @brief 敵のスポーン時間を計測します
	double m_spawnTimer = 0;

	/// @brief 一度のスポーンで出現させる敵の数の基準値です
	static constexpr double m_baseSpawnNum = 2;

	/// @brief 敵をスポーンさせる時間間隔です
	static constexpr double m_spawnIntervalSeconds = 1.5;

	/// @brief 敵が出現する地表からの最長距離です
	static constexpr double m_maxSpawnR = 330;

	/// @brief 敵が出現する地表からの最短距離です
	static constexpr double m_minSpawnR = 310;

	/// @brief 敵が出現する範囲の角度成分(度数法)です
	static constexpr double m_maxSpawnTheta = 360;

	/// @brief 敵が出現する範囲の角度成分(度数法)です
	static constexpr double m_minSpawnTheta = 0;
};








