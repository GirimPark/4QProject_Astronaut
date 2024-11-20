#pragma once
#include "PuzzleWorld.h"

class Stage3World
	: public PuzzleWorld
{
public:
	Stage3World();
	~Stage3World() override;

	void Initialize() override;
	void Update(float deltaTime) override;


public:
	void OnEnter() override;
	void OnExit() override;
};

