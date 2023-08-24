# XEngine
 
My C++ D3D9 Renderer Engine

GXDSkin2Loader for TwelveSky1.0,TwelveSky2.0,TwelveSky2.5, TwelveSkyGXCW(XTEA1,XXTEA111,XXTEA222), Troy vs Spartar, Waren Story

1.Create Window
```
XWin32 xWin32;
xWin32.Initialize(hInstance, "Viewer", 1024, 768);
xWin32.Show(nShowCmd);
```

2.Init Engine(Device)
```
auto& xEngine = XEngine::Instance();
xEngine.Initialize(xWin32.GetHWND(), xWin32.GetWidth(), xWin32.GetHeight());
```

3.Init Camera and set main camera
```
XCamera xCam;
xCam.Initialize( xWin32.GetWidth(), xWin32.GetHeight(), 45.0f, 1.0f, 10000.0f );
xEngine.SetCamera(&xCam);
```

4.Load and Compile Shader
```
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
```

5.Load Skin
```
std::vector<XSkinMesh*> vSkin = {};
XLoader::GXDSkin2Loader skin2;
XSkinMesh hair, face, body, arm, foot;
{
    skin2.Load("SObject\\TwelveSky2\\C001001001.SOBJECT", &hair);
    skin2.Load("SObject\\TwelveSky2\\C001002001.SOBJECT", &face);
    skin2.Load("SObject\\TwelveSky2\\C001003001.SOBJECT", &body);
    skin2.Load("SObject\\TwelveSky2\\C001004001.SOBJECT", &foot);

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
    //C00?001002 - Level Idle
    //C00?001078 - Master Idle
    //C00?001085 - God Idle
    printf("motion: %d\n", motion.Load("Motion\\C001001085.MOTION", &anim));
}
```

6.Load Motion
```
XLoader::GXDMotionLoader motion;
XAnimationMotion anim;
{
	//C00?001002 - Level Idle
	//C00?001078 - Master Idle
	//C00?001085 - God Idle
	printf("motion: %d\n", motion.Load("Motion\\C001001085.MOTION", &anim));
}
```

N.Loop
```
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
        if ( (int)testFrame >= anim.mFrameNum )
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
        xEngine.EndScene();
    }
}
```

Troy vs Sparta
![Alt text](/Screenshot_TroySparta.png?raw=true "Troy vs Spartar

Waren Story
![Alt text](/Screenshot_WS.png?raw=true "Waren Story")

TwelveSky1.0
![Alt text](/Screenshot_TS1.png?raw=true "TwelveSky1.0")

TwelveSky2.0
![Alt text](/Screenshot_TS2.png?raw=true "TwelveSky2.0")

TwelveSky2.5
![Alt text](/Screenshot_TS25.png?raw=true "TwelveSky2.5")


