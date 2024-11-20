#pragma once
#include "../Engine/UIObject.h"

class UICameraMove 
    : public UIObject
{
public:
    UICameraMove(const std::wstring& name) : UIObject(name) {};
    virtual ~UICameraMove() {};

    virtual void Initialize();
    virtual void Update(float deltaTime);
};

