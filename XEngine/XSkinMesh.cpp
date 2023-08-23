#include "XSkinMesh.h"

#include "XDevice.h"

#include "XRenderState.h"
#include "XShader.h"
#include "XTransform.h"

XSkinMesh::XSkinMesh()
    :
    mSkinVersion(V0), v2(NULL)
{
}

XSkinMesh::~XSkinMesh()
{
}

void XSkinMesh::Release()
{
    printf("XSkinMesh::Release()\n");
    if (v2)
    {
        for (int i = 0; i < (int)v2->mSkin.size(); ++i)
        {
            auto s = &v2->mSkin[i];

            for (int j = 0; j < (int)v2->mSkin[i].mLOD.size(); ++j)
            {
                auto l = &s->mLOD[j];

                SAFE_RELEASE( l->mVertexBuffer );
                SAFE_RELEASE( l->mIndexBuffer );

                l->vertices.clear();
                l->indices.clear();
            }

            s->mDiffuseMap.Free();
            s->mNormalMap.Free();
            s->mSpecularMap.Free();

            for (int j = 0; j < (int)s->mAnimationMap.size(); ++j)
            {
                s->mAnimationMap[j].Free();
            }

        }
        delete v2;
        v2 = NULL;
    }
}

void XSkinMesh::Create2(bool isSuccess)
{
    printf("XSkinMesh::Create2(%d)\n", isSuccess);
    if (!isSuccess)
    {
        if (v2)
            delete v2;
        v2 = NULL;
        return;
    }

    auto device = &XDevice::Instance();
    mSkinVersion = V2;

    for (int i = 0; i < (int)v2->mSkin.size(); ++i)
    {
        for (int j = 0; j < (int)v2->mSkin[i].mLOD.size(); ++j)
        {

            auto s = &v2->mSkin[i].mLOD[j];
            int vtxSize = int(sizeof(SkinVertex2) * s->vertices.size());
            int idxSize = int(sizeof(WORD) * s->indices.size());

            void* vbuffer = NULL;
            device->CreateVertexBuffer( (UINT)vtxSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &s->mVertexBuffer, 0);
            s->mVertexBuffer->Lock(0, 0, &vbuffer, 0);
            memcpy(vbuffer, s->vertices.data(), vtxSize);
            s->mVertexBuffer->Unlock();

            //clear to save memory?
            //s->vertices.clear();

            void* ibuffer = NULL;
            device->CreateIndexBuffer( (UINT)idxSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &s->mIndexBuffer, 0 );
            s->mIndexBuffer->Lock(0, 0, &ibuffer, 0);
            memcpy(ibuffer, s->indices.data(), idxSize);
            s->mIndexBuffer->Unlock();

            //clear to save memory?
            //s->indices.clear();

            printf("Creating skin[%d].lod[%d] : vtx: %d, idx: %d\n", i, j, vtxSize, idxSize);
        }
    }
}

void XSkinMesh::Draw()
{
    switch ( mSkinVersion )
    {
    case V2:
        if (v2 == NULL)
            return;
        break;
    default:
        return;
    }

    //printf("XSkinMesh::Draw()\n");
	auto device = &XDevice::Instance();

    
    switch ( mSkinVersion )
    {
        case V2:
        {
            //int i = 0;
            for (int i = 0; i < (int)v2->mSkin.size(); ++i)
            {
                auto s = &v2->mSkin[i];

                XTexture* pDiffuseMap = nullptr;
                XTexture* pNormalMap = nullptr;
                XTexture* pSpecularMap = nullptr;
                
                //if has skin safe texture, or skin[0] has no skin texture, dont use share texture
                //so use currrent skin texture
                if ( s->mDiffuseMap.mTexture || !&v2->mSkin[0].mDiffuseMap.mTexture )
                {
                    pDiffuseMap = &s->mDiffuseMap;
                    pNormalMap = &s->mNormalMap;
                    pSpecularMap = &s->mSpecularMap;
                }
                else
                {
                    //use share (1st skin) texture
                    pDiffuseMap = &v2->mSkin[0].mDiffuseMap;
                    pNormalMap = &v2->mSkin[0].mNormalMap;
                    pSpecularMap = &v2->mSkin[0].mSpecularMap;
                }

                //draw skin mesh (LOD by index)
                int j = 0;
                //for (int j = 0; j < (int)s->mLOD.size(); ++j)
                {
                    auto l = &s->mLOD[j];

                    //set render pass/state
                    //XRenderPass rp;
                    XRenderState rt;
                    XShader* xs = XShaderManager::Get("Common");
                    
                    //printf( "skin[%d] -> pDiffuseMap.Alpha: %d\r\n", i, pDiffuseMap->mAlphaModeCase );
                    
                    switch ( pDiffuseMap->mAlphaModeCase )
                    {
                    //case 0:
                    //    //device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
                    //    //device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
                    //    //device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                    //    break;
                    case 1:
                        rt.EnableAlphaTest(true);
                        rt.SetAlphaTestFunc(XRenderState::AlphaTestFunc::Greater);
                        rt.SetAlphaReferenceValue(128);//128 / 255 = 0.5 //Alpha Clip (0.5)
                        //device->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
                        //device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
                        //device->SetRenderState( D3DRS_ALPHAREF, 128 );
                        break;
                    case 2:
                        rt.SetDepthWrite(false);
                        rt.SetBlendMode(XRenderState::BlendMode::AlphaBlend);
                        //device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
                        //device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                        //device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
                        //device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
                        break;
                    }
                    rt.Apply(this, xs, pDiffuseMap, pNormalMap, pSpecularMap);
                    ////rp.SetRenderState(&rt);
                    
                    //printf("Drawing skin[%d].lod[%d]\n", i, j);
                    device->SetStreamSource(0, l->mVertexBuffer, 0, sizeof(SkinVertex2));
                    device->SetIndices(l->mIndexBuffer);
                    device->SetVertexDeclaration();
                    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, (UINT)l->mVertexNum, 0, (UINT)l->mIndexNum);
                }
            }
        }
        break;
    }

}
