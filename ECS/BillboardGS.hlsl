cbuffer externalData : register(b1) {
	matrix vp;
}

struct GSInput {
	float4 position : SV_POSITION;
	float2 size : SIZE;
};

struct GSOutput
{
	float4 position : SV_POSITION;
};

[maxvertexcount(4)]
void main(
	point GSInput gin[1], 
	inout TriangleStream< GSOutput > output
)
{
	
	GSOutput gout;
	gout.position = gin[0].position - float4(1.0, 1.0, 0.0,0.0);
	output.Append(gout);
	gout.position = gin[0].position + float4(1.0, 1.0, 0.0,0.0);
	output.Append(gout);
	gout.position = gin[0].position - float4(1.0, -1.0, 0.0,0.0);
	output.Append(gout);
	gout.position = gin[0].position + float4(1.0, -1.0, 0.0,0.0);
	output.Append(gout);
}