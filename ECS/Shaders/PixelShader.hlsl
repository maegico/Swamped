struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer lights : register(b0)
{
	DirectionalLight dirLight1;
	DirectionalLight dirLight2;
	DirectionalLight dirLight3;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

float4 calcDirLight(float3 normal, DirectionalLight dirLight)
{
	float3 dirToLight = normalize(-dirLight.Direction);

	//N dot L
	float lightAmount = dot(normal, dirToLight);
	lightAmount = saturate(lightAmount);

	float4 scaledDiffuse = dirLight.DiffuseColor * lightAmount;

	return scaledDiffuse + dirLight.AmbientColor;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	float4 dirL1 = calcDirLight(input.normal, dirLight1);
	float4 dirL2 = calcDirLight(input.normal, dirLight2);
	float4 dirL3 = calcDirLight(input.normal, dirLight3);

	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return (dirL1 + dirL2 + dirL3) * surfaceColor;
}