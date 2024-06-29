#include "Shileld.hpp"
#include "Player.hpp"

Shield::Shield(Player& player) : m_player(player) {}

void Shield::update()
{
	//耐久値が０以下の時アイテム消費をして回復
	if (m_currentHP <= 0)
	{
		if (m_player.getItemCollection()[ItemType::ShieldUpgrade] > 0)
		{
			m_player.removeItem(ItemType::ShieldUpgrade, 1);
			m_currentHP += m_durabilityPerItem;
		}
		else
		{
			m_currentHP = 0;
		}
	}

	//シールドを解除したフレームならアニメーションをリセット
	if (not m_isActive && m_wasActive)
	{
		m_shieldUseAnimation.reset();
	}

	m_collider.setCenter(m_player.getCenter());

	if (m_isActive)
	{
		m_shieldUseAnimation.update();
	}
}

void Shield::draw()	const
{
	if (not m_isActive)
	{
		return;
	}

	double hpRate = static_cast<double>(m_currentHP) / m_durabilityPerItem;
	m_collider.draw(ColorF(HSV{ 244 * hpRate, 0.9, 1 }, 0.7 ));
	m_shieldUseAnimation.drawAt(Circular(m_player.getCircular().r + m_animePositionOffset.r, m_player.getCircular().theta + m_animePositionOffset.theta), m_player.getCircular().theta);
}

void Shield::setActive(bool isActive)
{
	m_wasActive = m_isActive;

	//耐久値が０以下ならシールドを展開できない
	if (m_currentHP <= 0)
	{
		m_isActive = false;
		return;
	}

	m_isActive = isActive;
}

bool Shield::isActive() const
{
	return m_isActive;
}



Circle Shield::getCollider()
{
	return m_collider;
}

void Shield::damage(double damage)
{
	m_currentHP -= damage;
}

void Shield::shieldRestoreHP(double heal)
{
	m_currentHP += heal;
}




