Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
    float screenHeight;
    float screenWidth;
    float2 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 colour;
    float4 colour1;
    float4 colourf;
    
    float weight = -1.0;
    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);
    colour1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    colourf = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float texelSize = 1.0f / screenHeight; 
    float texelSize1 = 1.0f / screenWidth;
    // Add the vertical pixels to the colour by the specific weight of each.
    colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 1.0f)) * weight; //top mid
    colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -1.f)) * weight; //bottom mid
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * 1.0f, texelSize * 1.f)) * weight; //top right
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * 1.0f, texelSize * -1.0f)) * weight; //bottom right
    colour += shaderTexture.Sample(SampleType, input.tex) * 8;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * -1.0f, texelSize * 1.f)) * weight; //top left
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * -1.0f, texelSize * -1.0f)) * weight; //bottom left
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * 1.0f, 0)) * weight;
    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * -1.0f, 0)) * weight;
 
    // Add the horizontal pixels to the colour by the specific weight of each.
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 1.0f)) * weight; //top mid
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -1.f)) * weight; //bottom mid
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * 1.0f, texelSize * 1.f)) * weight; //top right
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * 1.0f, texelSize * -1.0f)) * weight; //bottom right
    colour1 += shaderTexture.Sample(SampleType, input.tex) * 8;
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * -1.0f, texelSize * 1.f)) * weight; //top left
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * -1.0f, texelSize * -1.0f)) * weight; //bottom left
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * 1.0f, 0)) * weight;
    colour1 += shaderTexture.Sample(SampleType, input.tex + float2(texelSize1 * -1.0f, 0)) * weight;
    
    colour.a = 1.0f;
    colour1.a = 1.f;
    
    colour = colour * colour;
    colour1 = colour1 * colour1;
    
    colourf = sqrt(colour + colour1);
    colourf = normalize(colourf);
    
    return colourf;
}

 