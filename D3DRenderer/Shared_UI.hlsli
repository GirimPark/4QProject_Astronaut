
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);


cbuffer TransformW : register(b2)
{
    matrix World;
}

cbuffer TransformVP : register(b3)
{
    matrix View;
    matrix Projection;
}

cbuffer DeltaTime : register(b7)
{
    float deltaTime;
    int UseFade;
    float pad[2];
}

struct VS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : UV;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : UV;
};

