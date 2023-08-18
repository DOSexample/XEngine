#include <X/Renderer/Renderer.h>
#include <X/Renderer/Scene.h>

#include <X/Core/Win32Native.h>
#include <X/Graphics/Graphic.h>

#include <X/Engine/Object.h>
#include <X/Engine/GameObject.h>
#include <X/Engine/Timer.h>

#include <X/Engine/Component/DXMesh.h>

#include <string>

namespace X {

	Renderer::Renderer()
	{
	}

    void Renderer::Draw(Graphic& gfx, Scene& scene, Win32Native& win32)
    {
        win32.SetTitle( std::wstring(win32.GetTitleW()).append( TEXT(" | FPS : ") ).append(std::to_wstring(Time.GetFPS())).c_str());

        gfx.BeginScene(D3DCOLOR_RGBA(0, 50, 50, 0));
        // Here you'd typically set up shaders, textures, draw calls, etc.
        // Just as an example:

        // m_pContext->SetVertexShader(SomeVertexShader);
        // m_pContext->SetPixelShader(SomePixelShader);

        for (auto obj : scene.m_gameObjects) {

            for (auto com : obj->components)
            {
                EXOBJECT_TYPE type = ((Object*)com.get())->m_Type;
                DebugA("obj >> m_Type: %d\r\n", type);

                switch ( type )
                {
                case XOBJ_DXMESH:
                    ((DXMesh*)com.get())->Draw();
                    break;
                }
            }
        }

        gfx.EndScene();
    }
}