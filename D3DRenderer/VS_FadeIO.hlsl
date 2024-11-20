#include "Shared_UI.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT ps_outPut;
    
    ps_outPut.Pos = mul(input.Pos, World);
    // ps_outPut.Pos = mul(input.Pos, View);
    ps_outPut.Pos = mul(input.Pos, Projection);    
    
    return ps_outPut;
}