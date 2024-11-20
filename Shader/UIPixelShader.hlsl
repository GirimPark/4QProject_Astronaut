struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : UV;
};

cbuffer FadeCB : register(b7)
{
    float deltaTime;
    int IsDim;
    int UseDim;
    int IsFade;
    int UseFadeOut;
    int UseFadeIn;
    float durationTime = 0.0f;
    float pad;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);
SamplerState samLinear1 : register(s1);
SamplerState samLinear2 : register(s2);

float currentTime = 0.0f;

float4 main(PS_INPUT input) : SV_TARGET
{
    if (IsFade)
    {
        if (UseFadeOut)  // FadeOut
        {
        // DeltaTime
            float currentTime = deltaTime;
            float FadeDuration = durationTime; // FadeIn 및 FadeOut 지속 시간 (초)    

        // FadeIn 및 FadeOut 효과를 계산합니다.
            float alpha = saturate(currentTime / FadeDuration); // FadeIn 시간에서 0에서 1로
   
        // Fade 색상
            float3 FadeColor = float3(0, 0, 0); // FadeIn 및 FadeOut 색상
            float4 finalColor = float4(FadeColor, alpha);
    
            return finalColor;
        }
    
        if (UseFadeIn)  // FadeIn
        {
        // DeltaTime
            float currentTime = deltaTime;
            float FadeDuration = durationTime; // FadeIn 및 FadeOut 지속 시간 (초)    

        // FadeIn 및 FadeOut 효과를 계산합니다.
            float alpha = saturate(currentTime / FadeDuration); // FadeIn 시간에서 0에서 1로
   
        // Fade 색상
            float3 FadeColor = float3(0, 0, 0); // FadeIn 및 FadeOut 색상
            float4 finalColor = float4(FadeColor, 1 - alpha);
    
            return finalColor;
        }
        
        discard;
    }
    
    
    if (IsDim)
    {
        if(UseDim)
        {            
            float3 DimColor = float3(0, 0, 0);
            float4 finalColor = float4(DimColor, 0.7f); // alpha

            return finalColor;
        }
        
        discard;
    }
    
    
    float4 textureColor = txDiffuse.Sample(samLinear, input.Tex);
     
    // 배경이 회색일 경우에 픽셀을 버린다.
    // 플루트 값은 정확히 측정이 안되서 0.01오차 내로 줄였습니다. 
    // 더 나은 방법 있으면 수정좀...
    if (textureColor.r >= 0.49f && textureColor.r <= 0.51f && textureColor.g >= 0.49f && textureColor.g <= 0.51f &&textureColor.b >= 0.49f && textureColor.b <= 0.51f)
    {
        discard;
    }
    
    return textureColor;

}