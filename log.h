#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Log 
{
    public:
        using log_t = std::shared_ptr<spdlog::logger>;
        static void init();
        static log_t& instance(){return s_client;}
    private:
        static log_t s_client;

};

#define SPDLOG_STR_H(x) #x
#define SPDLOG_STR_HELPER(x) SPDLOG_STR_H(x)

#define blog(...)  Log::instance()->trace("[" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#define binfo(...)  Log::instance()->info("[" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#define bwarn(...)  Log::instance()->warn("[" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#define berror(...)  Log::instance()->error("[" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#define bcritical(...)  Log::instance()->critical("[" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
