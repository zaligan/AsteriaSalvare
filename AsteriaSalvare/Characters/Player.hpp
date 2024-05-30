#pragma once
#include <Siv3D.hpp>
#include "Anime.hpp"
#include "Bullet.hpp"
#include "Shileld.hpp"
#include "StageInfo.hpp"
#include "Item.hpp"
#include "Volume.hpp"

class Player
{
public:
	/// @brief プレイヤーを作成します
	Player();

	/// @brief フレーム時間に合わせて更新します
	/// @param deltaTime フレーム時間を渡します
	void update(double deltaTime);

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
	/// @param damege 受けるダメージ量です
	void damage(double damege);

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

	Circle getShieldCollider();

	/// @brief シールドが使用中か返します
	/// @return シールドを使っていたら true
	bool isShieldActive() const;

	/// @brief シールドの展開、収納を行います
	/// @param isActive true の時シールドを展開、false の時シールドを収納します
	void setShieldActive(bool isActive);

	void shieldDamage(double damage);

	void addEnhancePoint(double addPoint);

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

	/// @brief 所持アップグレード数をすべて0にする
	void resetUpgrade();


	/// @brief 所持アップグレード数を返します
	/// @return 所持アップグレード数
	HashTable<ItemType, int32> getUpgradeCnt() const;

	bool isEnhanced();


	//TODO:関数名と引数名が適切か確認
	void setEnhancePoint(int32 point);

	//TODO:関数名と引数名が適切か確認
	void resetEnhancePoint();

private:

	Shield m_shield;

	//１フレームの時間です
	double m_deltaTime = 0;

	//プレイヤーの中心座標(半径、回転角)です
	Circular m_position{ StageInfo::stageRadius,0 };

	//プレイヤーの衝突範囲です
	Circle m_collider{ 0,0,m_playerSize * 10 };

	//上下方向のプレイヤーが動ける範囲
	struct MoveRange
	{
		double minRadius;
		double maxRadius;
	};
	static constexpr MoveRange m_moveRange{ 100,StageInfo::stageRadius + 200 };

	//上下方向の移動速度です
	static constexpr double m_vertSpeed = 200.0;

	//左右方向の移動速度です,MoveRangeによって変化
	//プレイヤーが,minRadiusにいる時１周にかかる秒数
	static constexpr double m_maxRotateSpeed = 3.0;
	//プレイヤーが,maxRadiusにいる時１周にかかる秒数
	static constexpr double m_minRotateSpeed = 18.0;

	/// @brief プレイヤーの大きさ
	static constexpr double m_playerSize = 1.3;

	//プレイヤーの最大体力です
	static constexpr double m_maxHP = 1.0;

	//プレイヤーの現在の体力です
	double m_currentHP = m_maxHP;

	//射撃間隔の時間(秒)です
	static constexpr double m_shotCoolTime = 0.5;

	//射撃してからの時間を計ります
	double m_shotTimer = 0.0;

//-------強化状態--------------------------

	//強化状態のとき,true
	bool m_isEnhanced = false;

	//シールドで吸収した強化値です
	double m_enhancePoint = 0;

	//m_enhancePointがこの値を超えると強化状態
	double m_enhanceThreshold = 30;

	//m_enhancePointがこの値を超えるとシールド全回復
	double m_shieldRestoreThreshold = 50;

	//m_enhancePointが,shieldRestoreThresholdを超えた回数
	int32 m_shieldRestoreCnt = 0;

	//強化状態で発射した時に減るm_enhancePointの量
	static constexpr double m_shotLostEnhancePoint = 10.0;

	//プレイヤー強化時のエフェクトです
	Anime m_enhanceEffectAnime{ Point(4,1), TextureAsset(U"enhancedEffect"), 3, 5, 0.04, 0.5 };

	//強化モード時の射撃間隔(秒)です
	double m_enhancedShotCoolTime = 0.15;

	//アップグレードアイテム
	HashTable<ItemType,int32> m_itemCollection =
	{
		{ItemType::AttackUpgrade,10},
		{ItemType::ShieldUpgrade,10},
		{ItemType::SpecialUpgrade,10},
	};
//-------弾--------------------

	//通常時の弾の威力
	static constexpr double m_bulletDamage = 10.0;

	//強化時の弾の威力
	static constexpr double m_enhancedBulletDamage = 20.0;

	//強化した弾のIDです
	int32 m_enhancedBulletID = 0;

};
