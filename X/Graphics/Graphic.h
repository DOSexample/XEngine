#pragma once

#include <X/Graphics/GraphicD3D9.h>

#ifndef SAFE_RELEAE
#define SAFE_RELEAE( x ) if( x != nullptr ) { x->Release(); x = nullptr; }
#endif

template<typename T>
inline void SafeRelease(T t)
{
    if (t != nullptr) { t->Release(); t = nullptr; }
}

namespace X {

    class Graphic : public GraphicD3D9 {
    public:
        Graphic(Win32Native* win32native);
        ~Graphic();

    };

    extern Graphic* gfx;
}


