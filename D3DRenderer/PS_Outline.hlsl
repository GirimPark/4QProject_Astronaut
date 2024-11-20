float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

technique RGBandAlphaMPR
{
    pass p0
    {
        //SetVertexShader(CompileShader(vs_5_0, BasicVertexShader()));
        SetGeometryShader(NULL);
    }    
}