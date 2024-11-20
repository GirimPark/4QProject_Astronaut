#pragma once
#include "../Engine/GameObject.h"

class MapObject :
    public GameObject
{
public:
	MapObject() = default;
	~MapObject() = default;

public:
	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }

public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::string m_filePath;
};

