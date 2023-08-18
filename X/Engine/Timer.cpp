#include <X/Engine/Timer.h>

#include <thread>

namespace X {

	Timer Time;

	Timer::Timer() : 
		m_lastDeltaTime(std::chrono::high_resolution_clock::now()), 
		m_lastFPSTime(std::chrono::high_resolution_clock::now()), 
		deltaTime(0.0f)
	{}

	void Timer::Update()
	{
		m_frameCount++;

		// Calculate deltaTime.
		auto now = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(now - m_lastDeltaTime).count();
		m_lastDeltaTime = now;

		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastFPSTime).count();

		if (elapsed >= 1) { // Every second
			m_fps = m_frameCount;
			m_frameCount = 0;
			m_lastFPSTime = now;
		}

	}

	void Timer::FrameStart(int maxFps)
	{
		if (maxFps < 1) return;

		auto frameStart = std::chrono::high_resolution_clock::now();
	}

	void Timer::FrameEnd(int maxFps)
	{
		if (maxFps < 1) return;

		const int desiredFrameTime = 1000 / maxFps;  // Time per frame for 30 FPS, in milliseconds

		auto frameEnd = std::chrono::high_resolution_clock::now();
		auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

		if (frameDuration < desiredFrameTime) {
			std::this_thread::sleep_for(std::chrono::milliseconds(desiredFrameTime - frameDuration));
		}
	}

}