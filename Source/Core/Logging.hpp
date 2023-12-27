#pragma once

#include <spdlog/spdlog.h>

#define HC_LOG_INFO(...)        ::spdlog::info(__VA_ARGS__)
#define HC_LOG_WARNING(...)     ::spdlog::warn(__VA_ARGS__)
#define HC_LOG_ERROR(...)       ::spdlog::error(__VA_ARGS__)
#define HC_LOG_FATAL(...)       ::spdlog::critical(__VA_ARGS__)
