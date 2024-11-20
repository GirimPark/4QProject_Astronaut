#include "Shared.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

static const float PI = 3.141592;
static const float Epsilon = 0.00001;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04;

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;

    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint querySpecularTextureLevels()
{
    uint width, height, levels;
    txIBL_Specular.GetDimensions(0, width, height, levels);
    return levels;
}

float4 main(PS_INPUT Input) : SV_Target
{
    float3 normal = normalize(Input.Norm);
    
    if (UseNormalMap)
    {
        float3 vTangent = normalize(Input.TangentWorld);
        float3 vBiTangent = cross(normal, vTangent);
        float3 vNormalTangentSpace = txNormal.Sample(samLinear, Input.Tex).rgb * 2.0f - 1.0f;
        
        float3x3 WorldTransform = float3x3(vTangent, vBiTangent, normal);
        normal = mul(vNormalTangentSpace, WorldTransform);
        normal = normalize(normal);
    }
    
    float4 normLightDir = normalize(vLightDir);
    float3 texAlbedo = txDiffuse.Sample(samLinear, Input.Tex).rgb;
    
    if (UseGamma)
        texAlbedo.rgb = pow(texAlbedo.rgb, 2.2f); // gamma
    
    float Metalness = 0.0f;
    float Roughness = Epsilon;

    float Opacity = 1.0f;
    float3 Emissive = 0.0f;
    
    
    if (isSkeletal)
    {
        Roughness = Epsilon; // skeletal
    }
    else
    {
        Roughness = 1.0f; // static
    }
    
    if (UseEmissiveMap)
        Emissive = txEmissive.Sample(samLinear, Input.Tex).rgb;
    if (UseMetalnessMap)
        Metalness = txMetalness.Sample(samLinear, Input.Tex).r * MetalnessValue;
    if (UseRoughnessMap)
        Roughness = txRoughness.Sample(samLinear, Input.Tex).r * RoughnessValue;
    if (UseOpacityMap) 
        Opacity = txOpacity.Sample(samLinear, Input.Tex).r;
    
    //--------------------------------------------------------------------------
    //? PBR
    //
    
    // 현재 Input.mViewDir 는 카메라에서 버텍스 방향.    
    
    // 표면 법선 방향과 나가는 빛 방향 사이의 각도입니다.
    float cosLo = max(0.0, dot(normal, -Input.mViewDir));
    
	// Specular reflection vector.
    float3 Lr = 2.0 * cosLo * normal + Input.mViewDir;

	// 수직 입사에서의 프레넬 반사율(금속의 경우 알베도 색상 사용)
    float3 F0 = lerp(Fdielectric, texAlbedo, Metalness);
    
	// Li와 Lo 사이의 절반 벡터.
    float3 Lh = normalize(-normLightDir.rgb - Input.mViewDir);

	// 표면 법선과 다양한 라이트 벡터 사이의 각도를 계산합니다.
    float cosLi = max(0.0, dot(normal, -normLightDir.rgb));
    float cosLh = max(0.0, dot(normal, Lh));

    float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, -Input.mViewDir)));
    float D = ndfGGX(cosLh, max(0.1f, Roughness));
    float G = gaSchlickGGX(cosLi, cosLo, Roughness);
    
	// Cook-Torrance specular microfacet BRDF.
    float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

	// 확산 산란은 빛이 유전 매체에 의해 여러 번 굴절되기 때문에 발생합니다.
    // 반면에 금속은 에너지를 반사하거나 흡수하므로 확산 기여도는 항상 0입니다.
    // 에너지를 절약하려면 프레넬 인자 및 금속성을 기반으로 확산 BRDF 기여도를 조정해야 합니다.
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), Metalness);

	// 램버트 확산 BRDF.
    // 보다 편리하게 조명 및 재료 단위를 위해 1/PI 단위로 크기를 조정하지 않습니다.
    float3 diffuseBRDF = kd * texAlbedo;

	// Total contribution for this light.
    float3 directLighting = (diffuseBRDF + specularBRDF) * vLightColor.rgb * cosLi;
        
    // --------------------------------------------------------------------------
    //? IBL
    
    float3 ambientLighting = 0;
    if (UseIBL)
    {
        float3 irradiance = txIBL_Diffuse.Sample(samLinear, normal).rgb;

        float3 F = fresnelSchlick(F0, cosLo);
        float3 kd = lerp(1.0 - F, 0.0, Metalness);
        float3 diffuseIBL = kd * texAlbedo * irradiance; // IBL의 diffuse 항
        uint specularTextureLevels = querySpecularTextureLevels(); // 전체 LOD 밉맵 레벨수 

        float3 specularIrradiance = txIBL_Specular.SampleLevel(samLinear, Lr, Roughness * specularTextureLevels).rgb;
        float2 specularBRDF = txIBL_SpecularBRDF_LUT.Sample(samClamp, float2(cosLo, Roughness)).rg;
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

        ambientLighting = (diffuseIBL + specularIBL) * AmbientOcclusion;
    }
    
    float3 pointAmbientLighting = 0;
    if (UseLightIBL)
    {
        float3 rotMatrix = mul(normal, (float3x3) m_TestLocal);
        float3 irradiance = txLightIBL_Diffuse.Sample(samLinear, rotMatrix).rgb;
        
        float3 F = fresnelSchlick(F0, cosLo);
        float3 kd = lerp(1.0 - F, 0.0, Metalness);
        float3 diffuseIBL = kd * texAlbedo * irradiance; // IBL의 diffuse 항
        uint specularTextureLevels = querySpecularTextureLevels(); // 전체 LOD 밉맵 레벨수 

        float3 specularIrradiance = txLightIBL_Specular.SampleLevel(samLinear, Lr,specularTextureLevels).rgb;
        float2 specularBRDF = txLightIBL_SpecularBRDF_LUT.Sample(samClamp, float2(cosLo, Roughness)).rg;
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

        pointAmbientLighting = (diffuseIBL + specularIBL) * AmbientOcclusion;
    }
    
    // 그림자 처리 부분
    // 광원NDC 좌표계에서의 좌표는 계산해주지 않으므로 계산한다.
    float currentShadowDepth = Input.ShadowPos.z / Input.ShadowPos.w;
    // 광원NDC 좌표계에서의 x(-1 ~ +1), y(-1 ~ +1)
    float2 uv = Input.ShadowPos.xy / Input.ShadowPos.w;
    // NDC좌표계에서 Texture좌표계로 변환
    uv.y = -uv.y; // y는 반대
    uv = uv * 0.5 + 0.5; // -1에서 1을 0~1로 변환
    
    float sampleDepth = txShadowMap.Sample(samLinear, uv).r;
    
    if (uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0)
    {
        if (currentShadowDepth > sampleDepth + 0.001)
        {
            directLighting = 0.f;
        }
    }
    
    float3 pointLighting = float3(0, 0, 0);
    
    for (uint i = 0; i < pointlightMaxCount; ++i)
    {
        float3 normal = normalize(Input.Norm);

     // Light Vector  ->  [obj -> PointLight]
        float3 toLightVec = normalize(pointLights[i].lightPosition.xyz - Input.WorldPos.xyz);
        float3 reflection = reflect(-toLightVec.xyz, normal);
       
        float3 diffuse = saturate(dot(toLightVec.xyz, normal));
        float3 specualr = saturate(dot(reflection, -normalize(Input.mViewDir)));
     
     // 표면으로부터 광원까지 거리
        float d = distance(pointLights[i].lightPosition, Input.WorldPos);
    
        if (d <= pointLights[i].lightRange)
        {
            toLightVec /= d;
            float att = 1.0f / (1 + (pointLights[i].linearTerm * d) + (pointLights[i].quadraticTerm * d * d));
         
            pointLighting += att * pointLights[i].lightIntensity * (diffuse + specualr) * pointLights[i].lightColor;
        }
    }
    
    float3 final = directLighting + ambientLighting * ValueIBL + pointAmbientLighting * ValueLightIBL + pointLighting + Emissive;
    float4 finalColor = float4(final, Opacity);
    
    if (UseGamma)
        finalColor.rgb = pow(finalColor.rgb, 1 / 2.2f); // gamma
    return finalColor;
};
