
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

cbuffer TransformW : register(b10)
{
    int PlayerNumber;
    float2 OffsetPosition;
    int IsOutLine;
}

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    output.uv = input.uv;
    output.Pos = input.Pos;
    
    if (PlayerNumber == 2)
    {
        output.Pos.x += -0.8f;
        output.Pos.y += -0.35f;
    }
    if (PlayerNumber == 1)
    {
        output.Pos.x += 0.8f;
        output.Pos.y += -0.35f;
    }
    
    return output;
}

Texture2D txUI_1_RenderMap : register(t18);
Texture2D txUI_2_RenderMap : register(t19);
SamplerState samLinear : register(s0);

float currentTime = 0.0f;

float4 PS(PS_INPUT input) : SV_TARGET
{
    float4 uiMeshTexture;
    
    if (PlayerNumber == 1)
    {
        uiMeshTexture = txUI_1_RenderMap.Sample(samLinear, input.uv);

        if (uiMeshTexture.a == 0.f && uiMeshTexture.r == 0.f && uiMeshTexture.g == 0.f && uiMeshTexture.b == 0.f)
        {
            discard;
        }
        
        if (IsOutLine == 1)
        {
            uiMeshTexture = float4(1.f, 1.f, 0.f, 1.f);
        }
        
        return uiMeshTexture;
    }
    
    if (PlayerNumber == 2)
    {
        uiMeshTexture = txUI_2_RenderMap.Sample(samLinear, input.uv);

        if (uiMeshTexture.a == 0.f && uiMeshTexture.r == 0.f && uiMeshTexture.g == 0.f && uiMeshTexture.b == 0.f)
        {
            discard;
        }
        
        if (IsOutLine == 1)
        {
            uiMeshTexture = float4(1.f, 1.f, 0.f, 1.f);
        }
        
        return uiMeshTexture;
    }
    
    return float4(1.f, 1.f, 1.f, 1.f);
}