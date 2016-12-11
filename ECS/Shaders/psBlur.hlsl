#define FULL_BLUR

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
#ifdef FULL_BLUR
	uint numSamples = 4*(blurAmount*blurAmount+blurAmount)+2;	//taking this out of the for loop

	for (int y = -blurAmount; y <= blurAmount; y++)
	{
		for (int x = -blurAmount; x <= blurAmount; x++)
		{
			//figure out this pixel's uv
			float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);

			totalColor += (1-((x*x+y*y)/(blurAmount*blurAmount))) * BrightPixels.Sample(Sampler, uv);
			//numSamples++;
		}
	}

#else

	uint numSamples = blurAmount * 4 + 1;

	for (int x = -blurAmount; x <= blurAmount; x++) {
		float2 uv = input.uv + float2(x*pixelWidth, 0);
		totalColor += (1 - (x*x) / (blurAmount*blurAmount)) * BrightPixels.Sample(Sampler, uv);
	}
	for (int y = -blurAmount; y < 0; y++) {
		float2 uv = input.uv + float2(0, y*pixelWidth);
		totalColor += (1 - (y*y) / (blurAmount*blurAmount)) * BrightPixels.Sample(Sampler, uv);
	}
	for (y = 1; y <= blurAmount; y++) {
		float2 uv = input.uv + float2(0, y*pixelWidth);
		totalColor += (1 - (y*y) / (blurAmount*blurAmount)) * BrightPixels.Sample(Sampler, uv);
	}
#endif

	return totalColor / numSamples;
}