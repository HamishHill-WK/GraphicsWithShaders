
// Calculate diffuse lighting for a single directional light (also texturing)
Texture2D depthTex : register(t1);
SamplerState shadowSamp : register(s1);

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    //float4 ambient[2];
    //float4 diffuse[2];
    //float3 position[2];
    //float type;
    //float4 atten[2];
    

    float3 lightDirection;
    float pad;
    float4 ambient[2];
    float4 diffuse[2];
    float3 position;
    float pad1;
    float3 atten;
    float pad2;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

bool isInShadow(Texture2D sMap, float2 uv, float4 lightViewPosition, float bias)
{
    // Sample the shadow map (get depth of geometry)
    float depthValue = sMap.Sample(shadowSamp, uv).r;
	// Calculate the depth from the light.
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        return false;
    }
    return true;
}

float2 getProjectiveCoords(float4 lightViewPosition)
{
    // Calculate the projected texture coordinates.
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}

float4 main(InputType input) : SV_TARGET
{ 
    float shadowMapBias = 0.005f;
    
    float4 textureColour;
    float4 lightColour;

    textureColour = texture0.Sample(sampler0, (input.tex));

   // float2 pTexCoord = getProjectiveCoords(input.worldPosition);
	
    // Shadow test. Is or isn't in shadow
   // if (hasDepthData(pTexCoord))
  //  {
        // Has depth map data
     //   if (!isInShadow(depthTex, pTexCoord, input.worldPosition, shadowMapBias))
     //   {
        
	
            float d; //array of distances from source to pixel -hh
            float attenMod; //"" attenuation modifiers -hh
            float3 lightVector; //"" normalized light vectors -hh
	
            d = length(position.xyz - input.worldPosition);

            attenMod = 1 / ((atten.x + (atten.y * d)) + (atten.z * (d * d)));
	
            lightVector = normalize(position.xyz - input.worldPosition);
	
            float4 finalDif;
	
                    //calculate lighting for point light                                    //add directional light
            finalDif = (calculateLighting(lightVector, input.normal, diffuse[1]) * attenMod) + (calculateLighting(lightDirection, input.normal, diffuse[0]));
	
            lightColour = ambient[0] + ambient[1] + finalDif;
        
        //}
    //    break;
//    }
    
    return lightColour * textureColour;

}

//float4 main(InputType input) : SV_TARGET
//{
//	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
//    float4 textureColour = texture0.Sample(sampler0, input.tex);
	
//    float d[2]; //array of distances from source to pixel -hh
//    float attenMod[2]; //"" attenuation modifiers -hh
//    float3 lightVector[2]; //"" normalized light vectors -hh
	
//    d[0] = length(position[0].xyz - input.worldPosition);
//    d[1] = length(position[1].xyz - input.worldPosition);

//    attenMod[0] = 1 / ((atten[0].x + (atten[0].y * d[0])) + (atten[0].z * (d[0] * d[0])));
//    attenMod[1] = 1 / ((atten[1].x + (atten[1].y * d[1])) + (atten[1].z * (d[1] * d[1])));

	
//    lightVector[0] = normalize(position[0].xyz - input.worldPosition);
//    lightVector[1] = normalize(position[1].xyz - input.worldPosition);
	
//    float4 finalDif;
	
//    finalDif = (calculateLighting(lightVector[0], input.normal, diffuse[0]) * attenMod[0]) +
//	(calculateLighting(lightVector[1], input.normal, diffuse[1]) * attenMod[1]);
	
//    float4 lightColour = ambient[0] + ambient[1] + finalDif;
	
//    return lightColour * textureColour;
//}



