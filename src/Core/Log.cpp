#include "Core/Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

CPP_CORE_BEGIN
Ptr<spdlog::logger> Log::m_CoreLogger;
Ptr<spdlog::logger> Log::m_ClientLogger;

void Log::init()
{
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log.log", true));

	logSinks[0]->set_pattern("%^[%T] %n: %v%$");
	logSinks[1]->set_pattern("[%T] [%l] %n: %v");

	m_CoreLogger = std::make_shared<spdlog::logger>("Core", begin(logSinks), end(logSinks));
	spdlog::register_logger(m_CoreLogger);
	m_CoreLogger->set_level(spdlog::level::trace);
	m_CoreLogger->flush_on(spdlog::level::trace);

	m_ClientLogger = std::make_shared<spdlog::logger>("Client", begin(logSinks), end(logSinks));
	spdlog::register_logger(m_ClientLogger);
	m_ClientLogger->set_level(spdlog::level::trace);
	m_ClientLogger->flush_on(spdlog::level::trace);


	

}


CPP_CORE_END