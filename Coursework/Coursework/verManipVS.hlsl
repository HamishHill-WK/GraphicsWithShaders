Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPos;
    float pad;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float4 lightViewPos : TEXCOORD2;
    float3 view : TEXCOORD3;
};

float heightMap(float2 uv)
{
	float texColour;
	texColour = texture0.SampleLevel(sampler0, uv, 0);
	return texColour;
}

float3 calcNormals(float2 UV)
{
    float3 norm = float3(0.0f, 0.0f, 0.0f);
    float d = 0.01;
	
    float2 tanUV = float2(UV + float2(-d, 0.0f));
    float2 tan1UV = float2(UV + float2(d, 0.0f));
    float2 biUV = float2(UV + float2(0.0f, -d));
    float2 bi1UV = float2(UV + float2(0.0f, d));
    float thisUV = UV;
	
    float tanCol = (heightMap(tanUV) * 20);
    float tanCol1 = (heightMap(tan1UV) * 20);
    float biCol = (heightMap(biUV) * 20);
    float biCol1 = (heightMap(bi1UV) * 20);
    float thisCol = (heightMap(UV) * 20);
	
    float3 tangent = float3(-d, tanCol, 0.0f);
    float3 tangent1 = float3(d, tanCol1, 0.0f);
	float3 bitangent = float3(0.0f, biCol, -d);	
	float3 bitangent1 = float3(0.0f, biCol1, d);
    float3 thisPos = float3(0.0f, thisCol, 0.0f);

    tangent -= thisPos;
    tangent1 -= thisPos;
    bitangent -= thisPos;
    bitangent1 -= thisPos;
	
    float3 n1 = cross(bitangent, tangent);
    float3 n2 = cross(tangent, bitangent1);
    float3 n3 = cross(bitangent1, tangent1);
    float3 n4 = cross(tangent1, bitangent);
	
    norm = float3(n1 + n2 + n3 + n4) * 0.25f;
  
    norm = normalize(norm);
	
    return norm;
}

OutputType main(InputType input)
{
	OutputType output;
    
    input.position.y += heightMap(input.tex) * 20 ;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	
    output.normal = calcNormals(input.tex);
	
    output.lightViewPos = mul(input.position, worldMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightViewMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightProjectionMatrix);

    output.worldPosition = mul(input.position, worldMatrix).xyz;
    output.view = cameraPos.xyz - output.worldPosition.xyz;
    output.view = normalize(output.view);

	return output;
}