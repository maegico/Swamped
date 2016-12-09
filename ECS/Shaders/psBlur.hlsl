cbuffer Data : register(b0)
{
	float pixelWidth;
	float pixelHeight;
	int blurAmount;
}

struct VertexToPixel
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

Texture2D BrightPixels	: register(t0);
SamplerState Sampler	: register(s0);

//box blur, should change to Guassian
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 totalColor = float4(0,0,0,0);
	uint numSamples = blurAmount * blurAmount * 4;	//taking this out of the for loop

	for (int y = -blurAmount; y < blurAmount; y++)
	{
		for (int x = -blurAmount; x < blurAmount; x++)
		{
			//figure out this pixel's uv
			float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);

			totalColor += BrightPixels.Sample(Sampler, uv);
			//numSamples++;
		}
	}

	return totalColor / numSamples;
}