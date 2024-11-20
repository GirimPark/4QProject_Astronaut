
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 uv : UV;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 uv : UV;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    //output.Pos = mul(input.Pos, World);
    //output.Pos = mul(output.Pos, Projection);
    output.Pos = input.Pos;
    output.uv = input.uv;
    
    return output;
}

Texture2D txOutLineMap : register(t15);
Texture2D txOutLineOverMap : register(t16);
Texture2D txFinalRenderMap : register(t17);
SamplerState samLinear : register(s0);

float currentTime = 0.0f;

float4 PS(PS_INPUT input) : SV_TARGET
{
    float Depth = txOutLineMap.Sample(samLinear, input.uv).r;
    float OutLine = txOutLineOverMap.Sample(samLinear, input.uv).r;
    
    if (Depth >= 1.f && OutLine < 1.f)
    {
        return float4(1.f, 1.f, 0.f, 1.f);
    }
    
    float4 finalColor = txFinalRenderMap.Sample(samLinear, input.uv);
    
    return finalColor;
}