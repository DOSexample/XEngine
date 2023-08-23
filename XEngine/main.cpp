#include "XWin32.h"
#include "XEngine.h"
#include "XCamera.h"
#include "XSkinMesh.h"
#include "XShader.h"
#include "XLoader/GXDSkin2Loader.h"

#include <string>

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {

    XWin32 xWin32;
    auto& xEngine = XEngine::Instance();
    XCamera xCam;

    if (!xWin32.Initialize(hInstance, "Viewer", 1024, 768))
    {
        return xWin32.MessageBoxA("!XWin32::Initialize()", BUTTON_OK_AND_ICON_ERROR);
    }
    xWin32.Show(nShowCmd);

    if (!xEngine.Initialize(xWin32.GetHWND(), xWin32.GetWidth(), xWin32.GetHeight()))
    {
        return xWin32.MessageBoxA("!xd3d9::Initialize()", BUTTON_OK_AND_ICON_ERROR);
    }
    xCam.Initialize( xWin32.GetWidth(), xWin32.GetHeight(), 45.0f, 1.0f, 10000.0f );
    xEngine.SetCamera(&xCam);

    XLoader::GXDSkin2Loader skin2;
    XSkinMesh hair, face, body, foot;

    skin2.Load( "SObject\\C001001001.SOBJECT", &hair);
    skin2.Load( "SObject\\C001002001.SOBJECT", &face);
    skin2.Load( "SObject\\C001003001.SOBJECT", &body);
    skin2.Load( "SObject\\C001004001.SOBJECT", &foot);

    XShader common;
    {
        common.AddVertexInput("mKeyMatrix");
        common.AddVertexInput("mWorldViewProjMatrix");
        common.AddVertexInput("mLightDirection");
        common.AddVertexInput("mLightAmbient");
        common.AddVertexInput("mLightDiffuse");
        common.AddPixelInput("mTexture0");
        printf( "common: %d\n", SUCCEEDED(common.CreateFromFile("Common", "Shaders\\Common.vs.fx", "Shaders\\Common.ps.fx") ) );
    }

    MSG msg = { 0, 0 };
    while (msg.message != WM_QUIT)
    {
        if (!xWin32.ProcessMessages(&msg))
        {
            auto rot = body.GetRotation();
            rot.y += 0.16f * 0.1f;
            if (rot.y > 360.0f)
                rot.y = 0.0f;
            hair.SetRotation(rot);
            face.SetRotation(rot);
            body.SetRotation(rot);
            foot.SetRotation(rot);

            xCam.Update();

            xEngine.Clear(ALL, Grey);
            xEngine.BeginScene();
            {
                hair.Update();
                face.Update();
                body.Update();
                foot.Update();

                hair.Draw();
                face.Draw();
                body.Draw();
                foot.Draw();
            }
            xEngine.EndScene();
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    std::string arg = "";
    for (int i = 1; i < argc; ++i)
    {
        arg += argv[i];
        if (i != argc - 1)
            arg += " ";
    }

    return WinMain( (HINSTANCE)NULL, (HINSTANCE)NULL, (LPSTR)arg.data(), SW_SHOWDEFAULT );
}