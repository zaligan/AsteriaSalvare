#pragma once
#include <Siv3D.hpp>
#include "Anime.hpp"

//前方宣言
class Player;

class Shield
{
public:
	
	Shield(Player& player);

	/// @brief シールドを更新します
	/// @param useShield trueの時シールドを展開します
	void update();

	/// @brief シールドを描画します
	/// @param pos 描画する位置です
	void draw() const;

	/// @brief シールドを展開します
	/// @param isActive trueの時シールドを展開して、falseの時シールドを収納します
	void setActive(bool isActive);

	/// @brief シールドが使用中か返します
	/// @return シールドを使っていたら true
	bool isActive() const;

	/// @brief シールドの衝突範囲を返します
	/// @return シールドの衝突範囲
	Circle getCollider();

	/// @brief シールドにダメージを与えます
	/// @param damage 受けるダメージ量です
	void damage(double damage);

	/// @brief シールドを回復します
	/// @param heal 回復量です
	void shieldRestoreHP(double heal);

private:

	/// @brief シールドを持つプレイヤーの参照です
	Player& m_player;

	/// @brief シールドのサイズです
	static constexpr double m_size = 1.0;

	/// @brief true の時シールドを展開し、false の時シールドを収納します
	bool m_isActive = false;

	/// @brief 1フレーム前のシールドの展開状態です
	bool m_wasActive = false;

	/// @brief シールドアイテム１つあたりの耐久値です
	double m_durabilityPerItem = 1000.0;

	/// @brief シールドの耐久値です
	double m_currentHP = 0;

	//TODO:pbriefを追加
	//シールド使用時のアニメーションです
	Anime m_shieldUseAnimation{ TextureAsset(U"shield"), 4, 5, 0.03, m_size * 0.18 };

	//シールドの衝突範囲です
	Circle m_collider{ m_size * 30.0 };

	//アニメーションの位置と衝突範囲の調整用
	Circular m_animePositionOffset{ 3,0 };
};
