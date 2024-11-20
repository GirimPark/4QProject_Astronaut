#pragma once
#include "PuzzleWorld.h"

class Stage5World
	: public PuzzleWorld
{
public:
	Stage5World();
	~Stage5World() override;

	void Initialize() override;
	void Update(float deltaTime) override;


public:
	void OnEnter() override;
	void OnExit() override;
};