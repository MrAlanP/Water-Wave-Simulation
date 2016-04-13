//Code originating from Rastertek has been utilised and modified within this class
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

//Vertex Shader

PixelInput VertexLightShader(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

	//Calculate the position of the vertex based on each matrix
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Set the output colour to the same as is input
	output.tex = input.tex;

	//Calculate the normal against the world matrix
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//Normalise the normal vector
	output.normal = normalize(output.normal);

	//Calculate the position of the vertex
	float4 worldPosition = mul(input.position, worldMatrix);

	//Calculate view direction based on camera and vertex position
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	//Normalise the view direction
	output.viewDirection = normalize(output.viewDirection);

	return output;
};

