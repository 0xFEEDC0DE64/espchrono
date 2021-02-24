#include "espchrono.h"

using namespace std::chrono_literals;
using namespace date;

namespace espchrono {

namespace {
static const uint8_t _monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool summerTime(local_clock::time_point timeStamp)
{
    LocalDateTime _tempDateTime = toDateTime(timeStamp);

    if (_tempDateTime.date.month() < March || _tempDateTime.date.month() > October) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
    if (_tempDateTime.date.month() > March && _tempDateTime.date.month() < October) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
    return
        (_tempDateTime.date.month() == March && (_tempDateTime.hour + 24 * unsigned(_tempDateTime.date.day())) >= (3 +  24 * (31 - (5 * int(_tempDateTime.date.year()) / 4 + 4) % 7))) ||
        (_tempDateTime.date.month() == October && (_tempDateTime.hour + 24 * unsigned(_tempDateTime.date.day())) < (3 +  24 * (31 - (5 * int(_tempDateTime.date.year()) / 4 + 1) % 7)));
}

bool daylightSavingTime(local_clock::time_point _timeStamp)
{
    LocalDateTime  _tempDateTime = toDateTime(_timeStamp);

    // see http://stackoverflow.com/questions/5590429/calculating-daylight-saving-time-from-only-date
    // since 2007 DST begins on second Sunday of March and ends on first Sunday of November.
    // Time change occurs at 2AM locally
    if (_tempDateTime.date.month() < March || _tempDateTime.date.month() > November) return false;  //January, february, and december are out.
    if (_tempDateTime.date.month() > March && _tempDateTime.date.month() < November) return true;   //April to October are in

    int previousSunday = unsigned(_tempDateTime.date.day()) - (_tempDateTime.dayOfWeek - 1);  // dow Sunday input was 1,
    // need it to be Sunday = 0. If 1st of month = Sunday, previousSunday=1-0=1
    //int previousSunday = day - (dow-1);
    // -------------------- March ---------------------------------------
    //In march, we are DST if our previous Sunday was = to or after the 8th.
    if (_tempDateTime.date.month() == March) {  // in march, if previous Sunday is after the 8th, is DST
        // unless Sunday and hour < 2am
        if (previousSunday >= 8) { // Sunday = 1
            // return true if day > 14 or (dow == 1 and hour >= 2)
            return ((_tempDateTime.date.day() > 14_d) ||
            ((_tempDateTime.dayOfWeek == 1 && _tempDateTime.hour >= 2) || _tempDateTime.dayOfWeek > 1));
        } // end if ( previousSunday >= 8 && _dateTime.dayofWeek > 0 )
        else
        {
            // previousSunday has to be < 8 to get here
            //return (previousSunday < 8 && (_tempDateTime.dayofWeek - 1) = 0 && _tempDateTime.hour >= 2)
            return false;
        } // end else
    } // end if (_tempDateTime.date.month == 3 )

    // ------------------------------- November -------------------------------

    // gets here only if month = November
    //In november we must be before the first Sunday to be dst.
    //That means the previous Sunday must be before the 2nd.
    if (previousSunday < 1)
    {
        // is not true for Sunday after 2am or any day after 1st Sunday any time
        return ((_tempDateTime.dayOfWeek == 1 && _tempDateTime.hour < 2) || (_tempDateTime.dayOfWeek > 1));
        //return true;
    } // end if (previousSunday < 1)
    else
    {
        // return false unless after first wk and dow = Sunday and hour < 2
        return (_tempDateTime.date.day() < 8_d && _tempDateTime.dayOfWeek == 1 && _tempDateTime.hour < 2);
    }  // end else
}
} // namespace

auto local_clock::now() noexcept -> time_point
{
    return utcToLocal(utc_clock::now());
}

local_clock::time_point utcToLocal(utc_clock::time_point utc, time_zone timezone)
{
    local_clock::time_point local{utc.time_since_epoch() + timezone.offset, timezone, false};

    switch (timezone.dayLightSavingMode)
    {
    case DayLightSavingMode::None: break;
    case DayLightSavingMode::EuropeanSummerTime: if (summerTime(local))         local.dst = true; break;
    case DayLightSavingMode::UsDaylightTime:     if (daylightSavingTime(local)) local.dst = true; break;
    }

    if (local.dst)
        local += 1h;

    return local;
}

utc_clock::time_point localToUtc(local_clock::time_point local)
{
    utc_clock::time_point utc{local.time_since_epoch()};

    if (local.dst)
        utc -= 1h;

    utc -= local.timezone.offset;

    return utc;
}

local_clock::time_point utcToLocal(utc_clock::time_point ts)
{
    return utcToLocal(ts, local_clock::timezone());
}

namespace {
DateTime toDateTime(seconds32 ts)
{
    auto _time = ts.count();

    uint8_t second(_time % 60);
    _time /= 60; // now it is minutes
    uint8_t minute(_time % 60);
    _time /= 60; // now it is hours
    uint8_t hour(_time % 24);
    _time /= 24; // now it is _days
    auto dayOfWeek = DateTime::DayOfWeek(((_time + 4) % 7) + 1);  // Sunday is day 1

    date::year year{1970};
    unsigned long _days = 0;
    while ((unsigned)(_days += (year.is_leap() ? 366 : 365)) <= _time) {
        year++;
    }

    _days -= year.is_leap() ? 366 : 365;
    _time  -= _days; // now it is days in this year, starting at 0

    _days = 0;
    uint8_t _monthLength = 0;
    date::month month = January;
    for (; month <= December; month++) {
        if (month == February) { // february
            if (year.is_leap()) {
                _monthLength = 29;
            } else {
                _monthLength = 28;
            }
        } else {
            _monthLength = _monthDays[unsigned(month)-1];
        }

        if (_time >= _monthLength) {
            _time -= _monthLength;
        } else {
            break;
        }
    }
    date::day day(_time + 1);     // day of month

    DateTime dateTime{year_month_day{year, month, day}};
    dateTime.hour=hour;
    dateTime.minute=minute;
    dateTime.second=second;
    dateTime.dayOfWeek=dayOfWeek;
    return dateTime;
}
}

DateTime toDateTime(utc_clock::time_point ts)
{
    return toDateTime(ts.time_since_epoch());
}

LocalDateTime toDateTime(local_clock::time_point ts)
{
    LocalDateTime dateTime = LocalDateTime{toDateTime(ts.time_since_epoch())};
    dateTime.dst = ts.dst;
    dateTime.timezone = ts.timezone;
    return dateTime;
}

std::optional<DateTime> parseDateTime(std::string_view str)
{
    // both valid:
    // 2020-11-10T21:31
    // 2020-11-10T21:31:10

    int year;
    unsigned month;
    unsigned day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second{};

    constexpr auto dateTimeFormat = "%4d-%2u-%2uT%2hhu:%2hhu:%2hhu";
    if (const auto scanned = std::sscanf(str.data(), dateTimeFormat, &year, &month, &day, &hour, &minute, &second); scanned < 5)
        return std::nullopt;

    return DateTime{
        .date=date::year_month_day{date::year{year}, date::month{month}, date::day{day}},
        .hour=hour,
        .minute=minute,
        .second=second
    };
}

std::optional<seconds32> parseDaypoint(std::string_view str)
{
    int8_t hour, minute, second{};

    constexpr auto daypointFormat = "%2hhd:%2hhd:%2hhd";
    if (const auto scanned = std::sscanf(str.data(), daypointFormat, &hour, &minute, &second); scanned < 2)
        return std::nullopt;

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
        return std::nullopt;

    return hours32{hour} + minutes32{minute} + seconds32{second};
}

std::string toString(const DateTime &dateTime)
{
    char buf[27];

    std::sprintf(buf, "%04i-%02u-%02uT%02hhu:%02hhu:%02hhu",
                 int{dateTime.date.year()}, unsigned{dateTime.date.month()}, unsigned{dateTime.date.day()},
                 dateTime.hour, dateTime.minute, dateTime.second);

    return std::string{buf};
}

std::string toString(const LocalDateTime &dateTime)
{
    char buf[34];

    date::hh_mm_ss helper{dateTime.timezone.offset + hours32{dateTime.dst ? 1 : 0}};

    std::sprintf(buf, "%04i-%02u-%02uT%02hhu:%02hhu:%02hhu %s%02hhu:%02hhu",
                 int{dateTime.date.year()}, unsigned{dateTime.date.month()}, unsigned{dateTime.date.day()},
                 dateTime.hour, dateTime.minute, dateTime.second,
                 helper.is_negative() ? "-" : "+", uint8_t(helper.hours().count()), uint8_t(helper.minutes().count()));

    return std::string{buf};
}

std::string toDaypointString(seconds32 seconds)
{
    date::hh_mm_ss helper(seconds);
    char buf[10];
    std::sprintf(buf, "%s%02hhd:%02hhd:%02hhd", helper.is_negative() ? "-" : "", int8_t(helper.hours().count()), int8_t(helper.minutes().count()), int8_t(helper.seconds().count()));
    return std::string{buf};
}

std::chrono::milliseconds ago(millis_clock::time_point a)
{
    return millis_clock::now() - a;
}

std::string toString(milliseconds32 val) { return std::to_string(val.count()) + "ms"; }
std::string toString(seconds32 val) { return std::to_string(val.count()) + "s"; }
std::string toString(minutes32 val) { return std::to_string(val.count()) + "min"; }
std::string toString(hours32 val) { return std::to_string(val.count()) + "h"; }

IMPLEMENT_TYPESAFE_ENUM(DayLightSavingMode, : uint8_t, DayLightSavingModeValues)

} // namespace espchrono
