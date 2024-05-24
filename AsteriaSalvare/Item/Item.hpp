# pragma once
# include <Siv3D.hpp>

/// @brief アイテムの種類と数を管理します
struct Item
{
	int32 AttackUpgrade = 0;
	int32 ShieldUpgrade = 0;
	int32 SpecialUpgrade = 0;
};
