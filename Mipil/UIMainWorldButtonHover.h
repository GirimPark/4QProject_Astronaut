#pragma once
#include "../Engine/UIHoverFunctor.h"

/// <summary>
/// MainWorld에 있는 버튼 세개의 Hover에 대한 Functor
/// </summary>
class UIMainWorldButtonHover :
    public UIHoverFunctor
{
public:
    virtual void ExecuteHovered(UIObject& onwer, float deltaTime) override;
    virtual void ExecuteHoverOut(UIObject& onwer, float deltaTime) override;

    void SetHoverTexturePath(std::wstring_view texturePath) { m_hoverTexturePath = texturePath; }
private:
    std::wstring_view m_hoverTexturePath;
    bool m_hovering = false;
};
