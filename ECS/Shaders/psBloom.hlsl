struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

Texture2D InitialRender	: register(t0);
Texture2D Blur	: register(t1);
SamplerState Sampler	: register(s0);

//box blur, should change to Guassian
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 totalColor = float4(0,0,0,0);

	totalColor += InitialRender.Sample(Sampler, input.uv);
	totalColor += Blur.Sample(Sampler, input.uv);
	
	return totalColor;
}