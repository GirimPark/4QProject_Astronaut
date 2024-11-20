//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txEmissive : register(t3);
Texture2D txOpacity : register(t4);
Texture2D txMetalness : register(t5);
Texture2D txRoughness : register(t6);

TextureCube txEnvironment : register(t7);
TextureCube txIBL_Diffuse : register(t8);
TextureCube txIBL_Specular : register(t9);
Texture2D txIBL_SpecularBRDF_LUT : register(t10);
Texture2D txShadowMap : register(t11);

TextureCube txLightIBL_Diffuse : register(t12);
TextureCube txLightIBL_Specular : register(t13);
Texture2D txLightIBL_SpecularBRDF_LUT : register(t14);

SamplerState samLinear : register(s0);
SamplerState samClamp : register(s1);

cbuffer IBL_Buffer : register(b0)
{
    int UseIBL = false;              // 4 
    int UseLightIBL = false; // 4 
    float AmbientOcclusion = 1.0f;   // 4
    int isSkeletal = false; // 4	16
    Matrix m_TestLocal;
}

cbuffer BoolBuffer : register(b1)
{
    int UseNormalMap;
    int UseSpecularMap;
    int UseGamma;
    int UseDiffuseMap;     // 16
    
    int UseEmissiveMap;
    int UseOpacityMap;
    int UseMetalnessMap;
    int UseRoughnessMap;   // 16
    
    int UseCubeMap;
    float MetalnessValue = 1.0f;
    float RoughnessValue = 1.0f;
    int IsGlass;
    
    float ValueIBL = 1.0f;
    float ValueLightIBL = 1.0f;
    float boolPad[2];
}

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

cbuffer LightDirBuffer : register(b4)
{
    float4 vLightDir;
    float4 vLightColor;
    float4 gWorldCameraPosition;
    float4 Lightpad[1];
}

cbuffer MatrixPalette : register(b5)
{
    matrix MatrixPaletteArray[128];
}

static const uint pointlightMaxCount = 10;

cbuffer PointLight : register(b6)
{
    struct
    {
        float4 lightPosition;
        float lightRange;
        float linearTerm;
        float quadraticTerm;
        float lightIntensity;
    
        float3 lightColor;
        float lightPad;
    } pointLights[pointlightMaxCount];
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : UV; // 버텍스 쉐이더의 시멘틱은 의미있는 이름으로 짓는 게 좋다.
    float3 Norm : NORMAL;
    float3 TangentWorld : TANGENT;
    
#ifdef VERTEX_SKINNING 
    int4 BlendIndices : BLENDINDICES;
    float4 BlendWeights : BLENDWEIGHT;
#endif
 
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 ShadowPos : POSTION;
    float4 WorldPos : WorldPosition;
    float2 Tex : UV;
    float3 Norm : NORMAL;
    float3 TangentWorld : TANGENT;
    
    float3 mDiffuse : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float3 mReflection : TEXCOORD3;
};

struct PS_INPUT_ENVIRONMENT
{
    float4 PositionProj : SV_POSITION;
    float3 TexCoords : TEXCOORD0;
};