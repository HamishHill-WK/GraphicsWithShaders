Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;

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
    float4 depthPos : TEXCOORD0;
};

float heightMap(float2 uv)
{
    float texColour;
    texColour = texture0.SampleLevel(sampler0, uv, 0);
    return texColour;
}

OutputType main(InputType input)
{
    OutputType output;
    
    input.position.y += heightMap(input.tex) * 20;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
    output.depthPos = output.position;

    return output;
}