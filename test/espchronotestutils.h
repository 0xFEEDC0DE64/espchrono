#pragma once

#include <QMetaType>
#include <QtTest>

#include <chrono>
#include <string>

#include <date/date.h>

#include "espchrono.h"
#include "cpputilstestutils.h"

Q_DECLARE_METATYPE(std::chrono::milliseconds)
Q_DECLARE_METATYPE(std::chrono::seconds)
Q_DECLARE_METATYPE(std::chrono::minutes)
Q_DECLARE_METATYPE(std::chrono::hours)
Q_DECLARE_METATYPE(espchrono::milliseconds32)
Q_DECLARE_METATYPE(espchrono::seconds32)
Q_DECLARE_METATYPE(espchrono::minutes32)
Q_DECLARE_METATYPE(espchrono::hours32)
Q_DECLARE_METATYPE(espchrono::utc_clock::time_point)
Q_DECLARE_METATYPE(espchrono::local_clock::time_point)
Q_DECLARE_METATYPE(espchrono::DateTime)
Q_DECLARE_METATYPE(espchrono::LocalDateTime)
Q_DECLARE_METATYPE(std::optional<espchrono::seconds32>)

extern const espchrono::time_zone testTimeZone;

extern espchrono::millis_clock::time_point mockedMillisClock;
extern espchrono::utc_clock::time_point mockedUtcClock;

namespace {
template<typename T>
espchrono::local_clock::time_point makeLocal(T day, espchrono::minutes32 time)
{
    espchrono::local_clock::time_point localTime{date::sys_days{day}.time_since_epoch(), testTimeZone, false};
    localTime += time;
    return localTime;
}

template<typename T>
espchrono::utc_clock::time_point makeUtcFromLocal(T day, espchrono::minutes32 time)
{
    const auto localTime = makeLocal(day, time);
    return espchrono::localToUtc(localTime);
}
} // namespace

namespace QTest {
template<>
char *toString(const espchrono::DateTime &dateTime);

template<>
char *toString(const espchrono::LocalDateTime &dateTime);

template<>
char *toString(const espchrono::utc_clock::time_point &ts);

template<>
char *toString(const espchrono::local_clock::time_point &ts);

template<>
char *toString(const espchrono::seconds32 &val);

template<>
char *toString(const std::optional<espchrono::seconds32> &val);
} // namespace QTest
