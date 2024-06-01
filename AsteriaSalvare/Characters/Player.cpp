#include "Player.hpp"

Player::Player() : m_shield(*this)
{}

void Player::update(double deltaTime)
{
	m_deltaTime = deltaTime;
	m_shotTimer += m_deltaTime;
	m_shield.update();
	m_collider.setCenter(m_position);

	//以下エンハンス関連
	//強化値が一定を超えたら強化モード
	if (m_enhancePoint > m_enhanceThreshold)
	{
		//強化時１度だけ鳴らす
		if (!m_isEnhanced)
		{
			AudioAsset(U"playerEnhanced").playOneShot();
		}
		m_isEnhanced = true;
	}

	//強化値が0以下の時強化モード解除
	if (m_enhancePoint <= 0)
	{
		m_isEnhanced = false;
		m_enhanceEffectAnime.reset();
	}

	m_enhanceEffectAnime.update();

}

void Player::draw() const
{
	TextureAsset(U"player").scaled(m_playerSize).rotated(m_position.theta).drawAt(m_position);
	m_shield.draw();

	if(m_isEnhanced)
	{
		m_enhanceEffectAnime.drawAt(m_position);
	}
}

void Player::move(Vec2 moveInput)
{
	m_position.r += m_vertSpeed * m_deltaTime * moveInput.y;
	m_position.theta += 2 * Math::Pi * m_deltaTime / (m_maxRotateSpeed + ((m_minRotateSpeed - m_maxRotateSpeed) * ((getR() - m_moveRange.minRadius) / (m_moveRange.maxRadius - m_moveRange.minRadius)))) * moveInput.x;
	m_position.r = Clamp(m_position.r, m_moveRange.minRadius, m_moveRange.maxRadius);
}

void Player::shot(Array<Bullet>& bulletArr)
{
	if (m_isEnhanced)
	{
		//強化時の射撃
		if (m_shotTimer > m_enhancedShotCoolTime)
		{
			m_shotTimer = 0;

			Vec2 direction = Vec2(Circular{ 1,getTheta() }).normalized();

			bulletArr << Bullet{ BulletType::Enhanced,m_enhancedBulletID,Circle{getCenter(),PlayerBullet::enhancedBulletSize}, direction };

			m_enhancedBulletID++;

			m_enhancePoint = Max(m_enhancePoint - m_shotLostEnhancePoint, 0.0);

			AudioAsset(U"playerShoot").playOneShot(Volume::playerShot, 0, 1.0);
		}
	}
	else
	{
		//通常時の射撃
		if (m_shotTimer > m_shotCoolTime)
		{
			m_shotTimer = 0;

			Vec2 direction = Vec2(Circular{ 1,getTheta() }).normalized();

			bulletArr << Bullet{ BulletType::Normal,0,Circle{getCenter(),PlayerBullet::size}, direction };

			AudioAsset(U"playerShoot").playOneShot(Volume::playerShot, 0, 1.0);
		}
	}
}

double Player::getHP() const
{
	return m_currentHP;
}

void Player::damage(double damege)
{
	m_currentHP -= damege;
}

Vec2 Player::getCenter() const
{
	return Vec2(m_position);
}

Circular Player::getCircular() const
{
	return m_position;
}

double Player::getR() const
{
	return m_position.r;
}

double Player::getTheta() const
{
	return m_position.theta;
}

Circle Player::getCollider() const
{
	return m_collider;
}

Circle Player::getShieldCollider()
{
	return m_shield.getCollider();
}

bool Player::isShieldActive() const
{
	return m_shield.isActive();
}

void Player::setShieldActive(bool isActive)
{
	m_shield.setActive(isActive);
}

void Player::shieldDamage(double damage)
{
	m_shield.damage(damage);
}

void Player::addEnhancePoint(double addPoint)
{
	m_enhancePoint += addPoint;
}

HashTable<ItemType, int32> Player::getItemCollection() const
{
	return m_itemCollection;
}

void Player::addOnePointUpgrade(ItemType itemType)
{
	m_itemCollection[itemType]++;
}

void Player::removeItem(ItemType itemType, int32 value)
{
	m_itemCollection[itemType] -= value;
}

void Player::resetUpgrade(ItemType itemType)
{
	m_itemCollection[itemType] = 0;
}

HashTable<ItemType, int32> Player::getUpgradeCnt() const
{
	return m_itemCollection;
}

bool Player::isEnhanced()
{
	return m_isEnhanced;
}

void Player::resetEnhancePoint()
{
	m_enhancePoint = 0;
}
