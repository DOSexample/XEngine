#include <X/Core/Win32Native.h>
#include <X/Graphics/Graphic.h>
#include <X/Renderer/Renderer.h>
#include <X/Renderer/Scene.h>

#include <X/Engine/Timer.h>
#include <X/Engine/GameObject.h>
#include <X/Engine/Component/Camera.h>
#include <X/Engine/Component/DXMesh.h>
#include <X/Engine/Component/Transform.h>


#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

int WINAPI
#ifndef _UNICODE
WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
#else
wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
#endif
)
{
    X::Win32Native window;

    if (!window.Initialize(hInstance, TEXT("XSky"), 1024, 768, nShowCmd) )
    {
        MessageBox( nullptr, TEXT("!Win32Native::Initialize()"), TEXT("ERROR"), MB_OK | MB_ICONERROR );
        return 0;
    }

    // Initialize the graphics system.
    X::gfx = new X::Graphic( &window );

    // Initialize the render system.
    X::Renderer renderer;


    MSG msg = {0};

    X::Scene scene;
    int maxFps = -1;

    X::GameObject dxmesh;
    dxmesh.AddComponent<X::DXMesh>();
    scene.AddGameObject(&dxmesh);

    X::GameObject cameraMain;
    cameraMain.AddComponent<X::Camera>();
    scene.AddGameObject(&cameraMain);

    dxmesh.transform->SetParent(cameraMain.transform);

    // Main application loop.
    while (msg.message != WM_QUIT)
    {

        if (!window.ProcessMessages(&msg))
        {
            X::Time.FrameStart(maxFps);
            {
                scene.Update();
                renderer.Draw(*X::gfx, scene, window);

                auto dm = dxmesh.GetComponent<X::DXMesh>();
                DebugA( "DM = %p\r\n", dm );
                if (dm) {
                    //auto position = dm->transform->GetPosition();
                    //position.y += 0.3f * X::Time.deltaTime;
                    //dm->transform->SetPosition(position);

                    auto rot = dm->transform->GetRotation();
                    rot.y += 0.3f * X::Time.deltaTime;
                    dm->transform->SetRotation(rot);
                }

            }
            X::Time.Update();
            X::Time.FrameEnd(maxFps);
        }

    }

    X::gfx->Release();
    
    return static_cast<int>(msg.wParam);
}