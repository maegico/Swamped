struct SpotLight
{
	float4 DiffuseColor;
	float3 Position;
	float DiffuseIntensity;
	float3 Direction;
	float nope;
};

struct PointLight
{
	float4 DiffuseColor;
	float3 Position;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	float4 DiffuseColor;
	float3 Direction;
	float nope;
};

struct Lights
{
	//issues
	SpotLight spotLights[8];
	PointLight pointLights[8];
	DirectionalLight dirLight;
	float4  AmbientColor;
	int sizePointLights;
	int sizeSpotLights;
};

//make arrays in the constant buffer
//make a max size for the arrays
//make a parameterized for loop for lighting
//on CPU side

cbuffer lights : register(b0)
{
	Lights lights;
};

Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
SamplerState Sampler	: register(s0);

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
	float4 worldPos		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;		// XYZ tangent
	float2 uv			: TEXCOORD;
	float4 worldSpace   : TEXCOORD1;
};

float4 calcDirLight(float3 normal, DirectionalLight dirLight)
{
	float3 dirToLight = normalize(-dirLight.Direction);

	//N dot L
	float lightAmount = saturate( dot(normal, dirToLight) );

	float4 scaledDiffuse = dirLight.DiffuseColor * lightAmount;

	return scaledDiffuse;
}

float4 calcPointLight(float4 worldPos, float3 normal, PointLight pointLight)
{
	float3 pointLightDirection = normalize(pointLight.Position - worldPos);
	//N dot L
	float lightAmount = saturate( dot(normal, pointLightDirection) );

	float4 scaledDiffuse = pointLight.DiffuseColor * lightAmount;

	return scaledDiffuse;
}

float4 calcSpotLight(float4 worldPos, float3 normal, SpotLight spotLight)
{
	float3 spotLightDirectionToPixel = worldPos - spotLight.Position;
	spotLightDirectionToPixel = -normalize(spotLightDirectionToPixel);

	//N dot L
	float lightAmount = saturate( dot(normal, spotLightDirectionToPixel) );

	//why 
	float angleFromCenter = max( 0.0f, dot(spotLightDirectionToPixel, spotLight.Direction) );
	//raise to a power for a nice "falloff"
	//multiply diffuse and specular results by this
	float spotAmount = pow(angleFromCenter, 20.0f);

	float4 scaledDiffuse = spotLight.DiffuseColor * lightAmount * spotAmount;

	
	//return float4(spotAmount, 0, 0, 0);
	return scaledDiffuse;
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
	float4 sumOfDiffuse = (0, 0, 0, 0);

	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	input.uv.y = 1.0f - input.uv.y;

	//BELOW IS NORMAL MAPPING

	float3 unpackedNormalFromMap = NormalMap.Sample(Sampler, input.uv).rgb * 2.0f - 1.0f;

	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	input.normal = normalize(mul(unpackedNormalFromMap, TBN));

	//BELOW IS LIGHTING

	float4 surfaceColor = Texture.Sample(Sampler, input.uv);

	sumOfDiffuse += calcDirLight(input.normal, lights.dirLight);

	for (int i1 = 0; i1 < lights.sizePointLights; i1++)
	{
		sumOfDiffuse += calcPointLight(input.worldPos, input.normal, lights.pointLights[i1]);
	}

	for (int i2 = 0; i2 < lights.sizeSpotLights; i2++)
	{
		sumOfDiffuse += calcSpotLight(input.worldPos, input.normal, lights.spotLights[i2]);
	}

	//float4 skyColor = Sky.Sample(Sampler, reflect(-toCamera, input.normal));

	float4 finalLighting = sumOfDiffuse + (lights.AmbientColor * 0.1f);

	//fog-related stuff
	float dist = 0;
	float fogFactor = 0;
	float4 fogColor = float4(0.5, 0.5, 0.5, 1.0); //grey

												  //range-based
	dist = length(input.worldSpace);

	//linear fog
	fogFactor = (10 - dist) / (10 - 5);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	//return lerp(fogColor, finalLighting * surfaceColor, fogFactor);
	return finalLighting * surfaceColor;
	//return sumOfDiffuse * surfaceColor;
	//return float4(input.normal, 1);
	//return float4(1, 0, 0, 1);
}