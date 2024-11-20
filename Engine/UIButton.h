#pragma once
#include "UIObject.h"

class UIButton :
    public UIObject
{
public:
    UIButton(const std::wstring& name)
        : UIObject(name) {}
    virtual ~UIButton() {}

    virtual void Initialize();
    virtual void Update(float deltaTime);

private:
    std::string m_label;
};

