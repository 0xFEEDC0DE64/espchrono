#pragma once

#ifdef ESP32
#include "sdkconfig.h"
#endif

// system includes
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <ratio>
#include <string>
#include <string_view>

// 3rdparty lib includes
#include <date/date.h>
#include <tl/expected.hpp>

// local includes
#include "cpptypesafeenum.h"

namespace espchrono {

using milliseconds32 = std::chrono::duration<int32_t, std::milli>;
using seconds32 = std::chrono::duration<int32_t>;
using minutes32 = std::chrono::duration<int32_t, std::ratio<60>>;
using hours32 = std::chrono::duration<int32_t, std::ratio<3600>>;

struct utc_clock
{
  typedef std::chrono::milliseconds			duration;
  typedef duration::rep    					rep;
  typedef duration::period 					period;
  typedef std::chrono::time_point<utc_clock, duration> time_point;

  //static_assert(utc_clock::duration::min() < utc_clock::duration::zero(),
  //      "a clock's minimum duration cannot be less than its epoch");

  static constexpr bool is_steady = false;

  static time_point now() noexcept;
};

#define DayLightSavingModeValues(x) \
    x(None) \
    x(EuropeanSummerTime) \
    x(UsDaylightTime)
DECLARE_TYPESAFE_ENUM(DayLightSavingMode, : uint8_t, DayLightSavingModeValues)

struct time_zone
{
    minutes32 offset{};
    DayLightSavingMode dayLightSavingMode{};

    bool operator== (const time_zone &other) const
    {
        return offset == other.offset &&
               dayLightSavingMode == other.dayLightSavingMode;
    }

    bool operator!= (const time_zone &other) const
    {
        return offset != other.offset ||
               dayLightSavingMode != other.dayLightSavingMode;
    }
};

#if !defined(ESP32) || defined(CONFIG_ESPCHRONO_SUPPORT_DEFAULT_TIMEZONE)
extern time_zone get_default_timezone() noexcept;
#endif

template<typename _Clock, typename _Dur>
struct local_time_point : std::chrono::time_point<_Clock, _Dur>
{
private:
    using base_class = std::chrono::time_point<_Clock, _Dur>;

public:
    using base_class::time_point;

    constexpr local_time_point(const local_time_point &other) = default;

    constexpr explicit local_time_point(_Dur duration, const time_zone &_timezone, bool _dst) :
        base_class{duration}, timezone{_timezone}, dst{_dst}
    {}

    time_zone timezone{};
    bool dst{};

    bool operator== (const base_class &other) const = delete;

    bool operator== (const local_time_point &other) const
    {
        return static_cast<const base_class &>(*this) == static_cast<const base_class &>(other) &&
               timezone == other.timezone &&
               dst == other.dst;
    }

    bool operator!= (const base_class &other) const = delete;

    bool operator!= (const local_time_point &other) const
    {
        return static_cast<const base_class &>(*this) != static_cast<const base_class &>(other) ||
               timezone != other.timezone ||
               dst != other.dst;
    }
};

struct local_clock
{
  typedef std::chrono::milliseconds			duration;
  typedef duration::rep    					rep;
  typedef duration::period 					period;
  typedef local_time_point<local_clock, duration> time_point;

  //static_assert(local_clock::duration::min() < local_clock::duration::zero(),
  //      "a clock's minimum duration cannot be less than its epoch");

  static constexpr bool is_steady = false;

#if !defined(ESP32) || defined(CONFIG_ESPCHRONO_SUPPORT_DEFAULT_TIMEZONE)
  static time_point now() noexcept;
#endif
};

struct millis_clock
{
  typedef std::chrono::milliseconds 	duration;
  typedef duration::rep	  				rep;
  typedef duration::period	  			period;
  typedef std::chrono::time_point<millis_clock, duration> 	time_point;

  static constexpr bool is_steady = true;

  static time_point now() noexcept;
};

struct micros_clock
{
    typedef std::chrono::microseconds 	duration;
    typedef duration::rep	  				rep;
    typedef duration::period	  			period;
    typedef std::chrono::time_point<micros_clock, duration> 	time_point;

    static constexpr bool is_steady = true;

    static time_point now() noexcept;
};

struct DateTime
{
    date::year_month_day date;

    uint8_t hour{};
    uint8_t minute{};
    uint8_t second{};
    uint16_t millisecond{};

    enum DayOfWeek { Sunday = 1, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };
    DayOfWeek dayOfWeek{};

    bool operator==(const DateTime &other) const
    {
        return date == other.date &&
               hour == other.hour &&
               minute == other.minute &&
               second == other.second &&
               millisecond == other.millisecond &&
               dayOfWeek == other.dayOfWeek;
    }
};

struct LocalDateTime : public DateTime
{
    time_zone timezone{};
    bool dst{};

    bool operator==(const LocalDateTime &other) const
    {
        return DateTime::operator==(other) &&
               timezone == other.timezone &&
               dst == other.dst;
    }
};

local_clock::time_point utcToLocal(utc_clock::time_point timeStamp, time_zone timezone);
utc_clock::time_point localToUtc(local_clock::time_point local);

#if !defined(ESP32) || defined(CONFIG_ESPCHRONO_SUPPORT_DEFAULT_TIMEZONE)
local_clock::time_point utcToLocal(utc_clock::time_point ts);
#endif

DateTime toDateTime(utc_clock::time_point ts);
LocalDateTime toDateTime(local_clock::time_point ts);

tl::expected<DateTime, std::string> parseDateTime(std::string_view str);

tl::expected<std::chrono::seconds, std::string> parseDaypoint(std::string_view str);

std::string toString(const DateTime &dateTime);

std::string toString(const LocalDateTime &dateTime);

std::string toDaypointString(std::chrono::seconds seconds);

std::chrono::milliseconds ago(millis_clock::time_point a);

std::string toString(milliseconds32 val);
std::string toString(seconds32 val);
std::string toString(minutes32 val);
std::string toString(hours32 val);
} // namespace espchrono
