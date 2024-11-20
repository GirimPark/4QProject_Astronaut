#pragma once

class Texture;
class UIMesh;

struct CB_FadeInOut
{
	float fadeDeltaTime = 0.0f;
	int IsDim = false;
	int UseDim = false;
	int IsFade = false;
	int UseFadeOut = false;
	int UseFadeIn = false;
	float durationTime = 0.0f;
	float pad;
};

struct CB_UIAnimationKeyframe
{
	Vector2 m_Offset = Vector2{ 0.f, 0.f };
	Vector2 m_Size = Vector2{ 0.f, 0.f };
	Vector2 m_TextureSize = Vector2{ 0.f, 0.f };
	float m_AnimationTime;
	int m_UseAnimation;
};

class UIInstance
{
public:
	UIInstance();
	~UIInstance();

public:
	// 건재 : 애니메이션 루프 Get & Set 추가 및 애니메이션 키 프레임 추가함수 추가
	bool GetIsLoop() { return m_bIsLoop; }
	void SetIsLoop(bool isloop) { m_bIsLoop = isloop; }
	void SetIsRender(bool isRender) { m_bIsRender = isRender; }
	Math::Matrix* GetWorldMatrix() { return m_pWorldTransform; }
	bool GetIsRender() { return m_bIsRender; }
	void NotLoopAnimationStart() { if (!m_bIsLoop) { m_nCurrentAnimationIndex = 0; m_DurationTime = 0; m_bIsOneCycle = false; } }

	void AddKeyframe(CB_UIAnimationKeyframe keyframe) { m_KeyFrame.push_back(keyframe); }
	void ClearKeyFrame() { m_KeyFrame.clear(); }
	int GetKeyFrameSize() { return m_KeyFrame.size(); }
	
	// Fade In Out Data
	void SetIsFade() {m_FadeInOut.IsFade = true; }
	void SetFadeDurationTime(float durationTime) {  m_FadeInOut.durationTime = durationTime; }
	void FadeOutStart()
	{
		if (m_FadeInOut.UseFadeIn == false)
		{
			m_FadeInOut.UseFadeOut = true;
			m_bIsPlayingFade = true;
		}
	}
	void FadeInStart()
	{
		if (m_FadeInOut.UseFadeOut == false)
		{
			m_FadeInOut.UseFadeIn = true;
			m_bIsPlayingFade = true;
		}
	}

public:
	void LoadTextureResource(std::wstring_view texturePath);
	void Create(Math::Matrix* pWorld, Math::Vector2 size);
	void Update(float deltaTime);
	bool UpdateFadeInOut(float deltaTime);	
	void Render(ID3D11DeviceContext* deviceContext);

	void UIAnimationPlay(CB_UIAnimationKeyframe& animationKeyFrame);
	void UIFadeInOut(CB_FadeInOut& fadeInOut);

	void SetIsDim() { m_FadeInOut.IsDim = true; }
	void SetUseDim(bool isUse) { m_FadeInOut.UseDim = isUse; }
	bool GetDimState() { return m_FadeInOut.UseDim ; }

private:
	bool m_bIsRender;

	Math::Matrix* m_pWorldTransform = nullptr;
	std::shared_ptr<UIMesh> m_UIMesh;
	std::shared_ptr<Texture> m_Texture;

	bool m_bIsLoop;
	bool m_bIsOneCycle;
	float m_DurationTime;
	float m_AnimationTime;
	int m_nCurrentAnimationIndex;
	std::vector<CB_UIAnimationKeyframe> m_KeyFrame;

	// 재현
	float m_FadeDelTaTime = 0.0f;  // for CB
	bool m_bIsPlayingFade = false;
	CB_FadeInOut m_FadeInOut;
};