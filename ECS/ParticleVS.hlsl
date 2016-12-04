struct VertexInput {
	float3 position : POSITION;
	float2 size : SIZE;
};

struct VertexToGeometry {
	float4 position : SV_POSITION;
	float2 size : SIZE;
};

VertexToGeometry main( VertexInput input )
{
	VertexToGeometry vtg;
	vtg.position = float4(input.position, 1.0);
	vtg.size = input.size;

	return vtg;
}