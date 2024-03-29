#pragma once

#ifdef USE_SPDLOG
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#endif
#include <QString>
#include <QDebug>

class Log {
  public:
#ifdef USE_SPDLOG
    using log_t = std::shared_ptr<spdlog::logger>;
#else
    using log_t = QString;
#endif
    static log_t& instance() { return s_client; }
    static void init();

  private:
    static log_t s_client;
};

template <typename OStream>
inline OStream& operator<<(OStream& os, const QString& x) {
    return os << x.toStdString();
}

#ifdef USE_SPDLOG
#define SPDLOG_STR_H(x) #x
#define SPDLOG_STR_HELPER(x) SPDLOG_STR_H(x)
#define blog(...)                       \
    Log::instance()->trace("[" __FILE__ \
                           "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#define binfo(...)                                                          \
    Log::instance()->info("[" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") " \
                                                                       "] " __VA_ARGS__)
#define bwarn(...)                                                          \
    Log::instance()->warn("[" __FILE__ "(" SPDLOG_STR_HELPER(__LINE__) ") " \
                                                                       "] " __VA_ARGS__)
#define berror(...)                     \
    Log::instance()->error("[" __FILE__ \
                           "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#define bcritical(...)                     \
    Log::instance()->critical("[" __FILE__ \
                              "(" SPDLOG_STR_HELPER(__LINE__) ") ] " __VA_ARGS__)
#else
#define blog(...)
#define binfo(...)
#define bwarn(...)
#define berror(...)
#define bcritical(...)
#endif
