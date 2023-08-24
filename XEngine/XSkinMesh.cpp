#include "XSkinMesh.h"

#include "XDevice.h"

#include "XRenderState.h"
#include "XShader.h"
#include "XTransform.h"
#include "XAnimation/XAnimationMotion.h"

struct SkinVertex1Imp
{
    SkinVertex1 VNT;
    float mBlendValue[4];
    int mBoneIndex[4];
    //SkinWeight BLEND;

    //float mV[3];
    //float mBlendWeight[4];
    //int mBlendIndex[4];
    //float mN[3];
    //float mT[2];

};

XSkinMesh::XSkinMesh()
    :
    mSkinVersion(V0), 
    v1(NULL),
    v2(NULL)
{
}

XSkinMesh::~XSkinMesh()
{
}

void XSkinMesh::ReleaseV1()
{
    if (v1)
    {
        for (int i = 0; i < (int)v1->mSkin.size(); ++i)
        {
            auto s = &v1->mSkin[i];

            SAFE_RELEASE( s->mVertexBuffer );
            SAFE_RELEASE( s->mIndexBuffer );

            s->vertices.clear();
            s->indices.clear();
            s->mMotionVertex.clear();
            s->mMotionNormal.clear();

            s->mDiffuseMap.Free();
            s->mSpecularMap.Free();

            for (int j = 0; j < (int)s->mAnimationMap.size(); ++j)
            {
                s->mAnimationMap[j].Free();
            }
        }
        delete v1;
        v1 = NULL;
    }
}

void XSkinMesh::ReleaseV2()
{
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
            s->mFlowMap.Free();

            for (int j = 0; j < (int)s->mAnimationMap.size(); ++j)
            {
                s->mAnimationMap[j].Free();
            }

        }
        delete v2;
        v2 = NULL;
    }
}

void XSkinMesh::Release()
{
    printf("XSkinMesh::Release()\n");
    ReleaseV1();
    ReleaseV2();
}

void XSkinMesh::Create1(bool isSuccess)
{
    printf("XSkinMesh::Create1(%d)\n", isSuccess);
    if (!isSuccess)
    {
        Release();
        return;
    }

    auto device = &XDevice::Instance();
    mSkinVersion = V1;
    for (int i = 0; i < (int)v1->mSkin.size(); ++i)
    {
        auto s = &v1->mSkin[i];

        auto v = s->vertices.data();
        auto w = s->weights.data();
        std::vector<SkinVertex1Imp> newVertices = {};
        for (int j = 0; j < (int)s->vertices.size(); ++j)
        {
            newVertices.push_back( SkinVertex1Imp{ v[j], 
                //w[j] 
                w[j].mBlendValue[0], w[j].mBlendValue[1], w[j].mBlendValue[2], w[j].mBlendValue[3],
                w[j].mBoneIndex[0], w[j].mBoneIndex[1], w[j].mBoneIndex[2], w[j].mBoneIndex[3],
                } );
            //newVertices.push_back( SkinVertex1Imp{ 
            //    v[j].mV[0], v[j].mV[1], v[j].mV[2],
            //    w[j].mBlendValue[0], w[j].mBlendValue[1], w[j].mBlendValue[2], w[j].mBlendValue[3],
            //    w[j].mBoneIndex[0], w[j].mBoneIndex[1], w[j].mBoneIndex[2], w[j].mBoneIndex[3],
            //    v[j].mN[0], v[j].mN[1], v[j].mN[2],
            //    v[j].mT[0], v[j].mT[1]
            //    }
            //);
        }
        int vtxSize = int( sizeof(SkinVertex1Imp) * newVertices.size() );
        int idxSize = int( sizeof(WORD) * s->indices.size() );

        void* vbuffer = NULL;
        device->CreateVertexBuffer( (UINT)vtxSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &s->mVertexBuffer, 0 );
        s->mVertexBuffer->Lock( 0, 0, &vbuffer, 0 );
        memcpy( vbuffer, newVertices.data(), vtxSize );
        s->mVertexBuffer->Unlock();

        //clear to save memory?
        //s->vertices.clear();

        void* ibuffer = NULL;
        device->CreateIndexBuffer( (UINT)idxSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &s->mIndexBuffer, 0 );
        s->mIndexBuffer->Lock( 0, 0, &ibuffer, 0 );
        memcpy( ibuffer, s->indices.data(), idxSize );
        s->mIndexBuffer->Unlock();

        //clear to save memory?
        //s->indices.clear();

        printf("Creating skin[%d]: vtx: %d, idx: %d\n", i, vtxSize, idxSize);
    }
}

