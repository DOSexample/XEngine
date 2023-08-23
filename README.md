# XEngine
 
My C++ D3D9 Renderer Engine
GXDSkin2Loader for TwelveSky2.5, Troy vs Spartar, Waren Story

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

4.Load Skin
```
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
```

5.Load and Compile Shader
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

6.Loop
```
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
```

![Alt text](/Screenshot_0.png?raw=true "TwelveSky2.5")
![Alt text](/Screenshot_1.png?raw=true "Troy vs Spartar")
![Alt text](/Screenshot_2.png?raw=true "Waren Story")
