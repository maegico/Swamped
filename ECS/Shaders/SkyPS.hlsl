
cbuffer externalData : register(b0) {
	float fogHeight;
	float3 cameraPos;
	//matrix invViewProj;
}

// External texture-related data
TextureCube Sky			: register(t4);
SamplerState Sampler	: register(s3);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD;
};


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//float3 worldPos = 100*input.uvw;
	//float3 cameraToVert = worldPos - cameraPos;
	//float totalY = abs(cameraPos.y - worldPos.y);
	//float cameraToFogY = saturate((fogHeight - cameraPos.y) / totalY);
	//float vertToFogY = saturate((fogHeight - worldPos.y) / totalY);
	//float lengthPercent = saturate(cameraToFogY + vertToFogY);
	//float fogDistance = lengthPercent * length(cameraToVert);

	////fog-related stuff
	//float fogFactor = 0;
	float4 fogColor = float4(0.1, 0.3, 0, 1.0); //grey

	//											//linear fog
	//fogFactor = (100 - fogDistance) / (100 - 20);
	//fogFactor = clamp(fogFactor, 0.0, 1.0);

	return lerp(fogColor, Sky.Sample(Sampler, input.uvw), input.uvw.y/.3);
}