#include "XRenderState.h"

#include "XDevice.h"
#include "XTexture.h"
#include "XShader.h"
#include "XTransform.h"
#include "XAnimation/XAnimationMotion.h"

XRenderState::XRenderState()
    : 
    blendEnabled(false),
    blendMode(BlendMode::AlphaBlend),
    depthTest(DepthTest::Never),
    cullMode(CullMode::Never),
    depthWriteEnabled(true),
    alphaTestEnabled(false),
    alphaTestFunc(AlphaTestFunc::Greater),
    alphaReferenceValue(0)
{

}

void XRenderState::Apply(XTransform* transform, XShader* xs, XAnimationMotion* anim, XTexture* pDiffuseMap, XTexture* pNormalMap, XTexture* pSpecularMap) {
    auto device = &XDevice::Instance();

    // Depth test
    //switch (depthTest) {
    //
    ////case DepthTest::Nothing:
    ////    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    ////    break;
    //
    //case DepthTest::Never:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
    //    break;
    //
    //case DepthTest::Less:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    //    break;
    //
    //case DepthTest::Equal:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
    //    break;
    //
    //case DepthTest::LessEqual:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    //    break;
    //
    //case DepthTest::Greater:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
    //    break;
    //
    //case DepthTest::NotEqual:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_NOTEQUAL);
    //    break;
    //
    //case DepthTest::GreaterEqual:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
    //    break;
    //
    //case DepthTest::Always:
    //    device->SetRenderState(D3DRS_ZENABLE, TRUE);
    //    device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    //    break;
    //}
    //// Depth write
    device->SetRenderState(D3DRS_ZWRITEENABLE, depthWriteEnabled);

    device->SetRenderState(D3DRS_ALPHATESTENABLE, alphaTestEnabled);
    //if (alphaTestEnabled)
    {
        switch (alphaTestFunc) {
        case AlphaTestFunc::Never:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NEVER);
            break;
        case AlphaTestFunc::Less:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
            break;
        case AlphaTestFunc::LessEqual:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESSEQUAL);
            break;
        case AlphaTestFunc::Equal:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_EQUAL);
            break;
        case AlphaTestFunc::Greater:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
            break;
        case AlphaTestFunc::NotEqual:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);
            break;
        case AlphaTestFunc::GreaterEqual:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
            break;
        case AlphaTestFunc::Always:
            device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
            break;
        }
    }
    device->SetRenderState(D3DRS_ALPHAREF, alphaReferenceValue);

    // Blend mode
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, blendEnabled);
    switch (blendMode) {

    case BlendMode::Opaque:
        break;

    case BlendMode::AlphaBlend:
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        break;
    case BlendMode::Additive:
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        break;

    case BlendMode::Multiply:
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
        break;

    case BlendMode::Screen:
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
        break;

    case BlendMode::Subtractive:
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
        break;

    case BlendMode::Replace:
        break;
    }

    // Cull mode
    switch (cullMode) {
    case CullMode::Never:
        break;

    case CullMode::None:
        device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        break;

    case CullMode::Front:
        device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        break;

    case CullMode::Back:
        device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        break;
    }

    int renderType = 1;
    if ( pDiffuseMap && pNormalMap && pSpecularMap )
    {
        renderType = 4;
    }
    else if ( pNormalMap && pNormalMap->mTexture || pSpecularMap && pSpecularMap->mTexture )
    {
        if( !pNormalMap || !pNormalMap->mTexture || !pSpecularMap || !pSpecularMap->mTexture )
            renderType = 3;
        else
            renderType = 2;
    }

    D3DXMATRIX xTRSMatrix = transform->GetWorldMatrix();
    D3DXMATRIX worldMatrix = xTRSMatrix;
    D3DXMATRIX wvp = worldMatrix * device->GetViewMatrix() * device->GetPerspectiveMatrix();
    D3DXMatrixInverse(&worldMatrix, 0, &worldMatrix);

    D3DLIGHT9 light = device->GetDefaultLight();
    D3DCOLORVALUE amb_light = light.Ambient, dif_light = light.Diffuse;
    D3DXVECTOR3 dir_light = light.Direction;
    dir_light.x *= -1.0f;
    dir_light.y *= -1.0f;
    dir_light.z *= -1.0f;

    amb_light.a = 1.0f;
    dif_light.a = 1.0f;

    D3DXVec3TransformNormal( &dir_light, &dir_light, &worldMatrix );
    D3DXVec3Normalize( &dir_light, &dir_light );

    //switch ( renderType )
    //{
    //case 1:
        if (XShader::IsValidVertex(xs))
        {
            if (anim && anim->mFrameNum > 0 && anim->mBoneNum > 0)
            {
                xs->Set(ShaderType::Vertex, ShaderSetType::SetMatrixArray, "mKeyMatrix", anim->mMatrixKey[ anim->mCurAnimFrame ].data(), anim->mBoneNum);
            }
            xs->Set(ShaderType::Vertex, ShaderSetType::SetMatrix, "mWorldViewProjMatrix", &wvp);
            xs->Set(ShaderType::Vertex, ShaderSetType::SetFloatArray, "mLightDirection", (const FLOAT*)&dir_light, 3);
            xs->Set(ShaderType::Vertex, ShaderSetType::SetFloatArray, "mLightAmbient", (const FLOAT*)&amb_light, 3);
            xs->Set(ShaderType::Vertex, ShaderSetType::SetFloatArray, "mLightDiffuse", (const FLOAT*)&dif_light, 3);
        }
        if (XShader::IsValidPixel(xs))
        {
            xs->Set(ShaderType::Pixel, ShaderSetType::SetTexture, "mTexture0", pDiffuseMap);
            xs->Set(ShaderType::Pixel, ShaderSetType::SetTexture, "mTexture1", pNormalMap);
            xs->Set(ShaderType::Pixel, ShaderSetType::SetTexture, "mTexture2", pSpecularMap);

            //for animmotion
            //xs->Set(ShaderType::Pixel, ShaderSetType::SetFloatArray, "mLightAmbient", (const FLOAT*)&amb_light, 3);
            //xs->Set(ShaderType::Pixel, ShaderSetType::SetFloatArray, "mLightDiffuse", (const FLOAT*)&dif_light, 3);
        }
    //    break;
    //}



}
