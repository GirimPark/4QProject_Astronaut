#pragma once
#include "UIObject.h"
class UIFunctorPopUp :
    public UIObject
{
public:
    UIFunctorPopUp(const std::wstring& name);
    virtual ~UIFunctorPopUp();

    virtual void Initialize();
    virtual void Update(float deltaTime);
};

