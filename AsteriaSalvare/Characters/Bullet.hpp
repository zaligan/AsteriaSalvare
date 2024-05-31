# pragma once
# include <Siv3D.hpp>

//弾の種類を表します
enum class BulletType
{
	//プレイヤーの普通の弾
	Normal,

	//強化時のプレイヤーの弾
	Enhanced,

	//敵の弾
	Enemy,

	//街の弾
	Town
};

/// @brief 敵とプレイヤーの弾のクラスです
struct Bullet
{
	//弾の種類です
	BulletType type;

	//弾の個体識別IDです
	int32 ID;

	//衝突範囲です
	Circle collider;

	// 進行方向を表す単位ベクトルです
	Vec2 direction;

	//この弾のダメージです
	int32 damage;
};

/// @brief プレイヤーの弾情報です
namespace PlayerBullet
{
	static constexpr double size = 6.0;
	static constexpr double speed = 400.0;
	static constexpr double enhancedBulletSize = 9.0;
}

/// @brief 敵の弾情報です
namespace EnemyBullet
{
	static constexpr double size = 4.0;
	static constexpr double speed = 40.0;
}

