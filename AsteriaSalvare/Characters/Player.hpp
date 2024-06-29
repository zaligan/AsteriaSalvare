#pragma once
#include <Siv3D.hpp>
#include "Anime.hpp"
#include "Bullet.hpp"
#include "Shileld.hpp"
#include "StageInfo.hpp"
#include "Item.hpp"
#include "Volume.hpp"

/// @brief プレイヤーを表すクラスです
class Player
{
public:
	/// @brief プレイヤーを作成します
	Player();

	/// @brief フレーム時間に合わせて更新します
	void update();

	/// @brief プレイヤーを描画します
	void draw() const;

	/// @brief 入力に応じてプレイヤーを移動します
	/// @param directions 入力されている方向を表すパラメータです
	void move(Vec2 moveInput);

	/// @brief 弾を発射します
	/// @param bulletArr プレイヤーの弾を管理する配列です
	void shot(Array< Bullet >& bulletArr);
	
	/// @brief 現在のHPを返します
	/// @return 現在のHPです
	double getHP() const;

	/// @brief プレイヤーにダメージを与えます
	/// @param damage 受けるダメージ量です
	void damage(double damage);

	/// @brief プレイヤーの中心座標をVec2で返します
	/// @return Vec2型のプレイヤー中心座標
	Vec2 getCenter() const;

	/// @brief プレイヤーの中心座標をCircularで返します
	/// @return Circular型のプレイヤー中心座標
	Circular getCircular() const;

	/// @brief マップ中央からプレイヤーの距離を返します
	/// @return マップ中央からプレイヤーの距離
	double getR() const;

	/// @brief プレイヤーの回転角度を返します
	/// @return プレイヤーの回転角度
	double getTheta() const;

	/// @brief プレイヤーの衝突範囲を返します
	/// @return プレイヤーの衝突範囲
	Circle getCollider() const;

	/// @brief シールドの衝突範囲を返します
	/// @return シールドの衝突範囲
	Circle getShieldCollider();

	/// @brief シールドが使用中か返します
	/// @return シールドを使っていたら true
	bool isShieldActive() const;

	/// @brief シールドの展開、収納を行います
	/// @param isActive true の時シールドを展開、false の時シールドを収納します
	void setShieldActive(bool isActive);

	/// @brief シールドにダメージを与えます
	/// @param damage ダメージ量
	void shieldDamage(double damage);

	/// @brief ダメージ量を強化値に換算して加算します
	/// @param damage 受けたダメージ量
	void addEnhancePoint(double damage);

	/// @brief 所持アイテム数を返します
	/// @return 所持しているアイテム数
	HashTable<ItemType, int32> getItemCollection() const;

	/// @brief アップグレードアイテムの所持数を加算
	/// @param itemType アップグレードの種類
	void addOnePointUpgrade(ItemType itemType);

	/// @brief 指定したアイテムを減らします
	/// @param itemType 減らすアイテムの種類
	/// @param value 減らす数
	void removeItem(ItemType itemType,int32 value);

	/// @brief アップグレードアイテムをリセットします
	/// @param itemType リセットするアップグレードの種類
	void resetUpgrade(ItemType itemType);


	/// @brief 所持アップグレード数を返します
	/// @return 所持アップグレード数
	HashTable<ItemType, int32> getUpgradeCnt() const;

	bool isEnhanced();

	double getEnhancePoint() const;

	double getMaxEnhancePoint() const;

	void resetEnhancePoint();

private:

	/// @brief シールドです
	Shield m_shield;

	/// @brief プレイヤーの中心座標(半径、回転角)です
	Circular m_position{ StageInfo::stageRadius,0 };

	/// @brief プレイヤーの衝突範囲です
	Circle m_collider{ 0,0,m_playerSize * 10 };

	/// @brief 上下方向のプレイヤーが動ける範囲
	struct MoveRange
	{
		double minRadius;
		double maxRadius;
	};

	/// @brief プレイヤーが動ける範囲です
	static constexpr MoveRange m_moveRange{ 100,StageInfo::stageRadius + 200 };

	/// @brief 上下方向の移動速度です
	static constexpr double m_vertSpeed = 200.0;

	/// @brief 左右方向の最大移動速度です,プレイヤーが,minRadiusにいる時１周にかかる秒数
	static constexpr double m_maxRotateSpeed = 3.0;

	/// @brief 左右方向の最小移動速度です,プレイヤーが,maxRadiusにいる時１周にかかる秒数
	static constexpr double m_minRotateSpeed = 18.0;

	/// @brief プレイヤーの大きさ
	static constexpr double m_playerSize = 1.3;

	/// @brief プレイヤーの最大体力です
	static constexpr double m_maxHP = 1.0;

	/// @brief プレイヤーの現在の体力です
	double m_currentHP = m_maxHP;


//----------------強化状態----------------

	/// @brief プレイヤーが強化状態のとき true
	bool m_isEnhanced = false;

	/// @brief シールドで吸収した強化値です
	double m_enhancePoint = 0;

	/// @brief m_enhancePointがこの値を超えると強化状態
	static constexpr double m_enhanceThreshold = 30;

	/// @brief 強化値の最大値です
	static constexpr double m_maxEnhancePoint = 60;

	/// @brief シールドで受けたダメージ量を強化値に換算する割合です
	static constexpr double m_damageToEnhanceRate = 0.1;

	/// @brief 攻撃１発ごとに減る強化値の量です
	static constexpr double m_shotLostEnhancePoint = 5.0;

	/// @brief プレイヤー強化時のエフェクトです
	Anime m_enhanceEffectAnime{ Point(4,1), TextureAsset(U"enhancedEffect"), 3, 5, 0.04, 0.5 };

	/// @brief 所持しているアイテム一覧です
	HashTable<ItemType,int32> m_itemCollection =
	{
		{ItemType::AttackUpgrade,0},
		{ItemType::ShieldUpgrade,0},
		{ItemType::SpecialUpgrade,0},
	};

//----------------弾----------------

	/// @brief 通常時の弾の威力
	static constexpr int32 m_bulletDamage = 10;

	/// @brief 通常時の射撃クールタイム(秒)です
	static constexpr double m_shotCoolTime = 0.5;

	/// @brief 強化時の弾の威力
	static constexpr int32 m_enhancedBulletDamage = 20;

	/// @brief 強化時の射撃クールタイム(秒)です
	double m_enhancedShotCoolTime = 0.15;

	/// @brief 射撃クールタイムを計測します
	double m_shotTimer = 0.0;

	/// @brief 強化した弾の個別IDです
	int32 m_enhancedBulletID = 0;

};
