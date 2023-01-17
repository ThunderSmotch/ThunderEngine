#pragma once

#include <iostream>

namespace ThunderEngine
{
	// ANSI Escape Codes https://en.wikipedia.org/wiki/ANSI_escape_code
	constexpr const char* RESET = "\033[0m";
	// Foreground Colors
	constexpr const char* CYAN = "\033[96m";
	constexpr const char* RED = "\033[31m";
	constexpr const char* YELLOW = "\033[93m";
	// Effects
	constexpr const char* BOLD = "\033[1m";
	constexpr const char* ITALIC = "\033[3m";
	constexpr const char* UNDERLINE = "\033[4m";

	enum class LogMessageType
	{
		INFO = 0,
		WARNING,
		ERROR
	};

	class Logger
	{
	public:

		static void LogPreMessage(LogMessageType message_type);

		static void Log(const char* text);

		template<typename... TArgs>
		static void Error(const char* text, TArgs... FArgs)
		{
			LogTimestamp();
			std::cout << RED << "[ERROR] ";
			Log(text, FArgs...);
			std::cout << RESET;
		}

		template<typename T, typename... TArgs>
		static void Log(const char* text, T FArg, TArgs... FArgs)
		{
			for (; *text != '\0'; text++)
			{
				if (*text == '%')
				{
					std::cout << FArg;
					Log(++text, FArgs...);
					return;
				}
				std::cout << *text;
			}
			std::cout << RESET << std::endl;
		}
	private:

		// Prints a formatted timestamp [HH:MM:SS.ms]
		static void LogTimestamp();

		// Configures the terminal to have the according message type
		static void SetMessageStyle(LogMessageType message_type);
	};
}

// Log macros
#define TE_INFO(...) ThunderEngine::Logger::LogPreMessage(ThunderEngine::LogMessageType::INFO); ThunderEngine::Logger::Log(__VA_ARGS__)
#define TE_WARN(...) ThunderEngine::Logger::LogPreMessage(ThunderEngine::LogMessageType::WARNING); ThunderEngine::Logger::Log(__VA_ARGS__)
#define TE_ERROR(...) ThunderEngine::Logger::LogPreMessage(ThunderEngine::LogMessageType::ERROR); ThunderEngine::Logger::Log(__VA_ARGS__)
