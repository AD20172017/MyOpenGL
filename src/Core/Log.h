#pragma once
#include "Core/Base.h"
#include "spdlog/spdlog.h"
CPP_CORE_BEGIN

class Log
{
public:
	static void init();

	static Ptr<spdlog::logger> GetCoreLogger() { return m_CoreLogger; }
	static Ptr<spdlog::logger> GetClientLogger() { return m_ClientLogger; }


private:
	static Ptr<spdlog::logger> m_CoreLogger;
	static Ptr<spdlog::logger> m_ClientLogger;
};



// Core log macros
#define CORE_TRACE(...)    ::Core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::Core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::Core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::Core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::Core::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CLI_TRACE(...)         ::Core::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CLI_INFO(...)          ::Core::Log::GetClientLogger()->info(__VA_ARGS__)
#define CLI_WARN(...)          ::Core::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CLI_ERROR(...)         ::Core::Log::GetClientLogger()->error(__VA_ARGS__)
#define CLI_CRITICAL(...)      ::Core::Log::GetClientLogger()->critical(__VA_ARGS__)

CPP_CORE_END
