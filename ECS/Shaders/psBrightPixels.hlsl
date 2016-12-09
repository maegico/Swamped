cbuffer Data: register(b0)
{
	float brightnessLvl;
}

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD0;
};


Texture2D InitialRender	: register(t0);
SamplerState Sampler	: register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 pixelColor = InitialRender.Sample(Sampler, input.uv);
	float brightness = dot(pixelColor.rgb, float3(0.2126, 0.7152, 0.0722));
	if (brightness > brightnessLvl)
		return float4(pixelColor.rgb, 1);
	else
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
}