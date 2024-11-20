#pragma once
#include "RenderComponent.h"

class EnvironmentModel;
/// <summary>
///	Environment Cube를 렌더링하기 위한 컴포넌트
///	Renderer의 EnvironmentModel을 만들고 소유한다.
/// </summary>
class EnvironmentComponent
	: public RenderComponent
{
	// transform 업데이트 옮기기
	// 파일 이름 설정 옮기기
	// renderer의 m_pEnvironment에 set 해주기

public:
	EnvironmentComponent() = default;
	~EnvironmentComponent() override;

public:
	std::shared_ptr<EnvironmentModel> GetEnvironmentModel() const { return m_environmentModel; }

	// CreateComponent 후 필수적으로 설정할 filePath들
	// fbx file
	void SetSceneFilePath(std::string_view filePath) { m_filePath = filePath; }
	// dds file
	void SetEnvironmentTexturePath(std::wstring_view filePath) { m_environmentTexturePath = filePath; }
	void SetIBLSpecularTexturePath(std::wstring_view filePath) { m_IBLSpecularTexturePath = filePath; }
	void SetIBLDiffuseTexturePath(std::wstring_view filePath) { m_IBLDiffuseTexturePath = filePath; }
	void SetIBLBRDFTexturePath(std::wstring_view filePath) { m_IBLBRDFTexturePath = filePath; }

public:
	void Initialize() override;	// staticMeshModel을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;


private:
	std::string m_filePath;
	std::wstring m_environmentTexturePath;
	std::wstring m_IBLSpecularTexturePath;
	std::wstring m_IBLDiffuseTexturePath;
	std::wstring m_IBLBRDFTexturePath;

	std::shared_ptr<EnvironmentModel> m_environmentModel;
};