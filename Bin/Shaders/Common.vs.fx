matrix mWorldViewProjMatrix;
float3 mLightDirection;
float3 mLightAmbient;
float3 mLightDiffuse;

struct VS_INPUT
{
    float4 mPosition    : POSITION;	
    float4 mBlendWeight : BLENDWEIGHT;	
    float4 mBlendIndex  : BLENDINDICES;	
    float3 mTangent     : TANGENT;	
    float3 mBinormal    : BINORMAL;	
    float3 mNormal      : NORMAL;	
    float2 mTexCoord    : TEXCOORD;
};

struct VS_OUTPUT
{	
    float4 mPosition       : POSITION;
    float3 mColor		   : COLOR;	
    float2 mTexCoord       : TEXCOORD0;
};

VS_OUTPUT Main( VS_INPUT tInput )
{
    VS_OUTPUT tOutput = (VS_OUTPUT) 0;		
    tOutput.mPosition = mul( float4( tInput.mPosition.xyz, 1.0f ), mWorldViewProjMatrix );
    float3 tDotForDiffuse = saturate( dot( tInput.mPosition, mLightDirection ) );
    tOutput.mColor = mLightAmbient + mLightDiffuse * tDotForDiffuse;
    tOutput.mTexCoord = tInput.mTexCoord;
    return tOutput;
}
