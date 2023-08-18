#pragma once

namespace X {

	class Graphic;
	class Scene;
	class Win32Native;

	class Renderer
	{
	public:
		Renderer();
		~Renderer() = default;

		void Draw(Graphic& gfx, Scene& scene, Win32Native& win32);

	};

}