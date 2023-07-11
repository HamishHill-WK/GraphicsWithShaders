Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
    float screenWidth;
    float3 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 colour;

    float weight = -1.0;

	// Initialize the colour to black.
    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float texelSize = 1.0f / screenWidth;
    // Add the horizontal pixels to the colour by the specific weight of each.
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -0.2f, 0.0f)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -0.15f, 0.0f)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -0.1f, 0.0f)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -0.05f, 0.0f)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex) * 9;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 0.05f, 0.0f)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 0.1f, 0.0f)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 0.15f, 0.0f)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 0.2f, 0.0f)) * weight;
	// Set the alpha channel to one.
    colour.a = 1.0f;

    return colour;
}
