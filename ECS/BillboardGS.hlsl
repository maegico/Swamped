cbuffer externalData : register(b1) {
	matrix view;
	matrix projection;
	float3 cameraPos;
}

struct GSInput {
	float4 position : SV_POSITION;
	float size : SIZE;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 positionW : POSITION;
	//float3 center : POSITION;
	//float size : SIZE;
};

[maxvertexcount(4)]
void main(
	point GSInput gin[1], 
	inout TriangleStream< GSOutput > output
)
{
	//float4x4 invView = transpose(view);
	//float3 cameraPos = invView[3];
	//
	// Compute the local coordinate system of the sprite relative to the world
	// space such that the billboard is aligned with the y-axis and faces the eye.
	//
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = cameraPos - gin[0].position;
	//look.y = 0.0f; // y-axis aligned, so project to xz-plane
	look = normalize(look);
	float3 right = cross(up, look);

	//
	// Compute triangle strip vertices (quad) in world space.
	//
	float halfWidth = 0.5f*gin[0].size;
	float halfHeight = 0.5f*gin[0].size;

	float4 v[4];
	v[0] = float4(gin[0].position + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(gin[0].position + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(gin[0].position - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(gin[0].position - halfWidth*right + halfHeight*up, 1.0f);

	float2 uv[4];
	uv[0] = float2(0, 1);
	uv[1] = float2(0, 0);
	uv[2] = float2(1, 1);
	uv[3] = float2(1, 0);

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GSOutput gout;
	//gout.center = gin[0].position;
	//gout.size = gin[0].size;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.position = mul(v[i], mul(view, projection));
		gout.positionW = v[i].xyz;
		gout.uv = uv[i];

		output.Append(gout);
	}
}