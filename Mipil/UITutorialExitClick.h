#pragma once
#include "../Engine/UIClickFunctor.h"


class UITutorialExitClick
    : public UIClickFunctor
{
public:
    virtual void Execute(UIObject& onwer, float deltaTime) override;

    void SetStage1Guide(bool* isGuide) { m_bStage1Guide = isGuide; }

public:
    bool* m_bStage1Guide;
};