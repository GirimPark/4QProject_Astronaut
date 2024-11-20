#pragma once
#include "PuzzleWorld.h"

class Stage1World
    : public PuzzleWorld
{
public:
	Stage1World();
	~Stage1World() override;

	void Initialize() override;
	void Update(float deltaTime) override;


public:
	void OnEnter() override;
	void OnExit() override;

private:
	int m_GuideNumber;
};

