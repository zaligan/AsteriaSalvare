# pragma once
# include <Siv3D.hpp>

enum class ItemType
{
	AttackUpgrade,
	ShieldUpgrade,
	SpecialUpgrade
};

struct StageItem
{
	Circular position;
	ItemType itemType;
};
