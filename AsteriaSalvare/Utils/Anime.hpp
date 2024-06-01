#pragma once
#include <Siv3D.hpp>

/// @brief スプライトシートを用いてアニメーションを表示します
class Anime
{
public:
	/// @brief Animeを作成します
	Anime(const Texture& texture, int32 rowSize, int32 majorSize, double frmTime) :
		Anime(texture, rowSize, majorSize, frmTime, 1.0) {}

	/// @brief サイズを変更したAnimeを作成します
	Anime(const Texture& texture, int32 rowSize, int32 majorSize, double frmTime, double resize) :
		Anime(Point(majorSize-1,rowSize-1),texture,rowSize,majorSize,frmTime,resize) {}

	/// @brief ループ再生するAnimeを作成します
	/// @param reStartIndex ループ再生の開始場所です
	/// @param texture 表示するテクスチャです
	/// @param rowSize textureが持つコマの行数です
	/// @param majorSize textureが持つコマの列数です
	/// @param frmTime １コマの描画時間です(秒)
	/// @param resize テクスチャの表示倍率です
	Anime(Point reStartIndex,const Texture& texture, int32 rowSize, int32 majorSize, double frmTime, double resize) :
		m_reStartIndex(reStartIndex),
		m_texture(texture),
		m_majorSize(majorSize),
		m_rowSize(rowSize),
		m_frmTime(frmTime),
		m_resize(resize),
		m_index({ 0,0 }) {}

	/// @brief テクスチャのアニメーション、効果音の再生を行います。
	/// @return アニメーションの再生が終わると true を返します
	bool update()
	{
		if (m_index.x == m_majorSize - 1 && m_index.y == m_rowSize-1)
		{
			m_index = m_reStartIndex;
			return true;
		}

		stopwatch.start();
		const double time = stopwatch.sF();

		if (time > m_frmTime)
		{
			stopwatch.restart();
			++m_index.x;

			if (m_index.x >= m_majorSize)
			{
				m_index.x = 0;
				++m_index.y;
			}
		}
		return false;
	}

	/// @brief アニメーションを１コマ目までリセットします
	void reset()
	{
		m_index = { 0,0 };
	}

	/// @brief アニメーションを描画します
	/// @param pos 描画する座標です
	void drawAt(const Vec2& pos) const
	{
		m_texture.uv(static_cast<double>(m_index.x) / m_majorSize, static_cast<double>(m_index.y) / m_rowSize, 1.0 / m_majorSize, 1.0 / m_rowSize).scaled(m_resize).drawAt(pos);
	}

	/// @brief 回転したアニメーションを描画します
	/// @param pos 描画する座標です
	/// @param theta 回転させる角度です
	void drawAt(const Vec2& pos,double theta) const
	{
		m_texture.uv(static_cast<double>(m_index.x) / m_majorSize, static_cast<double>(m_index.y) / m_rowSize, 1.0 / m_majorSize, 1.0 / m_rowSize).scaled(m_resize).rotated(theta).drawAt(pos);
	}

private:

	/// @brief スプライトシートです
	Texture m_texture;

	/// @brief スプライトシートのコマの列数です
	int32 m_majorSize = 0;

	/// @brief スプライトシートのコマの行数です
	int32 m_rowSize = 0;

	/// @brief １コマの描画時間です(秒)
	double m_frmTime = 0;

	/// @brief テクスチャの表示倍率です
	double m_resize = 1.0;

	/// @brief 表示するコマの{列、行}です
	Point m_index = { 0,0 };

	/// @brief ループ再生の開始場所です
	Point m_reStartIndex = { 0,0 };

	/// @brief アニメーションの経過時間を計測します
	Stopwatch stopwatch{ StartImmediately::No };
};
