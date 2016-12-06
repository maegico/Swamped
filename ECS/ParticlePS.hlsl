struct PixelInput {
	float4 position : SV_POSITION;
	float3 center : POSITION;
	float2 size : SIZE;
};

float4 main(PixelInput input) : SV_TARGET
{
	float scalar = distance(input.position, float4(input.center,1.0));
	float4 output = (scalar/input.size.x) * float4(1.0f, 1.0f, 1.0f, 1.0f);
	return output;
}