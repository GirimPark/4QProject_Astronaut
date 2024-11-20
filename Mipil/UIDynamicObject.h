#pragma once
#include "../Engine/UIObject.h"

class UIDynamicObject
	: public UIObject
{
public:
    UIDynamicObject(const std::wstring& name) : UIObject(name) {};
    virtual ~UIDynamicObject() {};

    virtual void Initialize();
    virtual void Update(float deltaTime);
};

