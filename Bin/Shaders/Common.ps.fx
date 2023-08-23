sampler2D mTexture0;

struct PS_INPUT
{	
    float3 mColor    	   : COLOR;	
    float2 mTexCoord       : TEXCOORD0;
};

float4 Main( PS_INPUT tInput ) : COLOR
{	
    float4 tColor = tex2D( mTexture0, tInput.mTexCoord );	
	return float4( ( tColor.xyz * tInput.mColor ), tColor.w );
}
	