#pragma once
#include "InputManager.h"
#include "UIFunctorFactory.h"
#include "UIObject.h"

class UIManager
	: public InputProcesser
{
public:
	UIManager();
	~UIManager();

public:
	template <typename UIType>
	std::weak_ptr<UIType> GetUIObject(const std::wstring& name);

public:
	template <typename UIType, typename... FunctorTypes>
		requires std::is_base_of<UIObject, UIType>::value
	&& ((std::is_base_of<UIFunctor, FunctorTypes>::value || std::is_same<FunctorTypes, void>::value) && ...)
		auto CreateUI(const std::wstring& name);

	void Initailize();
	void Update(float deltaTime);

private:
	void ClearFocusedUI();
	void UpdateHoveredUI(float deltaTime);
	void UpdateHoverOutUI(float deltaTime);
	void UpdateClickedUI(float deltaTime);

	template <typename FunctorType>
	void ApplyFunctor(auto& uiObject);

private:
	std::unordered_map<std::wstring, std::shared_ptr<UIObject>> m_UIObjects;
	std::vector<std::weak_ptr<UIObject>> m_HoveredUIObjects;
	std::vector<std::weak_ptr<UIObject>> m_HoverOutUIObjects;
	std::vector<std::weak_ptr<UIObject>> m_ClickedUIObjects;

	// InputProcesser을(를) 통해 상속됨
	void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;
};

template <typename UIType, typename... FunctorTypes>
	requires std::is_base_of<UIObject, UIType>::value
&& ((std::is_base_of<UIFunctor, FunctorTypes>::value || std::is_same<FunctorTypes, void>::value) && ...)
inline auto UIManager::CreateUI(const std::wstring& name)
{
	auto it = m_UIObjects.find(name);
	if (it != m_UIObjects.end()) {
		return std::static_pointer_cast<UIType>(it->second);
	}

	auto uiObject = std::make_shared<UIType>(name);

	(..., (ApplyFunctor<FunctorTypes>(uiObject)));

	m_UIObjects[name] = uiObject;

	return std::static_pointer_cast<UIType>(uiObject);
}

template <typename FunctorType>
inline void UIManager::ApplyFunctor(auto& uiObject)
{
	if constexpr (!std::is_same<FunctorType, void>::value) {
		auto createdFunctor = UIFunctorFactory::GetInstance()->GetFunctor<FunctorType>();
		if constexpr (std::is_base_of<UIHoverFunctor, FunctorType>::value) {
			uiObject->SetMouseHoverFunctor(createdFunctor);
		}
		else if constexpr (std::is_base_of<UIClickFunctor, FunctorType>::value) {
			uiObject->SetMouseClickFunctor(createdFunctor);
		}
	}
}

template<typename UIType>
inline std::weak_ptr<UIType> UIManager::GetUIObject(const std::wstring& name)
{
	auto it = m_UIObjects.find(name);
	if (it != m_UIObjects.end()) {
		return std::dynamic_pointer_cast<UIType>(it->second);
	}
	return std::weak_ptr<UIType>();
}