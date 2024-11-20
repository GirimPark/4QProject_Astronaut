#pragma once
#include "PuzzleWorld.h"

class Stage4World
	: public PuzzleWorld
{
public:
	Stage4World();
	~Stage4World() override;

	void Initialize() override;
	void Update(float deltaTime) override;


public:
	void OnEnter() override;
	void OnExit() override;
};

