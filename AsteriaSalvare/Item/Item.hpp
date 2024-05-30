# pragma once
# include <Siv3D.hpp>

/// @brief アイテムの種類を表します
enum class ItemType
{
	AttackUpgrade,
	ShieldUpgrade,
	SpecialUpgrade
};

/// @brief ステージ上のアイテムタイプと位置を表します
struct StageItem
{
	Circular position;
	ItemType itemType;
};
