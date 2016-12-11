#define FXAA_QUALITY__PRESET 39
#define FXAA_HLSL_5 1
#define FXAA_PC 1
#include "Fxaa3_11.h"

cbuffer cbFxaa : register(b1) {
	float4 rcpFrame : packoffset(c0);
};

struct FxaaVS_Output {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

SamplerState anisotropicSampler : register(s0);
Texture2D inputTexture : register(t0);

float4 main(FxaaVS_Output Input) : SV_TARGET
{
	FxaaTex tex = { anisotropicSampler, inputTexture };
	return FxaaPixelShader(Input.Tex.xy, 0, tex, tex, tex, rcpFrame.xy, 0, 0, 0, 1.0, .063, .0312, 0, 0, 0, 0);
}