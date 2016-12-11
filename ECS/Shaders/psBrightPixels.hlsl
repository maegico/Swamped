cbuffer Data: register(b0)
{
	float brightnessMin;
	float brightnessMax;
	float exponent;
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
	brightness = clamp(brightness - brightnessMin, 0.0, 1.0);
	float topBrightness = brightnessMax - brightnessMin;
	//if (brightness > brightnessLvl)
		return float4(clamp(pow(brightness,exponent)/pow(topBrightness,exponent),0.0,1.0) * pixelColor.rgb, 1);
	//else
	//	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}