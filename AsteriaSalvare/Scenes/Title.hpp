﻿#pragma once
#include "Common.hpp"

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

private:
	InputGroup m_input = (KeyJ | XInput(0).buttonA);
};
