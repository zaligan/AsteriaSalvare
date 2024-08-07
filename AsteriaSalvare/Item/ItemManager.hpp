﻿#pragma once
#include <Siv3D.hpp>
#include "Item.hpp"
#include "TimeUtil.hpp"

using namespace Util;

/// @brief アイテムの管理を行うクラスです
class ItemManager
{
public:
	ItemManager(){}

	/// @brief ステージ上アイテムの更新を行います
	void update()
	{
		for (auto& item : m_stageItemArray)
		{
			//アイテムの移動
			if (item.position.r > StageInfo::stageRadius)
			{
				item.position.r -= itemDropSpeed * getDeltaTime();
			}
		}
	}

	/// @brief アイテムを描画します
	void draw() const
	{
		for (auto& item : m_stageItemArray)
		{

			String itemType;
			switch (item.itemType)
			{
			case ItemType::AttackUpgrade:
				itemType = U"AttackUpgrade";
				break;
			case ItemType::ShieldUpgrade:
				itemType = U"shieldUpgrade";
				break;
			case ItemType::SpecialUpgrade:
				itemType = U"specialUpgrade";
				break;
			default:
				break;
			}
			TextureAsset(itemType).scaled(0.04).rotated(item.position.theta).drawAt(OffsetCircular({ 0,0 }, item.position));
		}
	}

	/// @brief 撃破した敵からアイテムをスポーンさせる
	/// @param positions 撃破した敵の座標が入った配列
	void dropEnemyItem(const Array<Vec2>& positions)
	{
		for (auto& enemyPosition : positions)
		{
			//3種類のアイテムをそれぞれ10%の確率でドロップ
			int32 itemDropChance = Random(0, 9);
			if (3 <= itemDropChance)
			{
				continue;
			}

			ItemType dropItem{};
			switch (itemDropChance)
			{
			case 0:
				dropItem = ItemType::AttackUpgrade;
				break;
			case 1:
				dropItem = ItemType::ShieldUpgrade;
				break;
			case 2:
				dropItem = ItemType::SpecialUpgrade;
				break;
			default:
				throw Error(U"アイテムのドロップに失敗しました");
				break;
			}

			m_stageItemArray << StageItem{ enemyPosition ,dropItem };
		}
		return;
	}

	/// @brief ステージ上にあるアイテムを取得します
	/// @return ステージ上にあるアイテムの配列
	Array<StageItem>& getStageItem()
	{
		return m_stageItemArray;
	}
private:

	/// @brief ステージ上にあるアイテムの配列です
	Array<StageItem> m_stageItemArray;

	/// @brief アイテムが落下してくる速度です。
	double itemDropSpeed = 30.0;
};
