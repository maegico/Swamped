struct PixelInput {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 positionW : POSITION;
	//float4 center : POSITION;
	//float2 size : PSIZE;
	//float distance : DISTANCE;
};

cbuffer externalData :register(b0) {
	float3 cameraPos;
	float fogHeight;
}

Texture2D pTexture : register(t3);
SamplerState pSampler : register(s2);

float4 main(PixelInput input) : SV_TARGET
{
	float3 worldPos = input.positionW;
	float3 cameraToVert = worldPos - cameraPos;
	float totalY = abs(cameraPos.y - worldPos.y);
	float cameraToFogY = saturate((fogHeight - cameraPos.y) / totalY);
	float vertToFogY = saturate((fogHeight - worldPos.y) / totalY);
	float lengthPercent = saturate(cameraToFogY + vertToFogY);
	float fogDistance = lengthPercent * length(cameraToVert);

	//fog-related stuff
	float fogFactor = 0;
	float4 fogColor = float4(0.1, 0.3, 0, 1.0); //grey

												//linear fog
	fogFactor = (100 - fogDistance) / (100 - 20);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	return lerp(fogColor, pTexture.Sample(pSampler, input.uv),fogFactor);
}