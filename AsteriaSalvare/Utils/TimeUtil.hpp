#pragma once

namespace Util
{
	void updateTimeUtil();

	double getDeltaTime();

	double getSceneTime();

	//TODO:シーン切り替えで自動に出来たら楽だなぁ～
	void startSceneTimeCounter();

	void stopSceneTimeCounter();

	void resetSceneTime();

	void pauseDeltaTime();

	void resumeDeltaTime();

	void setTimeScale(double scale);

	double getTimeScale();
}
