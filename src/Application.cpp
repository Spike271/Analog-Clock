#include "raylib.h"
#include "../headers/Application.hpp"

#include <ctime>
#include <iostream>
#include <ostream>

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

namespace Clock
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec, const ClockSpecification& c_spec) : m_a_Specification(spec), m_c_Specification(c_spec)
	{
		InitAudioDevice();
		s_Instance = this;
		m_sound = LoadMusicStream("./res/sound.wav");
	}

	Application::~Application()
	{
		if (m_sound.has_value()) UnloadMusicStream(*m_sound);
		CloseAudioDevice();
	}

	void Application::Run()
	{
		InitWindow(m_a_Specification.ScreenWidth, m_a_Specification.ScreenHeight, m_a_Specification.Title);
		if (!IsAudioDeviceReady())
		{
			std::cerr << "Failed to initialize audio device" << std::endl;
			return;
		}

		if (m_sound.has_value()) PlayMusicStream(*m_sound);

#ifndef PLATFORM_WEB
		SetConfigFlags(FLAG_MSAA_4X_HINT);
		SetIcon();
#endif
		SetExitKey(0);

#if defined(PLATFORM_WEB)
		emscripten_set_main_loop(Loop, 0, 1);
#else
		SetTargetFPS(GetMonitorRefreshRate(0));

		while (!WindowShouldClose())
		{
			Draw();
		}
#endif

		CloseWindow();
	}

	void Application::Draw()
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);
		Update();

		DrawCircleV(Vector2(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2), m_c_Specification.OuterCircleRadius, BLACK);
		DrawCircleV(Vector2(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2), m_c_Specification.OuterCircleRadius - 20, WHITE);

		for (int i = 0; i < 12; i++)
		{
			DrawRectanglePro(Rectangle(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2, 4, m_c_Specification.OuterCircleRadius), Vector2(0, 0), i * 30, BLACK);
		}

		DrawCircleV(Vector2(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2), m_c_Specification.InnerCircleRadius, WHITE);

		// Hour hand
		DrawRectanglePro(Rectangle(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2, 8, m_c_Specification.HourHandLength), Vector2(0, 0), m_c_Specification.HourHandPosition, BLACK);

		// Minute Hand
		DrawRectanglePro(Rectangle(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2, 4, m_c_Specification.MinuteHandLength), Vector2(0, 0), m_c_Specification.MinuteHandPosition, BLACK);

		// second hand
		DrawRectanglePro(Rectangle(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2, 2, m_c_Specification.SecondHandLength), Vector2(0, 30), m_c_Specification.SecondHandPosition, RED);

		DrawCircleV(Vector2(m_a_Specification.ScreenWidth / 2, m_a_Specification.ScreenHeight / 2), 15, GRAY);
		EndDrawing();
	}

	void Application::Update()
	{
		std::time_t t = std::time(nullptr);
		std::tm* now = std::localtime(&t);
		m_c_Specification.SecondHandPosition = now->tm_sec * 6 + 180;
		m_c_Specification.MinuteHandPosition = now->tm_min * 6 + 180;
		m_c_Specification.HourHandPosition = now->tm_hour * 30 + (now->tm_min / 60.0f) * 30 + 180;

		if (m_sound.has_value()) UpdateMusicStream(*m_sound);
	}

	void Application::SetIcon()
	{
		Image icon = LoadImage("./res/clock.png");
		if (icon.data != nullptr)
		{
			SetWindowIcon(icon);
			UnloadImage(icon);
		}
	}

	void Application::Loop()
	{
		if (s_Instance) s_Instance->Draw();
	}
}
