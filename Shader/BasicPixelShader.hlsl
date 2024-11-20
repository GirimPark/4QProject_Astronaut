#include "Shared.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 main(PS_INPUT Input) : SV_Target
{
    float3 normal = normalize(Input.Norm);
    float3 vTangent = normalize(Input.TangentWorld);
    float3 vBiTangent = cross(normal, vTangent);
    
    if (UseNormalMap)
    {
        float3 vNormalTangentSpace = txNormal.Sample(samLinear, Input.Tex).rgb * 2.0f - 1.0f;
        float3x3 WorldTransform = float3x3(vTangent, vBiTangent, normal);
        normal = mul(vNormalTangentSpace, WorldTransform);
        normal = normalize(normal);
    }
    
    float4 normLightDir = normalize(vLightDir);
    float3 diffuse = saturate(dot(-normLightDir.xyz, normal));
    float3 reflection = reflect(normLightDir.xyz, normal);
    
    
    float4 texColor = txDiffuse.Sample(samLinear, Input.Tex);
    if (UseGamma)
        texColor.rgb = pow(texColor.rgb, 2.2f); // gamma
    if (UseDiffuseMap)
        diffuse *= texColor;
    
    
    /*
     *  난반사광(diffuse)이 존재하지 않는 표면(diffuse.x < 0)에는 빛이 닿지 않으므로
     *  정반사광이 존재할 수 없기 때문에 계산에서 제외한다.
     *
     *  pow 함수의 거듭제곱에 들어가는 상수 값을 조절하면 정반사광의 범위가 타이트해진다.
     */      
    
    float3 specular = float3(0.0f, 0.0f, 0.0f);
    
    if (diffuse.x > 0)
    {
        specular = saturate(dot(reflection, -normalize(Input.mViewDir)));
     //   specular = pow(specular, gCubeSpecularPower);
        
        if (UseSpecularMap)
        {
            specular *= txSpecular.Sample(samLinear, Input.Tex);
        }
    }
    
    float3 Emissive = 0;
    if (UseEmissiveMap)
        Emissive = txEmissive.Sample(samLinear, Input.Tex);
    
    float Opacity = 1.0f;
    if (UseOpacityMap)
        Opacity = txOpacity.Sample(samLinear, Input.Tex).a;
    
    float3 ambient = texColor;
    float4 finalColor = float4(ambient + diffuse + specular + Emissive, Opacity) * vLightColor;

    if (UseGamma)
        finalColor.rgb = pow(finalColor.rgb, 1 / 2.2f); // gamma
    return finalColor;
};
