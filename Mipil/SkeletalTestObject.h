#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/InputManager.h"

class SkeletalMeshComponent;

class SkeletalTestObject
	: public GameObject
	, public InputProcesser
{
public:
	~SkeletalTestObject();

	void Initialize() override;
	void Update(float deltaTime) override;

	void OnInputProcess(
		const Keyboard::State& KeyState,
		const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState,
		const Mouse::ButtonStateTracker& MouseTracker) override;

private:
	std::weak_ptr<SkeletalMeshComponent> m_meshComponent;
};

