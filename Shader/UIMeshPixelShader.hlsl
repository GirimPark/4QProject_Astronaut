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
int querySpecularTextureLevels()
{
    int width, height, levels;
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
    float cosLo = max(0.0, dot(normal, -float3(1.f, 0.8f, 1.f)));
    
	// Specular reflection vector.
    float3 Lr = 2.0 * cosLo * normal + float3(1.f, 0.8f, 1.f);

	// 수직 입사에서의 프레넬 반사율(금속의 경우 알베도 색상 사용)
    float3 F0 = lerp(Fdielectric, texAlbedo, Metalness);
    
    // --------------------------------------------------------------------------
    //? IBL
    //
    
    float3 ambientLighting = 0;

    if (UseIBL)
    {
        // Sample diffuse irradiance at normal direction.
		// 표면이 받는 반구의 여러 방향에서 오는 간접광을 샘플링한다. Lambertian BRDF를 가정하여 포함되어 있다.
        float3 irradiance = txIBL_Diffuse.Sample(samLinear, normal).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions 
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
		// 비스듬이 보는정도, cos값은 빛 방향을 특정할수 없는 반구의 여러 방향에서 오는 간접광이므로 
		// dot(Half,View)를 사용하지않고  dot(Normal,View)을 사용한다.
        float3 F = fresnelSchlick(F0, cosLo);

		// Get diffuse contribution factor (as with direct lighting).
		// 표면산란 비율을 구한다.
        float3 kd = lerp(1.0 - F, 0.0, Metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
		// txIBL_Diffuse 맵에는 Lambertian BRDF를 가정하여 포함되어 있으므로 PI로 나눌필요가 없다.
        float3 diffuseIBL = kd * texAlbedo * irradiance; // IBL의 diffuse 항

		// Sample pre-filtered specular reflection environment at correct mipmap level.
        int specularTextureLevels = querySpecularTextureLevels(); // 전체 LOD 밉맵 레벨수 
		// View,Normal 반사벡터를 사용하여 샘플링한다. 거칠기에 따라 뭉게진 반사빛을 표현하기위해  LOD 보간이 적용된다. 
        float3 specularIrradiance = txIBL_Specular.SampleLevel(samLinear, Lr, Roughness * specularTextureLevels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
		// dot(Normal,View) , roughness를 텍셀좌표로 샘플링하여 미리계산된 x,y값을 가저온다.
        float2 specularBRDF = txIBL_SpecularBRDF_LUT.Sample(samClamp, float2(cosLo, Roughness)).rg;

		// Total specular IBL contribution.
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
        ambientLighting = (diffuseIBL + specularIBL) * AmbientOcclusion;
    }
    
    //float3 final = directLighting + ambientLighting + Emissive;
    float3 final = texAlbedo + ambientLighting + Emissive;
    float4 finalColor = float4(final, Opacity);
    
    if (UseGamma)
        finalColor.rgb = pow(finalColor.rgb, 1 / 2.2f); // gamma
    return finalColor;
    //return txDiffuse.Sample(samLinear, Input.Tex);
};