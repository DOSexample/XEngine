matrix mWorldViewProjMatrix;
float3 mLightDirection;
float3 mLightAmbient;
float3 mLightDiffuse;

struct VS_INPUT
{	
    float4 mPosition    : POSITION;	
    float2 mTexCoord    : TEXCOORD;
};

struct VS_OUTPUT
{	
    float4 mPosition : POSITION;	
    float3 mColor    : COLOR;	
    float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT Main( VS_INPUT tInput )
{	
    VS_OUTPUT tOutput = (VS_OUTPUT) 0;	
    float3 tPosition = tInput.mPosition.xyz;
	float3 tNormal = 1.0f;
    tOutput.mPosition = mul( float4( tPosition.xyz, 1.0f ), mWorldViewProjMatrix );	
    float3 tDotForDiffuse = saturate( dot( tNormal, mLightDirection ) );	
    tOutput.mColor = mLightAmbient + mLightDiffuse * tDotForDiffuse;
    tOutput.mTexCoord = tInput.mTexCoord;	
    return tOutput;
}
