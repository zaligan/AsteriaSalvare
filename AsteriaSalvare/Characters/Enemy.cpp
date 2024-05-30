#include "Enemy.hpp"

Enemy::Enemy(double r, double theta)
{
	double spawn_r = StageInfo::stageRadius + r;
	m_position = Circular{ spawn_r, theta };
	m_from = m_position;
	//一番近い家のラジアン 0, π/2, π, 3π/2
	double houseDeg = (static_cast<int>((m_position.theta + (Math::HalfPi / 2)) / Math::HalfPi) % 4) * Math::HalfPi;
	double enemyRandomTheta = Math::ToRadians(Random(-60, 60)) + houseDeg;
	double enemyRandomR = m_enemyHouseRange + Random(0, 120);

	//OffsetCircularからCircularに変換するためVec2を経由
	m_to = Vec2(OffsetCircular({ Circular(StageInfo::stageRadius,houseDeg) }, enemyRandomR, enemyRandomTheta));
}

void Enemy::draw() const
{
	if (m_currentHP <= 0)
	{
		const ScopedRenderStates2D blend{ BlendState::Additive };
		m_explosionAnime.drawAt(OffsetCircular({ 0,0 }, m_position.r, m_position.theta));
	}
	else
	{
		TextureAsset(U"enemy").rotated(m_position.theta).drawAt(m_collider.center);
	}
}

void Enemy::update()
{
	//死亡アニメーションが終了したら死亡フラグをたてる
	if (m_currentHP <= 0)
	{
		m_deadFlag = m_explosionAnime.update();
	}
	
	//移動
	const double t = Min(stopwatch.sF() / 10, 1.0);
	m_position.r = Math::Lerp(m_from.r, m_to.r, t);
	m_position.theta = Math::LerpAngle(m_from.theta, m_to.theta, t);
	m_collider.setPos(m_position);
}

bool Enemy::damage(double damage)
{
	m_currentHP -= damage;
	return m_currentHP <= 0;
}

double Enemy::getHP() const
{
	return m_currentHP;
}

bool Enemy::shot(Array<Bullet>& enemyBulletArray, const Vec2& playerPosition)
{
	//eShotCoolTime経過するごとに発射
	if (stopwatch.sF() > m_eShotCoolTime * (m_shotCnt + 1))
	{
		m_shotCnt++;

		//プレイヤーまでのベクトル
		const Vec2 toPlayer = playerPosition - getCenter();
		//街までのベクトル
		const Vec2 toTown = OffsetCircular({ 0,0 }, StageInfo::stageRadius, ((static_cast<int>((m_position.theta + (Math::HalfPi / 2)) / Math::HalfPi) % 4) * Math::HalfPi)) - getCollider().center;
		//弾が発射される方向
		Vec2 direction = {0,0};

		//プレイヤーと街の近いほうを狙う
		if (toTown.lengthSq() < toPlayer.lengthSq())
		{
			direction = toTown.normalized();
		}
		else
		{
			direction = toPlayer.normalized();
		}
		if (direction.isZero())
		{
			direction = Vec2{ 0,1 };
		}

		enemyBulletArray << Bullet{ BulletType::Enemy,0, Circle{Arg::center(getCenter()),EnemyBullet::size},direction ,m_bulletDamage };

		return true;
	}
	return false;
}

Circle Enemy::getCollider() const
{
	return m_collider;
}

bool Enemy::isDead() const
{
	return m_deadFlag;
}

Circular Enemy::getCenter() const
{
	return m_position;
}

bool Enemy::isHitThisBullet(int32 bulletID)
{
	for (auto& m_bulletID : m_hitEnhancedBulletArr)
	{
		if (m_bulletID == bulletID)
		{
			return true;
		}
	}
	m_hitEnhancedBulletArr << bulletID;
	return false;
}



