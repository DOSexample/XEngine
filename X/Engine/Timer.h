#pragma once

#include <chrono>

namespace X {

	class Timer
	{
	public:
		Timer();
		~Timer() = default;

		void Update();

		int GetFPS() const {
			return m_fps;
		}

		void FrameStart(int maxFps = -1);
		void FrameEnd(int maxFps = -1);

	public:
		int m_frameCount;
		int m_fps;

		float deltaTime;
		std::chrono::high_resolution_clock::time_point m_lastDeltaTime;
		std::chrono::high_resolution_clock::time_point m_lastFPSTime;


	private:
		std::chrono::high_resolution_clock::time_point frameStart;
	};

	extern Timer Time;
}