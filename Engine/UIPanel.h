#pragma once
#include "UIObject.h"
class UIPanel 
    : public UIObject
{
public:
    UIPanel(const std::wstring& name)
        : UIObject(name) {}
    virtual ~UIPanel() {}

    virtual void Initialize();
    virtual void Update(float deltaTime);
};

