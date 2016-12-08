
// External texture-related data
TextureCube Sky			: register(t4);
SamplerState Sampler	: register(s3);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 worldPos		: POSITION;
	float4 cameraSpace	: TEXCOORD1;
	float3 uvw			: TEXCOORD;
};


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//fog-related stuff
	float dist = 0;
	float fogFactor = 0;
	float4 fogColor = float4(0.5, 0.5, 0.5, 1.0); //grey

												  //range-based
	dist = length(input.cameraSpace);

	//linear fog
	/*fogFactor = (10 - dist) / (10 - 5);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	return lerp(fogColor, Sky.Sample(Sampler, input.uvw), fogFactor);*/
	return Sky.Sample(Sampler, input.uvw);
}