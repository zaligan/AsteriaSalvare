#pragma once
#include <Siv3D.hpp>
#include "Player.hpp"

class Shield
{
public:
	
	Shield();

private:
//
//	/// @brief シールドを更新します
//	/// @param useShield trueの時シールドを展開します
//	void update(bool useShield);
//
//	/// @brief シールドを描画します
//	/// @param pos 描画する位置です
//	void draw() const;
//
//	/// @brief シールドが使用可能かチェックします
//	/// @return シールドが使用できる時true
//	bool isShieldAvailable() const
//	{
//		return m_isActive && m_currentHP > 0;
//	}
//
//	/// @brief シールドの衝突範囲を返します
//	/// @return シールドの衝突範囲
//	Circle getShieldCollider()
//	{
//		return m_collider;
//	}
//
//	/// @brief シールドにダメージを与えます
//	/// @param damage 受けるダメージ量です
//	void shieldDamage(double damage)
//	{
//		m_currentHP -= damage;
//	}
//
//	/// @brief シールドを回復します
//	/// @param heal 回復量です
//	void shieldRestoreHP(double heal)
//	{
//		m_currentHP += heal;
//		AudioAsset(U"shieldRestore").playOneShot();
//	}
//
//private:
//
//	Player& m_player;
//
//	//シールドのサイズです
//	double m_size = 1.0;
//
//	//tureの時シールドが展開されています
//	bool m_isActive = false;
//
//	//シールドの最大耐久値です
//	double  m_maxHP = 1000.0;
//
//	//シールドの現在の耐久値です
//	double m_currentHP = m_maxHP;
//
//	//TODO:仕様変更。自動回復はせず、シールドアイテムの消費
//	// 毎秒自動回復する量です
//	static constexpr double m_shieldRegenerationRate = 100.0;
//
//	//シールド使用時のアニメーションです
//	Anime m_shieldUseAnimation{ TextureAsset(U"shield"), 4, 5, 0.03, m_size * 0.18 };
//
//	//シールドの衝突範囲です
//	Circle m_collider{ m_size * 30.0 };
//
//	//アニメーションの位置と衝突範囲の調整用
//	Circular m_animePositionOffset{ 3,0 };
};
