#pragma once
#include <optional>
#include "raylib.h"

namespace Clock
{
	struct ApplicationSpecification
	{
		const int ScreenWidth = 700;
		const int ScreenHeight = 700;
		const char* Title = "Analog Clock";
	};

	struct ClockSpecification
	{
		const int OuterCircleRadius = 250;
		const int InnerCircleRadius = 210;
		const int MinuteHandLength = 200;
		const int HourHandLength = 180;
		const int SecondHandLength = 230;

		float MinuteHandPosition = 180.0f;
		float HourHandPosition = 180.0f;
		float SecondHandPosition = 180.0f;
	};

	class Application
	{
	public:
		explicit Application(const ApplicationSpecification& spec = ApplicationSpecification(),
							const ClockSpecification& c_spec = ClockSpecification());

		~Application();

		void Run();
		void Draw();
		void Update();

		static void SetIcon();
		static void Loop();

		static Application* s_Instance;

	private:
		const ApplicationSpecification m_a_Specification;
		ClockSpecification m_c_Specification;
		std::optional<Music> m_sound;
		bool m_music_playing = false;
	};
}
