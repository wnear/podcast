#include "log.h"

#ifdef USE_SPDLOG
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#endif

#include <vector>


Log::log_t Log::s_client;

void Log::init()
{}
#ifdef USE_SPDLOG
{
    std::vector<spdlog::sink_ptr> logSinks;

    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("app.log", true));
    // l for level.
    // T for time.
    // t for thread.
    // n for
    // v for original message.
    logSinks[0]->set_pattern("%^*** [%T][thread %t]%v%$");
    logSinks[1]->set_pattern("[%T][%l] %n: %v");

    s_client = std::make_shared<spdlog::logger>("podcast", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_client);

    s_client->set_level(spdlog::level::trace);
    s_client->flush_on(spdlog::level::trace);
}
#endif
