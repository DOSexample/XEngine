#include "XRenderPass.h"
#include "XRenderState.h"

void XRenderPass::SetRenderState(XRenderState* rt)
{
    rt->Apply();
}