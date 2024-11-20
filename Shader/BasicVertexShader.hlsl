#include "Shared.hlsli"
// 정점 셰이더.

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

PS_INPUT main(VS_INPUT Input)
{
	PS_INPUT output;
	    
    // 최대 4개까지의 본을 참조함. x y z w (이름은 중요하지 않음)
    float4x4 matWorld;
#ifdef VERTEX_SKINNING 
    matWorld =  mul(Input.BlendWeights.x, MatrixPaletteArray[Input.BlendIndices.x]);
    matWorld += mul(Input.BlendWeights.y, MatrixPaletteArray[Input.BlendIndices.y]);
    matWorld += mul(Input.BlendWeights.z, MatrixPaletteArray[Input.BlendIndices.z]);
    matWorld += mul(Input.BlendWeights.w, MatrixPaletteArray[Input.BlendIndices.w]);
#else
    matWorld = World;
#endif
 
	// 정점 월드 변환
    output.WorldPos = mul(Input.Pos, matWorld);
    output.Tex = Input.Tex;
    
	// 카메라 벡터
    output.mViewDir = normalize(output.WorldPos.xyz - gWorldCameraPosition.xyz);
    output.Norm         = normalize(mul(Input.Norm, (float3x3) matWorld));
    output.TangentWorld = normalize(mul(Input.TangentWorld, (float3x3) matWorld));
    
    output.ShadowPos = mul(output.WorldPos, ShadowView);
    output.ShadowPos = mul(output.ShadowPos, ShadowProjection);
    
    output.Pos = mul(output.WorldPos, View);
    output.Pos = mul(output.Pos, Projection);
    
    return output;
};