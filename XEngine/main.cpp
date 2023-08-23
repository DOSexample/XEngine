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
    XSkinMesh hair, face, body, arm, foot;

//    skin2.Load( "SObject\\TwelveSky2\\C001001001.SOBJECT", &hair);
//    skin2.Load( "SObject\\TwelveSky2\\C001002001.SOBJECT", &face);
//    skin2.Load( "SObject\\TwelveSky2\\C001003001.SOBJECT", &body);
//    skin2.Load( "SObject\\TwelveSky2\\C001004001.SOBJECT", &foot);
//
//    //skin2.Load( "SObject\\Troy vs Sparta\\FC_102_100_001.SOBJECT", &hair);
//    skin2.Load( "SObject\\Troy vs Sparta\\FC_101_101_001.SOBJECT", &face);
//    skin2.Load( "SObject\\Troy vs Sparta\\FC_W_103_101_001.SOBJECT", &hair);//helmet
//    skin2.Load( "SObject\\Troy vs Sparta\\FC_W_104_102_001.SOBJECT", &body);
//    skin2.Load( "SObject\\Troy vs Sparta\\FC_W_105_101_001.SOBJECT", &arm);//arm
//    skin2.Load( "SObject\\Troy vs Sparta\\FC_W_106_101_001.SOBJECT", &foot);
//
//    skin2.Load("SObject\\WarenStory\\C001002001.SOBJECT", &hair);
//    skin2.Load("SObject\\WarenStory\\C001003001.SOBJECT", &face);
//    skin2.Load("SObject\\WarenStory\\C001008001.SOBJECT", &body);

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

    std::vector<XSkinMesh*> vSkin = {};
    vSkin.push_back( &hair );
    vSkin.push_back( &face );
    vSkin.push_back( &body );
    vSkin.push_back( &arm  );
    vSkin.push_back( &foot );

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
                for (auto it = vSkin.begin(); it != vSkin.end(); ++it)
                    (*it)->Update();

                for (auto it = vSkin.begin(); it != vSkin.end(); ++it)
                    (*it)->Draw();
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