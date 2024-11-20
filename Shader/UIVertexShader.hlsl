
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

cbuffer TransformW : register(b2)
{
    matrix World;
}

cbuffer TransformVP : register(b3)
{
    matrix View;
    matrix Projection;
}

cbuffer UIAnimationKeyframe : register(b6)
{
    float2 Offset;
    float2 Size;
    float2 TextureSize;
    float AnimationTime;
    int UseAnimation;
}

cbuffer DeltaTime : register(b7)
{
    float deltaTime;
    int UseFade;
    float pad[2];
}

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    output.Pos = mul(input.Pos, World);
   // output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.uv = input.uv;
    
    // UI 애니메이션 사용시
    if (UseAnimation)
    {
        output.uv *= Size / TextureSize;
        output.uv += Offset / TextureSize;
    }

    return output;
}