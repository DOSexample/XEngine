#include <X/Graphics/Graphic.h>
#include <X/Graphics/GraphicD3D9.h>

namespace X {

    Graphic* gfx = nullptr;

    Graphic::Graphic(Win32Native* win32native) : GraphicD3D9(win32native)
    {
    }

    Graphic::~Graphic() {
    }

}

