#pragma once

#include <QMetaType>
#include <QtTest>

#include <chrono>
#include <string>

#include <date/date.h>

#include "espchrono.h"
#include "cpputilstestutils.h"

Q_DECLARE_METATYPE(espchrono::milliseconds)
Q_DECLARE_METATYPE(espchrono::seconds)
Q_DECLARE_METATYPE(espchrono::minutes)
Q_DECLARE_METATYPE(espchrono::hours)
Q_DECLARE_METATYPE(espchrono::utc_clock::time_point)
Q_DECLARE_METATYPE(espchrono::local_clock::time_point)
Q_DECLARE_METATYPE(espchrono::DateTime)
Q_DECLARE_METATYPE(espchrono::LocalDateTime)
Q_DECLARE_METATYPE(std::optional<espchrono::seconds>)

extern const espchrono::time_zone testTimeZone;

extern espchrono::millis_clock::time_point wallClock;

namespace {
template<typename T>
espchrono::local_clock::time_point makeLocal(T day, espchrono::minutes time)
{
    espchrono::local_clock::time_point localTime{date::sys_days{day}.time_since_epoch(), testTimeZone, false};
    localTime += time;
    return localTime;
}

template<typename T>
espchrono::utc_clock::time_point makeUtcFromLocal(T day, espchrono::minutes time)
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
char *toString(const espchrono::seconds &val);

template<>
char *toString(const std::optional<espchrono::seconds> &val);
} // namespace QTest
