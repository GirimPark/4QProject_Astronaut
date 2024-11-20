struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : UV; // 버텍스 쉐이더의 시멘틱은 의미있는 이름으로 짓는 게 좋다.
    float3 Norm : NORMAL;
    float3 TangentWorld : TANGENT;
    int4 BlendIndices : BLENDINDICES;
    float4 BlendWeights : BLENDWEIGHT;
};

struct PS_INPUT
{
    float4 ProjPos : SV_POSITION;
};

cbuffer TransformW : register(b2)
{
    matrix World;
}

cbuffer TransformVP : register(b3)
{
    matrix View;
    matrix Projection;
    matrix ShadowView;
    matrix ShadowProjection;
}

cbuffer MatrixPalette : register(b5)
{
    matrix MatrixPaletteArray[128];
}

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    // 오브젝트 월드 변환
    float4 pos = input.Pos;
    float4x4 matWorldBlended = MatrixPaletteArray[0];
    
    // 본이 있으면 본의 월드 트랜스폼 위치에 맞춰서 위치 아니면 기존 월드 트랜스폼 위치에 위치

    // 월드 내의 위치
    matWorldBlended = mul(input.BlendWeights.x, MatrixPaletteArray[input.BlendIndices.x]);
    matWorldBlended += mul(input.BlendWeights.y, MatrixPaletteArray[input.BlendIndices.y]);
    matWorldBlended += mul(input.BlendWeights.z, MatrixPaletteArray[input.BlendIndices.z]);
    matWorldBlended += mul(input.BlendWeights.w, MatrixPaletteArray[input.BlendIndices.w]);
    pos = mul(pos, matWorldBlended);
    
    // ndc 공간 내의 위치
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.ProjPos = pos;
    
    return output;
}