#pragma once

#include "PuzzleWorld.h"

class Stage2World
	: public PuzzleWorld
{
public:
	Stage2World();
	~Stage2World() override;

	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnEnter() override;
	void OnExit() override;
};

