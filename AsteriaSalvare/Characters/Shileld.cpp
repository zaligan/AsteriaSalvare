#include "Shileld.hpp"

Shield::Shield()
{
}
//
//void Shield::update(bool useShield)
//{
//	m_collider.setCenter(m_player.getCenter());
//
//	//シールドを解除したら
//	if (m_isActive && !useShield)
//	{
//		m_shieldUseAnimation.reset();
//
//		//非強化状態でシールドを解いたら強化ポイントリセット
//		if (!m_player.isEnhanced())
//		{
//			m_player.resetEnhancePoint();
//		}
//	}
//
//	m_isActive = useShield;
//	m_shieldUseAnimation.update();
//}
//
//void Shield::draw() const
//{
//	double colorH = (m_maxHP - m_currentHP) / m_maxHP * 110;
//	m_collider.draw(ColorF(HSV{ 250 + colorH,0.9,1 }, 0.7));
//	m_shieldUseAnimation.drawAt(Circular(m_player.getCircular().r + m_animePositionOffset.r, m_player.getCircular().theta + m_animePositionOffset.theta), m_player.getCircular().theta);
//}
