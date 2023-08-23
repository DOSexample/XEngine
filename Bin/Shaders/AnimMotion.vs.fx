float4x3 mKeyMatrix[82];
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
    float4 mPosition : POSITION;	
    float3 mColor    : COLOR;	
    float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT Main( VS_INPUT tInput )
{	
    VS_OUTPUT tOutput = (VS_OUTPUT) 0;	
    int4 tBlendIndexVector = D3DCOLORtoUBYTE4( tInput.mBlendIndex );	
    int tBlendIndexArray[4] = (int[4]) tBlendIndexVector;	
    float3 tPosition = 0.0f;	
    float3 tTangent = 0.0f;
    float3 tBinormal = 0.0f;
    float3 tNormal = 0.0f;
	
    tPosition += mul( tInput.mPosition, mKeyMatrix[tBlendIndexArray[0]] ) * tInput.mBlendWeight.x;	
    tPosition += mul( tInput.mPosition, mKeyMatrix[tBlendIndexArray[1]] ) * tInput.mBlendWeight.y;	
    tPosition += mul( tInput.mPosition, mKeyMatrix[tBlendIndexArray[2]] ) * tInput.mBlendWeight.z;	
    tPosition += mul( tInput.mPosition, mKeyMatrix[tBlendIndexArray[3]] ) * tInput.mBlendWeight.w;	
    tTangent += mul( tInput.mTangent, (float3x3) mKeyMatrix[tBlendIndexArray[0]] ) * tInput.mBlendWeight.x;	
    tTangent += mul( tInput.mTangent, (float3x3) mKeyMatrix[tBlendIndexArray[1]] ) * tInput.mBlendWeight.y;	
    tTangent += mul( tInput.mTangent, (float3x3) mKeyMatrix[tBlendIndexArray[2]] ) * tInput.mBlendWeight.z;	
    tTangent += mul( tInput.mTangent, (float3x3) mKeyMatrix[tBlendIndexArray[3]] ) * tInput.mBlendWeight.w;	
    tBinormal += mul( tInput.mBinormal, (float3x3) mKeyMatrix[tBlendIndexArray[0]] ) * tInput.mBlendWeight.x;	
    tBinormal += mul( tInput.mBinormal, (float3x3) mKeyMatrix[tBlendIndexArray[1]] ) * tInput.mBlendWeight.y;	
    tBinormal += mul( tInput.mBinormal, (float3x3) mKeyMatrix[tBlendIndexArray[2]] ) * tInput.mBlendWeight.z;	
    tBinormal += mul( tInput.mBinormal, (float3x3) mKeyMatrix[tBlendIndexArray[3]] ) * tInput.mBlendWeight.w;	
    tNormal += mul( tInput.mNormal, (float3x3) mKeyMatrix[tBlendIndexArray[0]] ) * tInput.mBlendWeight.x;	
    tNormal += mul( tInput.mNormal, (float3x3) mKeyMatrix[tBlendIndexArray[1]] ) * tInput.mBlendWeight.y;	
    tNormal += mul( tInput.mNormal, (float3x3) mKeyMatrix[tBlendIndexArray[2]] ) * tInput.mBlendWeight.z;	
    tNormal += mul( tInput.mNormal, (float3x3) mKeyMatrix[tBlendIndexArray[3]] ) * tInput.mBlendWeight.w;	

	tOutput.mPosition = mul( float4( tPosition.xyz, 1.0f ), mWorldViewProjMatrix );	
    float3 tDotForDiffuse = saturate( dot( tNormal, mLightDirection ) );
    tOutput.mColor = mLightAmbient + mLightDiffuse * tDotForDiffuse;
    tOutput.mTexCoord = tInput.mTexCoord;	
    return tOutput;
}
