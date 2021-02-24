// local includes
#include "espchrono.h"

// system includes
#include <time.h>

// esp-idf inlcludes
#include <esp_timer.h>

using namespace std::chrono_literals;

// actual implementations used on the ESP32

namespace espchrono {
auto utc_clock::now() noexcept -> time_point
{
    timeval tv;
    gettimeofday(&tv, NULL);
    seconds32 seconds{tv.tv_sec};
    return time_point{seconds};
}

auto millis_clock::now() noexcept -> time_point
{
    return time_point{std::chrono::floor<duration>(std::chrono::microseconds{esp_timer_get_time()})};
}
} // namespace espchrono
