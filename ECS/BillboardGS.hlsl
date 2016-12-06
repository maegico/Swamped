cbuffer externalData : register(b1) {
	matrix view;
	matrix projection;
	float3 cameraPos;
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
	float halfWidth = 0.5f*gin[0].size.x;
	float halfHeight = 0.5f*gin[0].size.y;

	float4 v[4];
	v[0] = float4(gin[0].position + halfWidth*right - halfHeight*up, 1.0f);
	v[1] = float4(gin[0].position + halfWidth*right + halfHeight*up, 1.0f);
	v[2] = float4(gin[0].position - halfWidth*right - halfHeight*up, 1.0f);
	v[3] = float4(gin[0].position - halfWidth*right + halfHeight*up, 1.0f);

	//
	// Transform quad vertices to world space and output 
	// them as a triangle strip.
	//
	GSOutput gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.position = mul(v[i], mul(view, projection));

		output.Append(gout);
	}
}