#include "Shared_UI.hlsli"

// 픽셀 셰이더

float4 main(VS_INPUT input) : SV_TARGET
{
    // DeltaTime
    float currentTime = deltaTime;
    float FadeDuration = 2.0; // FadeIn 및 FadeOut 지속 시간 (초)    

    // FadeIn 및 FadeOut 효과를 계산합니다.
    float alpha = saturate(currentTime / FadeDuration); // FadeIn 시간에서 0에서 1로
   
    // FadeOut 시간에서 1에서 0로
    alpha = (currentTime > FadeDuration) ? (2.0 - alpha) : alpha;

    // Fade 색상
    float3 FadeColor = float3(0, 0, 0); // FadeIn 및 FadeOut 색상
    float4 finalColor = float4(FadeColor, alpha);
    
    return finalColor;
}