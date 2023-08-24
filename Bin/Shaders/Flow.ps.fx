sampler2D mTexture0;
sampler2D mTexture1;
sampler2D mTexture2;
sampler2D mTexture3;
float3 mLightAmbient;
float3 mLightDiffuse;
float2 mUVOffset;
float3 mFlowColor;

struct PS_INPUT
{
	float2 mTexCoord       : TEXCOORD0;
	float3 mLightDirection : TEXCOORD1;
	float3 mEyeDirection   : TEXCOORD2;
};

float4 Main( PS_INPUT tInput ) : COLOR
{
	float4 tColor = tex2D( mTexture0, tInput.mTexCoord );
	float3 tNormal = 2.0f * tex2D( mTexture1, tInput.mTexCoord ).xyz - 1.0f;
	float4 tSpecular = tex2D( mTexture2, tInput.mTexCoord );
	float4 tFlow = tex2D( mTexture3, (tInput.mTexCoord + mUVOffset) );
	float3 tLightDirection = normalize( tInput.mLightDirection );
	float3 tDotForDiffuse = saturate( dot( tNormal, tLightDirection ) );
	float3 tEyeDirection = normalize( tInput.mEyeDirection );
	float3 tReflect = reflect( -tLightDirection, tNormal );
	float3 tDotForSpecular = saturate( dot( tEyeDirection, tReflect ) );
	float3 tColor1 = tColor.xyz * ( mLightAmbient * 1.3f  + mLightDiffuse * tDotForDiffuse * 0.4f) + ( tSpecular.xyz * tDotForSpecular * 0.45f);
	float3 tColor2 = tColor1 + mFlowColor * tFlow.xyz * tFlow.w * tSpecular.w * 1.0f;
	return float4( tColor2, tColor.w );

    //float4 tColor = tex2D( mTexture0, tInput.mTexCoord );	
    //float3 tNormal = 2.0f * tex2D( mTexture1, tInput.mTexCoord ).xyz - 1.0f;	
    //float3 tLightDirection = normalize( tInput.mLightDirection );	
    //float3 tDotForDiffuse = saturate( dot( tNormal, tLightDirection ) );
	//return float4( ( tColor.xyz * ( mLightAmbient * 1.3f + mLightDiffuse * tDotForDiffuse * 0.4f ) ), tColor.w );
}