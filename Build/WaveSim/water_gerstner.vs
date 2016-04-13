cbuffer MatrixBuffer : register (b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer UpdateBuffer : register (b1)
{
	float3 cameraPosition;
	float time;
};

cbuffer WaveBuffer : register (b2)
{
	float4 amplitude[8];
	float4 wavelength[8];
	float4 speed[8];
	float4 waveDirectionX[8];	
	float4 waveDirectionY[8];	
};

//Typedefs

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
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 bumpTex : TEXCOORD1;
	float2 bumpTex2 : TEXCOORD2;
	float3 viewDirection : TEXCOORD3;
	float whitecapStrength : TEXCOORD4;
};


float3 CalculateTangent(float3 normal)
{
	float3 tangent;

	float3 cross1 = cross(normal,float3(0,0,1));
	float3 cross2 = cross(normal,float3(0,1,0));

	if(length(cross1)>length(cross1))
	{
		tangent = cross1;
	}
	else
	{
		tangent = cross1;
	}

	return tangent;
}

float3 CalculateBinormal(float3 normal, float3 tangent)
{
	return cross(normal, tangent);
}


//Vertex Shader
PixelInput VertexLightShader(VertexInput input)
{
	PixelInput output;
	float4 worldPosition;

	input.position.w = 1.0f;
	
	//Update position based on camera position
	input.position.x += cameraPosition.x;
	input.position.z += cameraPosition.z;
	//Calculate the position of the vertex
	worldPosition = mul(input.position, worldMatrix);

	input.tex.x = (worldPosition.x*0.2f);
	input.tex.y = (worldPosition.z*0.2f);

	float3 normal = float3(0,0,0);
	float2 averageDirection = float2(0,0);
	float averageSpeed = 0;

	//Y position before gerstner equations
	float oldY = input.position.y;

	//Calculate the 8 gerstner waves
	for(int i = 0; i<8; i++)
	{
		//Gerstner wave y calculation
		float2 direction = float2(waveDirectionX[i].x, waveDirectionY[i].x);
		averageDirection += direction;
		averageSpeed += speed[i];
		float dotProd = dot(direction, float2(worldPosition.x, worldPosition.z));
		float phase = (2*3.14f*(dotProd - speed[i]*time)/wavelength[i]);
		float yAdjustment = amplitude[i]*sin(phase);
		input.position.y += yAdjustment;

		//Gerstner wave normal
		normal.x += -(direction.x * amplitude[i] * cos(phase));
		normal.y += 1;
		normal.z += -(direction.y * amplitude[i] * cos(phase));

		output.whitecapStrength += (yAdjustment-oldY)*0.15f;
	}

	averageDirection = normalize(averageDirection);
	averageSpeed /= 8;
	//Average whitecap strength
	output.whitecapStrength /= 8;

	input.normal = float3(normal.x,normal.y,normal.z)/8;
	
	//Calculate the position of the vertex based on each matrix
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Set the output colour to the same as is input
	output.tex = input.tex;
	output.bumpTex = input.tex*0.1 - (averageDirection*time*averageSpeed*0.01);
	output.bumpTex2 = input.tex*0.1 + (averageDirection*time*averageSpeed*0.01);

	//Calculate the normal against the world matrix
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//Normalise the normal vector
	output.normal = normalize(output.normal);
	//Calculate tangent and binormal from normal
	output.tangent = CalculateTangent(output.normal);
	output.binormal = CalculateBinormal(output.normal, output.tangent);

	//Calculate view direction based on camera and vertex position
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	//Normalise the view direction
	output.viewDirection = normalize(output.viewDirection);

	return output;
};

