#pragma once
#include "Component.h"
#include "InputManager.h"
#include "Controller.h"
#include <type_traits>
/// <summary>
/// ControllerComponent
/// 조작 필요한 게임오브젝트에다 달아주면 됨.
/// 달고 CreateController<네가 쓰고 싶은 인풋 처리 로직 클래스>()호출해주면 된다.
/// 네가 쓰고 싶은 인풋 처리 로직 클래스는 Controller 클래스 상속시켜서 만들어서 쓰셈~
/// 현재 활성화 돼 있는 월드에 있는 오브젝트의 ControllerComponent만 인풋처리랑 업데이트 돌아감.
/// </summary>

class ControllerComponent :
    public Component , public InputProcesser
{
public:
    template <class T>
        requires std::is_base_of<Controller, T>::value
    void CreateController();

    void ClearControllers();

    virtual void OnInputProcess(
        const Keyboard::State& KeyState,
        const Keyboard::KeyboardStateTracker& KeyTracker,
        const Mouse::State& MouseState,
        const Mouse::ButtonStateTracker& MouseTracker) final;

public:
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;

public:
    ~ControllerComponent();

private:
    bool m_IsUpdate = false;
    std::list<std::unique_ptr<Controller>> m_InputOperations;
};

template<class T>
    requires std::is_base_of<Controller, T>::value
inline void ControllerComponent::CreateController()
{
    std::unique_ptr<Controller> controller = std::make_unique<T>();
    controller->SetOwner(GetOwner());
    m_InputOperations.push_back(std::move(controller));
}
