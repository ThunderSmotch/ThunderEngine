#pragma once

#include <iostream>

namespace ThunderEngine
{
	enum class LogMessageType
	{
		INFO,
		WARNING,
		ERROR
	};

	/// <summary>
	/// Logger static class to print formatted and timestamped messages.
	/// It is recommended to use the below macros for logging purposes.
	/// </summary>
	class Logger
	{
	public:

		/// <summary>
		/// Prints the timestamp and sets up the style for the following message
		/// </summary>
		/// <param name="message_type">The type of message to log</param>
		static void LogPreMessage(LogMessageType message_type);


		/// <summary>
		/// Prints the given text and resets the style
		/// </summary>
		/// <param name="text">Text to be printed</param>
		static void Log(const char* text)
		{
			std::cout << text << RESET << std::endl;
		}

		/// <summary>
		/// Prints the given message, printing T inplace of the first unescaped %.
		/// </summary>
		/// <typeparam name="T">First optional argument</typeparam>
		/// <typeparam name="...TArgs">Other following arguments to replace following %'s</typeparam>
		/// <param name="text">The string to print. 
		/// Unescaped % will use additional arguments provided.
		/// </param>
		/// <param name="FArg">First optional argument that will replace the first %</param>
		/// <param name="...FArgs">Other following arguments to replace following %'s</param>
		/// <remarks>To escape the % insert another one, like so "%%".</remarks>
		template<typename T, typename... TArgs>
		static void Log(const char* text, T FArg, TArgs... FArgs)
		{
			for (; *text; ++text)
			{
				if (*text == '%' && *(text+1) != '%')
				{
					std::cout << FArg;
					Log(text + 1, FArgs...);
					return;
				}
				std::cout << *text;
			}
			std::cout << RESET << std::endl;
		}

	private:

		// Prints a formatted timestamp [HH:MM:SS.ms]
		static void LogTimestamp();

		// Configures the terminal to have the style associated to this message type
		static void LogStyleAndTypeName(LogMessageType message_type);

		// ANSI Escape Codes https://en.wikipedia.org/wiki/ANSI_escape_code
		static constexpr const char* RESET = "\033[0m";
		// Foreground Colors
		static constexpr const char* CYAN = "\033[96m";
		static constexpr const char* RED = "\033[31m";
		static constexpr const char* YELLOW = "\033[93m";
		// Effects
		static constexpr const char* BOLD = "\033[1m";
		static constexpr const char* ITALIC = "\033[3m";
		static constexpr const char* UNDERLINE = "\033[4m";
	};
}

// Log macros

// Log an Information
#define TE_INFO(...) ThunderEngine::Logger::LogPreMessage(ThunderEngine::LogMessageType::INFO); ThunderEngine::Logger::Log(__VA_ARGS__)
// Log a Warning
#define TE_WARN(...) ThunderEngine::Logger::LogPreMessage(ThunderEngine::LogMessageType::WARNING); ThunderEngine::Logger::Log(__VA_ARGS__)
// Log an Error
#define TE_ERROR(...) ThunderEngine::Logger::LogPreMessage(ThunderEngine::LogMessageType::ERROR); ThunderEngine::Logger::Log(__VA_ARGS__)
