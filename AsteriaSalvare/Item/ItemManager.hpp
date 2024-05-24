# pragma once
# include <Siv3D.hpp>
# include "Item.hpp"

class ItemManager
{
public:
	ItemManager();

	void spawnItem(Vec2 position);

	Array<Item>& getItemArray()
	{
		return m_stageItemArray;
	}
private:
	Array<Item> m_stageItemArray;
};
