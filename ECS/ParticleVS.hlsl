struct VertexInput {
	float3 position : POSITION;
	float3 velocity : VELOCITY;
	float size : SIZE;
	float birthTime : TIME;
};

struct VertexToGeometry {
	float4 position : SV_POSITION;
	float size : SIZE;
};

cbuffer externalData : register(b2) {
	float currentTime;
}

VertexToGeometry main( VertexInput input )
{
	VertexToGeometry vtg;
	vtg.position = float4(input.position + (currentTime-input.birthTime)*input.velocity, 1.0);
	vtg.size = input.size;

	return vtg;
}