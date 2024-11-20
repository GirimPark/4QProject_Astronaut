#pragma once

struct AnimationKey
{
	double m_Time;
	Math::Vector3 m_Position;
	Math::Quaternion m_Rotation;
	Math::Vector3 m_Scaling;
};

class NodeAnimation
{
public:
	std::string m_NodeName;
	std::vector<AnimationKey> m_AnimationKey;

	bool* m_bIsLoop;
	int frameIndex = 0;

public:
	Math::Matrix  Evaluate(float progressTime);	

	// 구면 보간 함수
	Math::Quaternion SphericalInterpolation(const Math::Quaternion& start, const Math::Quaternion& end, float factor);

	void Create(aiNodeAnim* nodeAnimation, double tickPerSecond);
};

class Animation
{
public:
	Animation() {};
	~Animation() {};

	void Create(const std::string strFBXFilePath, const aiAnimation* pAiAnimation);
	void Create(const std::string strFBXFilePath);

public:
	std::vector<NodeAnimation> m_NodeAnims;
	std::string FilePath;
	std::string Name;

	bool m_animLoop = true;
	double m_Duration = 0;
	double m_TicksPerSecond = 0;
};

// Animation
// 하나의 애니메이션 클립 
// 
// NodeAnimation
// 애니메이션 클립 내에서 모델의 특정 뼈에 대한 애니메이션
// 
// AnimationKey
// 애니메이션 클립 내에서 뼈의 변환 정보를 저장하고 관리
//
// mChannels 
// 모델의 뼈에 대한 애니메이션 동작 정보
//
//