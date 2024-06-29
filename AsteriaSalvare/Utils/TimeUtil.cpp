#pragma once
#include <Siv3D.hpp>
#include "TimeUtil.hpp"

namespace Util
{
	namespace
	{
		double s_deltaTime{};

		double s_sceneTime{};

		bool s_isPause = false;

		bool s_isSceneTimeCounting = false;

		double s_timeScale = 1.0;
	}

	void updateTimeUtil()
	{
		if (s_isPause)
		{
			s_deltaTime = 0;
			return;
		}
		s_deltaTime = Scene::DeltaTime() * s_timeScale;

		if (s_isSceneTimeCounting)
		{
			s_sceneTime += s_deltaTime;
		}
	}

	double getDeltaTime()
	{
		return s_deltaTime;
	}

	double getSceneTime()
	{
		return s_sceneTime;
	}

	void startSceneTimeCounter()
	{
		s_isSceneTimeCounting = true;
	}

	void stopSceneTimeCounter()
	{
		s_isSceneTimeCounting = false;
	}

	void resetSceneTime()
	{
		s_sceneTime = 0;
	}

	void pauseDeltaTime()
	{
		s_isPause = true;
	}

	void resumeDeltaTime()
	{
		s_isPause = false;
	}

	void setTimeScale(double scale)
	{
		s_timeScale = scale;
		updateTimeUtil();
	}

	double getTimeScale()
	{
		return s_timeScale;
	}

}
