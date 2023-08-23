# XEngine
 
My C++ D3D9 Renderer Engine
GXDSkin2Loader for TwelveSky2.5

1.
XWin32 xWin32;
xWin32.Initialize(hInstance, "Viewer", 1024, 768);
xWin32.Show(nShowCmd);

2.
auto& xEngine = XEngine::Instance();
xEngine.Initialize(xWin32.GetHWND(), xWin32.GetWidth(), xWin32.GetHeight());

3.
XCamera xCam;
xCam.Initialize( xWin32.GetWidth(), xWin32.GetHeight(), 45.0f, 1.0f, 10000.0f );
xEngine.SetCamera(&xCam);

4.
XLoader::GXDSkin2Loader skin2;
XSkinMesh hair, face, body, foot;

skin2.Load( "SObject\\C001001001.SOBJECT", &hair);
skin2.Load( "SObject\\C001002001.SOBJECT", &face);
skin2.Load( "SObject\\C001003001.SOBJECT", &body);
skin2.Load( "SObject\\C001004001.SOBJECT", &foot);

5.
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

6.
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

![Alt text](/Screenshot_0.png?raw=true "Optional Title")