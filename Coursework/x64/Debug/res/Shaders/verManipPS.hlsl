
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    //float3 lightDirection;
    //float pad;
    //float4 ambient;
    //float4 diffuseColour;
    //float3 position;
    //float pad1;
    //float3 atten;
    //float pad2;    
    
    float3 lightDirection;
    float pad;
    float4 ambient[2];
    float4 diffuseColour[2];
    float3 position;
    float pad1;
    float3 atten;
    bool norms;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
};


float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    if(norms)
    {
        float4 colour = float4(input.normal.x, input.normal.y, input.normal.z, 0.0f);
        return colour;
    }
    
    float4 textureColour;
  //  float4 lightColour[3];
    //float4 finalColour;

    textureColour = texture0.Sample(sampler0, (input.tex * 2));
    
 //   for (int i = 0; i <= 2; i++)
   // {
        //switch (type[i])
        //{
        //    case 0:
              // lightColour[0] = (calculateLighting(-lightDirection[0], input.normal, diffuseColour[0]));
    //            break;
        
            //case 1:
            //    break;
        
     //       case 2:
	
    float d; //array of distances from source to pixel -hh
    float attenMod; //"" attenuation modifiers -hh
    float3 lightVector; //"" normalized light vectors -hh
	
    //d = length(position[1].xyz - input.worldPosition);
    d = length(position.xyz - input.worldPosition);

    attenMod = 1 / ((atten.x + (atten.y * d)) + (atten.z * (d * d)));
	
    //   lightVector = normalize(position[1].xyz - input.worldPosition);
    lightVector = normalize(position.xyz - input.worldPosition);
	
    float4 finalDif;
	
  //  finalDif = /*(ambient + calculateLighting(-lightDirection[0], input.normal, diffuseColour[0])) +*/
  //  (calculateLighting(lightVector, input.normal, diffuseColour[1]) * attenMod);    
    
    finalDif = calculateLighting(-lightDirection, input.normal, diffuseColour[0]) +
    (calculateLighting(lightVector, input.normal, diffuseColour[1]) * attenMod) + ambient[0] + ambient[1];       
    
    return (finalDif * textureColour);
    

    
}

