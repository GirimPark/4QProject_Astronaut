#include "pch.h"
#include "UIClickFunctor.h"

#include "SoundManager.h"

void UIClickFunctor::Execute(UIObject& onwer, float deltaTime)
{
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Button_Click.mp3");
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Button_Click.mp3");
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/Effect/Button_Click.mp3", 0.5f);
}