void XSkinMesh::Create2(bool isSuccess)
{
    printf("XSkinMesh::Create2(%d)\n", isSuccess);
    if (!isSuccess)
    {
        Release();
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

void XSkinMesh::Draw(XAnimationMotion* anim)
{
    //printf( "XSkinMesh::Draw() -> Version: %d, v1: %p, v2: %p\r\n", mSkinVersion, v1, v2 );
    switch ( mSkinVersion )
    {
    case V1:
        if (v1 == NULL)
            return;
        break;
    case V2:
        if (v2 == NULL)
            return;
        break;
    default:
        return;
    }

    //printf("XSkinMesh::Draw()\n");
	auto device = &XDevice::Instance();

    XAnimationMotion* xa = nullptr;
    XShader* xs = nullptr;

    if (anim && anim->mFrameNum > 0 && anim->mBoneNum > 0) {
        xs = XShaderManager::Get("AnimMotion");
        xa = anim;
    }
    else
        xs = XShaderManager::Get("Common");
    
    switch ( mSkinVersion )
    {
        case V1:
        {
            //int i = 0;

            for (int i = 0; i < (int)v1->mSkin.size(); ++i)
            {
                auto s = &v1->mSkin[i];
                XTexture* pDiffuseMap = nullptr;
                XTexture* pNormalMap = nullptr;
                XTexture* pSpecularMap = nullptr;
                XTexture* pFlowMap = nullptr;

                //auto ReadyForDraw = [](SkinData1* s, const std::vector<D3DXMATRIX> mx, bool tCheckCalculateNormal) -> void {
                //
                //    for (int i = 0; i < (int)s->vertices.size(); i++)
                //    {
                //        SkinWeight* w = &s->weights[i];
                //        SkinVertex1* v = &s->vertices[i];
                //        float* mv = (float*)&s->mMotionVertex[i];
                //        float* mn = (float*)&s->mMotionNormal[i];
                //
                //        D3DXVECTOR3 bi0, bi1, bi2, bi3;
                //
                //        bi0 = D3DXVECTOR3(0, 0, 0);
                //        bi1 = D3DXVECTOR3(0, 0, 0);
                //        bi2 = D3DXVECTOR3(0, 0, 0);
                //        bi3 = D3DXVECTOR3(0, 0, 0);
                //        D3DXVec3TransformCoord(&bi0, (D3DXVECTOR3*)mv, &mx[w->mBoneIndex[0]]);
                //        D3DXVec3TransformCoord(&bi1, (D3DXVECTOR3*)mv, &mx[w->mBoneIndex[1]]);
                //        if (w->mBlendValue[2] > 0.0f)
                //            D3DXVec3TransformCoord(&bi2, (D3DXVECTOR3*)mv, &mx[w->mBoneIndex[2]]);
                //        if (w->mBlendValue[3] > 0.0f)
                //            D3DXVec3TransformCoord(&bi3, (D3DXVECTOR3*)mv, &mx[w->mBoneIndex[3]]);
                //        v->mV[0] = bi0.x * w->mBlendValue[0]
                //            + bi1.x * w->mBlendValue[1]
                //            + bi2.x * w->mBlendValue[2]
                //            + bi3.x * w->mBlendValue[3];
                //        v->mV[1] = bi0.y * w->mBlendValue[0]
                //            + bi1.y * w->mBlendValue[1]
                //            + bi2.y * w->mBlendValue[2]
                //            + bi3.y * w->mBlendValue[3];
                //        v->mV[2] = bi0.z * w->mBlendValue[0]
                //            + bi1.z * w->mBlendValue[1]
                //            + bi2.z * w->mBlendValue[2]
                //            + bi3.z * w->mBlendValue[3];
                //
                //        bi0 = D3DXVECTOR3(0, 0, 0);
                //        bi1 = D3DXVECTOR3(0, 0, 0);
                //        bi2 = D3DXVECTOR3(0, 0, 0);
                //        bi3 = D3DXVECTOR3(0, 0, 0);
                //        D3DXVec3TransformNormal(&bi0, (D3DXVECTOR3*)mn, &mx[w->mBoneIndex[0]]);
                //        D3DXVec3TransformNormal(&bi1, (D3DXVECTOR3*)mn, &mx[w->mBoneIndex[1]]);
                //        if (w->mBlendValue[2] > 0.0f)
                //            D3DXVec3TransformNormal(&bi2, (D3DXVECTOR3*)mn, &mx[w->mBoneIndex[2]]);
                //        if (w->mBlendValue[3] > 0.0f)
                //            D3DXVec3TransformNormal(&bi3, (D3DXVECTOR3*)mn, &mx[w->mBoneIndex[3]]);
                //        v->mN[0] = bi0.x * w->mBlendValue[0]
                //            + bi1.x * w->mBlendValue[1]
                //            + bi2.x * w->mBlendValue[2]
                //            + bi3.x * w->mBlendValue[3];
                //        v->mN[1] = bi0.y * w->mBlendValue[0]
                //            + bi1.y * w->mBlendValue[1]
                //            + bi2.y * w->mBlendValue[2]
                //            + bi3.y * w->mBlendValue[3];
                //        v->mN[2] = bi0.z * w->mBlendValue[0]
                //            + bi1.z * w->mBlendValue[1]
                //            + bi2.z * w->mBlendValue[2]
                //            + bi3.z * w->mBlendValue[3];
                //    }
                //};
                //
                //if(xa)
                //    ReadyForDraw( s, xa->mMatrixKey[xa->mCurAnimFrame], TRUE );

                //if has skin safe texture, or skin[0] has no skin texture, dont use share texture
                //so use currrent skin texture
                if (s->mDiffuseMap.mTexture || !&v2->mSkin[0].mDiffuseMap.mTexture)
                {
                    pDiffuseMap = &s->mDiffuseMap;
                    //pNormalMap = &s->mNormalMap;
                    pSpecularMap = &s->mSpecularMap;
                    //pFlowMap = &s->mFlowMap;
                }
                else
                {
                    //use share (1st skin) texture
                    pDiffuseMap = &v2->mSkin[0].mDiffuseMap;
                    //pNormalMap = &v2->mSkin[0].mNormalMap;
                    pSpecularMap = &v2->mSkin[0].mSpecularMap;
                    //pFlowMap = &v2->mSkin[0].mFlowMap;
                }
                //set render pass/state
                    //XRenderPass rp;
                XRenderState rt;
                
                //printf( "skin[%d] -> pDiffuseMap.Alpha: %d\r\n", i, pDiffuseMap->mAlphaModeCase );
                switch (pDiffuseMap->mAlphaModeCase)
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
                rt.Apply2(this, xs, xa, pDiffuseMap, pNormalMap, pSpecularMap, pFlowMap);
                //rp.SetRenderState(&rt);

                //printf("Drawing skin[%d]\n", i);
                device->SetStreamSource(0, s->mVertexBuffer, 0, sizeof(SkinVertex1Imp) );
                device->SetIndices( s->mIndexBuffer );
                device->SetVertexDeclaration( VertexDeclaretion::VNT_AND_BLEND );
                device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, (UINT)s->mVertexNum, 0, (UINT)s->mIndexNum);


                //const DWORD alphaRef = 128;
                //bool bAlphaTest = false;
                //bool bAlphaBlend = false;
                //if (pDiffuseMap)
                //{
                //    if (pDiffuseMap->mAlphaModeCase == 1)
                //    {
                //        bAlphaTest = 1;
                //        device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
                //        device->SetRenderState(D3DRS_ALPHAREF, alphaRef);
                //    }
                //    else if (pDiffuseMap->mAlphaModeCase == 2)
                //    {
                //        bAlphaBlend = 1;
                //        device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
                //        device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
                //    }
                //}
                //device->SetTexture(0, pDiffuseMap);
                //device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, s->mVertexNum, s->mIndexNum, s->mIndexBuffer, D3DFMT_INDEX16, s->mVertexBuffer, sizeof(SkinVertex1Imp));
                //device->SetStreamSource(0, s->mVertexBuffer, 0, sizeof(SkinVertex1Imp));
                //device->SetIndices(s->mIndexBuffer);
                //device->SetVertexDeclaration(VertexDeclaretion::VNT_AND_BLEND);
                //device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, (UINT)s->mVertexNum, 0, (UINT)s->mIndexNum);
                //
                //if (bAlphaBlend)
                //{
                //    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                //    device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
                //}
                //if (bAlphaTest)
                //{
                //    device->SetRenderState(D3DRS_ALPHAREF, 0);
                //    device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
                //}
            }
        }
        break;

        case V2:
        {
            //int i = 0;
            for (int i = 0; i < (int)v2->mSkin.size(); ++i)
            {
                auto s = &v2->mSkin[i];

                XTexture* pDiffuseMap = nullptr;
                XTexture* pNormalMap = nullptr;
                XTexture* pSpecularMap = nullptr;
                XTexture* pFlowMap = nullptr;
                
                //if has skin safe texture, or skin[0] has no skin texture, dont use share texture
                //so use currrent skin texture
                if ( s->mDiffuseMap.mTexture || !&v2->mSkin[0].mDiffuseMap.mTexture )
                {
                    pDiffuseMap = &s->mDiffuseMap;
                    pNormalMap = &s->mNormalMap;
                    pSpecularMap = &s->mSpecularMap;
                    pFlowMap = &s->mFlowMap;
                }
                else
                {
                    //use share (1st skin) texture
                    pDiffuseMap = &v2->mSkin[0].mDiffuseMap;
                    pNormalMap = &v2->mSkin[0].mNormalMap;
                    pSpecularMap = &v2->mSkin[0].mSpecularMap;
                    pFlowMap = &v2->mSkin[0].mFlowMap;
                }

                //draw skin mesh (LOD by index)
                int j = 0;
                //for (int j = 0; j < (int)s->mLOD.size(); ++j)
                {
                    auto l = &s->mLOD[j];

                    //set render pass/state
                    //XRenderPass rp;
                    XRenderState rt;
                    
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
                    rt.Apply2(this, xs, xa, pDiffuseMap, pNormalMap, pSpecularMap, pFlowMap);
                    ////rp.SetRenderState(&rt);
                    
                    //printf("Drawing skin[%d].lod[%d]\n", i, j);
                    device->SetStreamSource(0, l->mVertexBuffer, 0, sizeof(SkinVertex2));
                    device->SetIndices(l->mIndexBuffer);
                    device->SetVertexDeclaration( VertexDeclaretion::ALL );
                    device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, (UINT)l->mVertexNum, 0, (UINT)l->mIndexNum);
                }
            }
        }
        break;
    }

}
