#pragma once
#include "RenderComponent.h"

class StaticMeshModel;

/// <summary>
///	StaticMesh를 렌더링하기 위한 컴포넌트
///	Renderer의 StaticMeshModel을 만들고 소유한다.
/// </summary>
class StaticMeshComponent
	: public RenderComponent
{
public:
	StaticMeshComponent() = default;
	~StaticMeshComponent() override;

public:
	std::shared_ptr<StaticMeshModel> GetStaticMeshModel() const { return m_staticMeshModel; }

	// CreateComponent 후 필수적으로 filePath 설정
	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }
	void SetOutLine(bool isOutLine);

	Math::Vector3 GetModelDimensions() const;

public:
	void Initialize() override;	// staticMeshModel을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;


private:
	std::string m_filePath;
	std::shared_ptr<StaticMeshModel> m_staticMeshModel;
};

