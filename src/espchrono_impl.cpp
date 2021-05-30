// local includes
#include "espchrono.h"

// system includes
#include <time.h>

// esp-idf inlcludes
#include <esp_timer.h>

using namespace std::chrono_literals;

// actual implementations used on the ESP32

auto espchrono::utc_clock::now() noexcept -> time_point
{
    timeval tv;
    gettimeofday(&tv, NULL);
    std::chrono::seconds seconds{tv.tv_sec};
    std::chrono::microseconds microseconds{tv.tv_usec};
    return time_point{std::chrono::floor<duration>(seconds+microseconds)};
}

auto espchrono::millis_clock::now() noexcept -> time_point
{
    return time_point{std::chrono::floor<duration>(std::chrono::microseconds{esp_timer_get_time()})};
}
