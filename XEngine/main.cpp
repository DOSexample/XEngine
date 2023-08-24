#include "XWin32.h"
#include "XEngine.h"
#include "XCamera.h"
#include "XSkinMesh.h"
#include "XShader.h"
#include "XAnimation/XAnimationMotion.h"
#include "XLoader/GXDSkin2Loader.h"
#include "XLoader/GXDMotionLoader.h"
//#include "XPostProcess.h"

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

    if (!xWin32.Initialize(hInstance, "XEngine", 1024, 768))
    {
        return xWin32.MessageBoxA("!XWin32::Initialize()", BUTTON_OK_AND_ICON_ERROR);
    }
    xWin32.Show(nShowCmd);


    if (!xEngine.Initialize(xWin32.GetHWND(), xWin32.GetWidth(), xWin32.GetHeight()))
    {
        return xWin32.MessageBoxA("!XEngine::Initialize()", BUTTON_OK_AND_ICON_ERROR);
    }
    xCam.Initialize( xWin32.GetWidth(), xWin32.GetHeight(), 45.0f, 1.0f, 10000.0f );
    xEngine.SetCamera(&xCam);


    XShader common, animMotion;
    {
        common.AddVertexInput("mWorldViewProjMatrix");
        common.AddVertexInput("mLightDirection");
        common.AddVertexInput("mLightAmbient");
        common.AddVertexInput("mLightDiffuse");
        common.AddPixelInput("mTexture0");
        printf("common: %d\n", SUCCEEDED(common.CreateFromFile("Common", "Shaders\\Common.vs.fx", "Shaders\\Common.ps.fx")));
    }
    //{   //anim 2.5+
    //    animMotion.AddVertexInput("mKeyMatrix");
    //    animMotion.AddVertexInput("mWorldViewProjMatrix");
    //    animMotion.AddVertexInput("mLightDirection");
    //    animMotion.AddVertexInput("mLightAmbient");
    //    animMotion.AddVertexInput("mLightDiffuse");
    //    animMotion.AddPixelInput("mTexture0");
    //    printf("AnimMotion: %d\n", SUCCEEDED(animMotion.CreateFromFile("AnimMotion", "Shaders\\AnimMotion.vs.fx", "Shaders\\AnimMotion.ps.fx")));
    //}
    { //anim 1.0 & 2.0
        animMotion.AddVertexInput("mKeyMatrix");
        animMotion.AddVertexInput("mWorldViewProjMatrix");
        animMotion.AddVertexInput("mLightDirection");
        animMotion.AddVertexInput("mLightAmbient");
        animMotion.AddVertexInput("mLightDiffuse");
        animMotion.AddPixelInput("mTexture0");
        printf("AnimMotion: %d\n", SUCCEEDED(animMotion.CreateFromFile("AnimMotion", "Shaders\\AnimMotion.1.0.vs.fx", "Shaders\\AnimMotion.1.0.ps.fx")));
    }
    //{   //flow
    //    animMotion.AddVertexInput("mKeyMatrix");
    //    animMotion.AddVertexInput("mWorldViewProjMatrix");
    //    animMotion.AddVertexInput("mLightDirection");
    //    animMotion.AddVertexInput("mCameraEye");
    //    animMotion.AddPixelInput("mTexture0");
    //    animMotion.AddPixelInput("mTexture1");
    //    animMotion.AddPixelInput("mTexture2");
    //    animMotion.AddPixelInput("mTexture3");
    //    animMotion.AddPixelInput("mLightAmbient");
    //    animMotion.AddPixelInput("mLightDiffuse");
    //    printf("AnimMotion: %d\n", SUCCEEDED(animMotion.CreateFromFile("AnimMotion", "Shaders\\Flow.vs.fx", "Shaders\\Flow.ps.fx")));
    //}


    std::vector<XSkinMesh*> vSkin = {};
    XLoader::GXDSkin2Loader skin2;
    XSkinMesh hair, face, body, arm, foot;
    {
    //    skin2.Load("SObject\\TwelveSky2\\C001001001.SOBJECT", &hair);
    //    skin2.Load("SObject\\TwelveSky2\\C001002001.SOBJECT", &face);
    //    skin2.Load("SObject\\TwelveSky2\\C001003001.SOBJECT", &body);
    //    skin2.Load("SObject\\TwelveSky2\\C001004001.SOBJECT", &foot);
    //    skin2.Load("SObject\\TwelveSky2GXCW\\A001001039.SOBJECT", &body);//costume

    //    skin2.Load("SObject\\TwelveSky1\\C001001015.SOBJECT", &hair);
    //    skin2.Load("SObject\\TwelveSky1\\C001002006.SOBJECT", &face);
    //    skin2.Load("SObject\\TwelveSky1\\C001003001.SOBJECT", &body);
    //    skin2.Load("SObject\\TwelveSky1\\C001004015.SOBJECT", &foot);

        skin2.Load("SObject\\TwelveSky2MaynGames\\C001001002.SOBJECT", &hair);
        skin2.Load("SObject\\TwelveSky2MaynGames\\C001002002.SOBJECT", &face);
        skin2.Load("SObject\\TwelveSky2MaynGames\\C001003002.SOBJECT", &body);
        skin2.Load("SObject\\TwelveSky2MaynGames\\C001004002.SOBJECT", &foot);

    //    //skin2.Load( "SObject\\Troy vs Sparta\\FC_102_100_001.SOBJECT", &hair);
    //    skin2.Load("SObject\\Troy vs Sparta\\FC_101_101_001.SOBJECT", &face);
    //    skin2.Load("SObject\\Troy vs Sparta\\FC_W_103_101_001.SOBJECT", &hair);//helmet
    //    skin2.Load("SObject\\Troy vs Sparta\\FC_W_104_102_001.SOBJECT", &body);
    //    skin2.Load("SObject\\Troy vs Sparta\\FC_W_105_101_001.SOBJECT", &arm);//arm
    //    skin2.Load("SObject\\Troy vs Sparta\\FC_W_106_101_001.SOBJECT", &foot);
    //
    //    skin2.Load("SObject\\WarenStory\\C001002001.SOBJECT", &hair);
    //    skin2.Load("SObject\\WarenStory\\C001003001.SOBJECT", &face);
    //    skin2.Load("SObject\\WarenStory\\C001008001.SOBJECT", &body);

        vSkin.push_back(&hair);
        vSkin.push_back(&face);
        vSkin.push_back(&body);
        vSkin.push_back(&arm);
        vSkin.push_back(&foot);
    }

    XLoader::GXDMotionLoader motion;
    XAnimationMotion anim;
    {
        //2.0
        //C00?001002 - Level Idle
        //C00?001078 - Master Idle
        //C00?001085 - God Idle
        //printf("motion: %d\n", motion.Load("Motion\\C001001085.MOTION", &anim));
        //printf("motion: %d\n", motion.Load("Motion\\C001001002.MOTION", &anim));

        //2.0
        //printf("motion: %d\n", motion.Load("Motion\\TwelveSky2MaynGames\\C001001085.MOTION", &anim));
        //printf("motion: %d\n", motion.Load("Motion\\C001001002.MOTION", &anim));
        
        //1.0
        //printf("motion: %d\n", motion.Load("Motion\\TwelveSky1\\C001001001.MOTION", &anim));
        //printf("motion: %d\n", motion.Load("Motion\\TwelveSky1\\C001001002.MOTION", &anim));
    }


    //XPostProcess xFilter;
    //{
    //    xFilter.Setup("Shaders\\PostProcessing\\Filter.fx");
    //}

    float oneFrame = 0.033f * 0.1f;
    MSG msg = { 0, 0 };
    while (msg.message != WM_QUIT)
    {
        if (!xWin32.ProcessMessages(&msg))
        {
            auto rot = body.GetRotation();
            rot.y += oneFrame;
            if (rot.y > 360.0f)
                rot.y = 0.0f;
            for (auto it = vSkin.begin(); it != vSkin.end(); ++it)
                (*it)->SetRotation(rot);

            static float testFrame = 0;
            anim.mCurAnimFrame = (int)testFrame;
            testFrame += oneFrame;
            if ((int)testFrame >= anim.mFrameNum)
                testFrame = 0.0f;

            xCam.Update();

            xEngine.Clear(ALL, Grey);
            xEngine.BeginScene();
            {
                for (auto it = vSkin.begin(); it != vSkin.end(); ++it)
                    (*it)->Update();

                for (auto it = vSkin.begin(); it != vSkin.end(); ++it)
                {
                    //Draw with animation motion
                    (*it)->Draw(&anim);
                    //Draw without animation motion
                    //(*it)->Draw();
                }
            }
            //xFilter.Draw(xWin32.GetWidth(), xWin32.GetHeight());
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

    return WinMain( (HINSTANCE)'0', (HINSTANCE)'0', (LPSTR)arg.data(), SW_SHOWDEFAULT);
}