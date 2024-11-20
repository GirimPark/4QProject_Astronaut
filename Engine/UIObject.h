#pragma once
#include "UIHoverFunctor.h"
#include "UIClickFunctor.h"

#include "../D3DRenderer/UIInstance.h"

class UIObject
	: public std::enable_shared_from_this<UIObject>
{
public:
	UIObject(const std::wstring& name);
	virtual ~UIObject();

public:
	const Math::Vector2& GetSize() const { return m_size; }
	const Math::Vector2& GetPosition() const { return m_position; }
	const Math::Vector2& GetScale() const { return m_scale; }
	const Math::Matrix& GetWorldMatrix() const { return m_worldMatrix; }
	const std::vector<std::shared_ptr<UIObject>>& GetChildren() const { return m_children; }
	std::weak_ptr<UIObject> GetParent() { return m_parent; }
	bool IsHovered() const { return m_isHovered; }

public:
	virtual void SetMouseHoverFunctor(std::weak_ptr<UIHoverFunctor> functor) {
		if (!functor.expired()) { auto lockedFunctor = functor.lock(); m_mouseHoverFunctors.push_back(std::move(lockedFunctor)); }
	}
	virtual void SetMouseClickFunctor(std::weak_ptr<UIClickFunctor> functor) {
		if (!functor.expired()) { auto lockedFunctor = functor.lock(); 
			m_mouseClickFunctors.push_back(std::move(lockedFunctor));
		}
	}

	void SetSize(const Math::Vector2& size) { m_size = size; }
	void SetPosition(const Math::Vector2& position) { m_position = position; }
	void SetScale(const Math::Vector2& scale) { m_scale = scale; }

	void SetTexturePath(std::wstring path) { m_texturePath = path; }
	std::wstring GetTexturePath() { return m_texturePath; }

	void SetVisible(bool isVisible) { m_bVisible = isVisible; }
	// 자식까지 Visible 세팅해주기
	void SetAllVisible(bool isVisible);
	bool GetVisible() { return m_bVisible; }

	void SetHovered(bool hovered) { m_isHovered = hovered; }

	// 임시 추가 : UIInstance에 KeyFrame전달해주기 위한 함수
	void AddKeyFrame(CB_UIAnimationKeyframe animationKeyFrame) { m_uiInstance->AddKeyframe(animationKeyFrame); }

	std::weak_ptr<UIInstance> GetUIInstance() { if (this == nullptr) return std::weak_ptr<UIInstance>(); return m_uiInstance; }

public:
	template<typename... Args>
	void AddChildren(Args&&... children);
	void RemoveChild(std::shared_ptr<UIObject> child);

	void ExecuteHoverFunctors(float deltaTime);
	void ExecuteHoverOutFunctors(float deltaTime);
	void ExecuteClickFunctors(float deltaTime);

	virtual void Initialize();
	virtual void Update(float deltaTime);

private:
	void SetParent(std::shared_ptr<UIObject> parent);

protected:
	std::wstring m_name;

	std::vector<std::shared_ptr<UIHoverFunctor>> m_mouseHoverFunctors;
	std::vector<std::shared_ptr<UIClickFunctor>> m_mouseClickFunctors;

	std::weak_ptr<UIObject> m_parent;
	std::vector<std::shared_ptr<UIObject>> m_children;

	bool m_bVisible = false;
	bool m_isHovered;

	Math::Vector2 m_size;
	Math::Vector2 m_position;
	Math::Vector2 m_scale;
	Math::Matrix m_worldMatrix;

	std::wstring m_texturePath;
	std::shared_ptr<UIInstance> m_uiInstance;
};

template<typename... Args>
inline void UIObject::AddChildren(Args&&... children)
{
	(m_children.push_back(std::forward<Args>(children)), ...);
	(children->SetParent(shared_from_this()), ...);
}