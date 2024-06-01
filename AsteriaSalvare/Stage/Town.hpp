#pragma once
#include <Siv3D.hpp>
#include "StageInfo.hpp"
#include "HPBar.h"
#include "Item.hpp"

//TODO:街のタイプに合わせたアップグレードを実装する
/// @brief 街の種類
namespace TownType
{
	enum Type
	{
		Normal,
		Attack,
		Defense,
		Special
	};
}

/// @brief 街のクラス
class Town
{
public:
	Town(){}

	Town(TownType::Type tType,Circular pos):
		m_townType(tType),
		m_position(pos)
	{
		m_collider.setCenter(pos);
	}

	void update(double deltaTime)
	{
		m_attackTimer += deltaTime;
		m_hPBar.update();
		m_attackLevel = Min(5, m_upgradeItem[ItemType::AttackUpgrade] / m_attackItemsNeeded);
	}

	void shot(Array<Bullet>& bulletArr)
	{
		//発射時間の計算
		if (m_attackTimer < m_attackCoolTime)
		{
			return;
		}
		m_attackTimer -= m_attackCoolTime;


		if (m_attackLevel < 1)
		{
			return;
		}

		//弾の衝突範囲
		Circle bulletCollider{ m_position,m_bulletR };

		//発射弾数が1の時
		if (m_attackLevel == 1)
		{
			Vec2 direction = Vec2(Circular{ 1,m_position.theta }).normalized();
			bulletArr << Bullet{ BulletType::Town,0,bulletCollider,direction,m_attackValue };
			return;
		}

		//発射弾数が2以上の時
		for (int32 i : step(m_attackLevel))
		{
			double degrees = -15.0 * m_attackLevel + i * 30.0 * m_attackLevel / (m_attackLevel - 1.0);
			Vec2 direction = Vec2(Circular{ 1,ToRadians(degrees) + m_position.theta }).normalized();
			bulletArr << Bullet{ BulletType::Town,0,bulletCollider,Vec2{direction}.normalized(),m_attackValue};
		}
	}

	void drawHPBar(const RectF& rect) const
	{
		m_hPBar.draw(rect);
	}

	int32 getHP() const
	{
		return m_hPBar.getHP();
	}

	void damage(int32 damage)
	{
		m_hPBar.damage(damage);
	}

	TownType::Type getTownType() const
	{
		return m_townType;
	}

	double getBulletDamage() const
	{
		return m_attackValue;
	}

	void addUpgrade(HashTable<ItemType, int32> upgradeItem, ItemType itemType)
	{
		m_upgradeItem[itemType] += upgradeItem[itemType];
	}

	/// @brief 衝突範囲を返します
	/// @return 衝突範囲
	Circle getCollider() const
	{
		return m_collider;
	}

private:
	/// @brief 街のCircular座標
	Circular m_position = { 0,0 };

	/// @brief 衝突判定
	Circle m_collider{ 0,0,StageInfo::townSize};

	/// @brief 街の種類
	TownType::Type m_townType = TownType::Normal;

	/// @brief 最大HP
	static constexpr int32 m_maxTownHP = 3000;

	/// @brief HPBar
	HPBar m_hPBar{ m_maxTownHP };

	/// @brief 街の強化状態
	HashTable<ItemType, int32> m_upgradeItem =
	{
		{ItemType::AttackUpgrade,0},
		{ItemType::ShieldUpgrade,0},
		{ItemType::SpecialUpgrade,0},
	};
	/// @brief 街が１度に発射する弾数(0以上5以下)
	int32 m_attackLevel = 0;

	/// @brief 攻撃アップグレードに必要なアイテム数
	int32 m_attackItemsNeeded = 3;

	/// @brief 攻撃力
	static constexpr int32 m_attackValue = 100;

	/// @brief 攻撃のクールタイム
	static constexpr double m_attackCoolTime = 1.0;

	/// @brief 前回の攻撃からの時間
	double m_attackTimer = 0;

	/// @brief 弾の半径
	static constexpr double m_bulletR = 10.0;

	/// @brief シールドの最大HP
	static constexpr double m_maxShieldHP = 100;

	/// @brief 現在のシールドHP
	double m_currentShieldHP = m_maxShieldHP;

	/// @brief 前回のフレームで経過した時間
	double m_deltaTime = 0;
};
