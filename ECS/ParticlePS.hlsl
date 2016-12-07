struct PixelInput {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	//float4 center : POSITION;
	//float2 size : PSIZE;
	//float distance : DISTANCE;
};

Texture2D pTexture : register(t3);
SamplerState pSampler : register(s2);

float4 main(PixelInput input) : SV_TARGET
{
	return pTexture.Sample(pSampler, input.uv);
}