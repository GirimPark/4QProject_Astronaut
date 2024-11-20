#pragma once
#include "UIObject.h"

class UITriggerPopUp :
    public UIObject
{
public:
    UITriggerPopUp(const std::wstring& name);
    virtual ~UITriggerPopUp();

    virtual void Initialize();
    virtual void Update(float deltaTime);

    void TriggerPop();

    bool m_triggerPop;
};

