#include <QtTest>
#include <QDebug>
#include <QByteArray>

#include <chrono>
#include <string>

#include "espchronotestutils.h"
#include "espchrono.h"

using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace date;
using QTest::toString;

class TstEspChrono : public QObject
{
    Q_OBJECT

private slots:
    void test_dateTimeUtcToString_data()
    {
        QTest::addColumn<espchrono::DateTime>("dateTime");
        QTest::addColumn<std::string>("expected");

        QTest::addRow("monday") << espchrono::DateTime{
                                    .date = 10_d/October/2020,
                                    .hour=10,
                                    .minute=20,
                                    .second=30,
                                    .dayOfWeek=espchrono::DateTime::DayOfWeek::Monday
                                   } << "2020-10-10T10:20:30.000"s;

        QTest::addRow("tuesday") << espchrono::DateTime{
                                    .date = 10_d/October/2020,
                                    .hour=10,
                                    .minute=20,
                                    .second=30,
                                    .dayOfWeek=espchrono::DateTime::DayOfWeek::Tuesday
                                   } << "2020-10-10T10:20:30.000"s;

        QTest::addRow("wednesday") << espchrono::DateTime{
                                    .date = 10_d/October/2020,
                                    .hour=10,
                                    .minute=20,
                                    .second=30,
                                    .dayOfWeek=espchrono::DateTime::DayOfWeek::Wednesday
                                   } << "2020-10-10T10:20:30.000"s;

        QTest::addRow("thursday") << espchrono::DateTime{
                                    .date = 10_d/October/2020,
                                    .hour=10,
                                    .minute=20,
                                    .second=30,
                                    .dayOfWeek=espchrono::DateTime::DayOfWeek::Thursday
                                   } << "2020-10-10T10:20:30.000"s;

        QTest::addRow("friday") << espchrono::DateTime{
                                    .date = 10_d/October/2020,
                                    .hour=10,
                                    .minute=20,
                                    .second=30,
                                    .dayOfWeek=espchrono::DateTime::DayOfWeek::Friday
                                   } << "2020-10-10T10:20:30.000"s;

        QTest::addRow("saturday") << espchrono::DateTime{
                                     .date = 10_d/October/2020,
                                     .hour=10,
                                     .minute=20,
                                     .second=30,
                                     .dayOfWeek=espchrono::DateTime::DayOfWeek::Saturday
                                    } << "2020-10-10T10:20:30.000"s;

        QTest::addRow("sunday") << espchrono::DateTime{
                                     .date = 10_d/October/2020,
                                     .hour=10,
                                     .minute=20,
                                     .second=30,
                                     .dayOfWeek=espchrono::DateTime::DayOfWeek::Sunday
                                    } << "2020-10-10T10:20:30.000"s;

        QTest::addRow("leading_zeros") << espchrono::DateTime{
                                           .date = 1_d/January/2020,
                                           .hour=1,
                                           .minute=2,
                                           .second=3,
                                           .dayOfWeek=espchrono::DateTime::DayOfWeek::Monday
                                          } << "2020-01-01T01:02:03.000"s;
    }

    void test_dateTimeUtcToString()
    {
        QFETCH(espchrono::DateTime, dateTime);
        QFETCH(std::string, expected);

        FIXEDCOMPARE(espchrono::toString(dateTime), expected);
    }

    void test_dateTimeLocalToString_data()
    {
        QTest::addColumn<espchrono::LocalDateTime>("dateTime");
        QTest::addColumn<std::string>("expected");

        constexpr const auto makeLocalDateTime = [](auto date, auto hour, auto minute, auto second, auto dayOfWeek, auto timezone, auto dst){
            espchrono::LocalDateTime localDateTime;
            localDateTime.date = date;
            localDateTime.hour = hour;
            localDateTime.minute = minute;
            localDateTime.second = second;
            localDateTime.dayOfWeek = dayOfWeek;
            localDateTime.timezone = timezone;
            localDateTime.dst = dst;
            return localDateTime;
        };

        QTest::addRow("leading_zeros") << makeLocalDateTime(1_d/January/2020, 1, 2, 3, espchrono::DateTime::DayOfWeek::Monday, testTimeZone, false) << "2020-01-01T01:02:03.000 +01:00"s;

        QTest::addRow("leading_zeros_dst") << makeLocalDateTime(1_d/January/2020, 1, 2, 3, espchrono::DateTime::DayOfWeek::Monday, testTimeZone, true) << "2020-01-01T01:02:03.000 +02:00"s;
    }

    void test_dateTimeLocalToString()
    {
        QFETCH(espchrono::LocalDateTime, dateTime);
        QFETCH(std::string, expected);

        FIXEDCOMPARE(espchrono::toString(dateTime), expected);
    }

    void test_toDateTimeUtc_data()
    {
        QTest::addColumn<espchrono::utc_clock::time_point>("time_point");
        QTest::addColumn<espchrono::DateTime>("expected");

        QTest::addRow("random") << espchrono::utc_clock::time_point{123456s} << espchrono::DateTime{
                                    .date = 2_d/January/1970,
                                    .hour=10, .minute=17, .second=36,
                                    .dayOfWeek=espchrono::DateTime::DayOfWeek::Friday
                                   };

        QTest::addRow("leap_year") << espchrono::utc_clock::time_point{1582934400s}
                                   << espchrono::DateTime{
                                       .date = 29_d/February/2020,
                                       .hour=0, .minute=0, .second=0,
                                       .dayOfWeek=espchrono::DateTime::DayOfWeek::Saturday
                                      };

        QTest::addRow("normal_year") << espchrono::utc_clock::time_point{1614556800s}
                                     << espchrono::DateTime{
                                         .date = 1_d/March/2021,
                                         .hour=0, .minute=0, .second=0,
                                         .dayOfWeek=espchrono::DateTime::DayOfWeek::Monday
                                        };
    }

    void test_toDateTimeUtc()
    {
        QFETCH(espchrono::utc_clock::time_point, time_point);
        QFETCH(espchrono::DateTime, expected);

        FIXEDCOMPARE(espchrono::toDateTime(time_point), expected);
    }

    void test_toDateTimeLocal_data()
    {
        QTest::addColumn<espchrono::local_clock::time_point>("time_point");
        QTest::addColumn<espchrono::LocalDateTime>("expected");

        constexpr const auto makeLocalDateTime = [](auto date, auto hour, auto minute, auto second, auto dayOfWeek, auto timezone, auto dst){
            espchrono::LocalDateTime localDateTime;
            localDateTime.date = date;
            localDateTime.hour = hour;
            localDateTime.minute = minute;
            localDateTime.second = second;
            localDateTime.dayOfWeek = dayOfWeek;
            localDateTime.timezone = timezone;
            localDateTime.dst = dst;
            return localDateTime;
        };

        QTest::addRow("no_dst") << espchrono::local_clock::time_point(123456s, testTimeZone, false)
                                << makeLocalDateTime(2_d/January/1970, 10, 17, 36, espchrono::DateTime::DayOfWeek::Friday, testTimeZone, false);
        QTest::addRow("with_dst") << espchrono::local_clock::time_point(123456s, testTimeZone, true)
                                  << makeLocalDateTime(2_d/January/1970, 10, 17, 36, espchrono::DateTime::DayOfWeek::Friday, testTimeZone, true);
    }

    void test_toDateTimeLocal()
    {
        QFETCH(espchrono::local_clock::time_point, time_point);
        QFETCH(espchrono::LocalDateTime, expected);

        FIXEDCOMPARE(espchrono::toDateTime(time_point), expected);
    }

    void test_toDaypointString_data()
    {
        QTest::addColumn<espchrono::seconds32>("input");
        QTest::addColumn<std::string>("expected");

        QTest::addRow("00:00:00") << espchrono::seconds32{}                << "00:00:00"s;
        QTest::addRow("05:00:00") << espchrono::seconds32{5h}              << "05:00:00"s;
        QTest::addRow("05:04:00") << espchrono::seconds32{5h+4min}         << "05:04:00"s;
        QTest::addRow("05:04:03") << espchrono::seconds32{5h+4min+3s}      << "05:04:03"s;
        QTest::addRow("05:00:03") << espchrono::seconds32{5h+3s}           << "05:00:03"s;
        QTest::addRow("23:59:59") << espchrono::seconds32{23h+59min+59s}   << "23:59:59"s;
        QTest::addRow("-23:59:59") << espchrono::seconds32{-23h-59min-59s} << "-23:59:59"s;
        QTest::addRow("-00:59:59") << espchrono::seconds32{-59min-59s}     << "-00:59:59"s;
    }

    void test_toDaypointString()
    {
        QFETCH(espchrono::seconds32, input);
        QFETCH(std::string, expected);
        FIXEDCOMPARE(espchrono::toDaypointString(input), expected);
    }

    void test_parseDaypoint_data()
    {
        using result_t = std::expected<espchrono::seconds32, std::string>;

        QTest::addColumn<std::string>("input");
        QTest::addColumn<result_t>("expected");

        QTest::addRow("bullshit") << "bullshit"s << result_t{std::unexpected("invalid daypoint (bullshit)")};
        QTest::addRow("missing_minute") << "00:"s << result_t{std::unexpected("invalid daypoint (00:)")};
        QTest::addRow("zero") << "00:00"s << result_t{0s};
        QTest::addRow("zero3") << "00:00:00"s << result_t{0s};
        QTest::addRow("random") << "12:34:56"s << result_t{12h+34min+56s};
        QTest::addRow("random2") << "12:34"s << result_t{12h+34min};
//        QTest::addRow("negative") << "-12:34:56"s << result_t{-12h-34min-56s};
//        QTest::addRow("negative_leading_zero") << "-00:34:56"s << result_t{-34min-56s};
    }

    void test_parseDaypoint()
    {
        using result_t = std::expected<espchrono::seconds32, std::string>;

        QFETCH(std::string, input);
        QFETCH(result_t, expected);

        FIXEDCOMPARE(espchrono::parseDaypoint(input), expected);
    }

    void test_compareTimezones()
    {
        espchrono::time_zone a{
            .offset = 10min,
            .dayLightSavingMode = espchrono::DayLightSavingMode::None
        };
        espchrono::time_zone b{
            .offset = 10min,
            .dayLightSavingMode = espchrono::DayLightSavingMode::None
        };

        FIXEDCOMPARE(a == b, true);
        FIXEDCOMPARE(a != b, false);

        a.offset -= 1min;

        FIXEDCOMPARE(a == b, false);
        FIXEDCOMPARE(a != b, true);

        a.dayLightSavingMode = espchrono::DayLightSavingMode::EuropeanSummerTime;

        FIXEDCOMPARE(a == b, false);
        FIXEDCOMPARE(a != b, true);

        a.offset += 1min;

        FIXEDCOMPARE(a == b, false);
        FIXEDCOMPARE(a != b, true);

        a.dayLightSavingMode = espchrono::DayLightSavingMode::None;

        FIXEDCOMPARE(a == b, true);
        FIXEDCOMPARE(a != b, false);
    }

    void test_compareLocalTimepoints()
    {
        espchrono::local_clock::time_point a {
            10s,
            espchrono::time_zone{
                .offset = 1h,
                .dayLightSavingMode = espchrono::DayLightSavingMode::EuropeanSummerTime
            },
            false
        };
        espchrono::local_clock::time_point b {
            10s,
            espchrono::time_zone{
                .offset = 1h,
                .dayLightSavingMode = espchrono::DayLightSavingMode::EuropeanSummerTime
            },
            false
        };

        FIXEDCOMPARE(a == b, true);
        FIXEDCOMPARE(a != b, false);

        a.timezone.offset -= 1h;

        FIXEDCOMPARE(a == b, false);
        FIXEDCOMPARE(a != b, true);

        a.dst = true;

        FIXEDCOMPARE(a == b, false);
        FIXEDCOMPARE(a != b, true);

        a.timezone.offset += 1h;

        FIXEDCOMPARE(a == b, false);
        FIXEDCOMPARE(a != b, true);

        a.dst = false;

        FIXEDCOMPARE(a == b, true);
        FIXEDCOMPARE(a != b, false);

        a -= 1h;

        FIXEDCOMPARE(a == b, false);
        FIXEDCOMPARE(a != b, true);
    }

    void test_utcToLocal_data()
    {
//        espchrono::utc_clock::time_point test{sys_days{2020_y/January/1}.time_since_epoch()};
//        for (int i = 0; i < 365; i++)
//        {
//            const auto result = espchrono::utcToLocal(test, testTimeZone);
//            qDebug() << espchrono::toString(espchrono::toDateTime(test)) << espchrono::toString(espchrono::toDateTime(result));
//            test += 24h;
//        }

        QTest::addColumn<espchrono::utc_clock::time_point>("utc");
        QTest::addColumn<espchrono::local_clock::time_point>("expected");

        QTest::addRow("test1") << espchrono::utc_clock::time_point{sys_days{2020_y/January/1}.time_since_epoch()}
                               << espchrono::local_clock::time_point{sys_days{2020_y/January/1}.time_since_epoch() + 1h, testTimeZone, false};
        QTest::addRow("test2") << espchrono::utc_clock::time_point{sys_days{2020_y/March/29}.time_since_epoch()}
                               << espchrono::local_clock::time_point{sys_days{2020_y/March/29}.time_since_epoch() + 1h, testTimeZone, false};
        QTest::addRow("test3") << espchrono::utc_clock::time_point{sys_days{2020_y/March/30}.time_since_epoch()}
                               << espchrono::local_clock::time_point{sys_days{2020_y/March/30}.time_since_epoch() + 2h, testTimeZone, true};
        QTest::addRow("test4") << espchrono::utc_clock::time_point{sys_days{2020_y/October/25}.time_since_epoch()}
                               << espchrono::local_clock::time_point{sys_days{2020_y/October/25}.time_since_epoch() + 2h, testTimeZone, true};
        QTest::addRow("test5") << espchrono::utc_clock::time_point{sys_days{2020_y/October/26}.time_since_epoch()}
                               << espchrono::local_clock::time_point{sys_days{2020_y/October/26}.time_since_epoch() + 1h, testTimeZone, false};
        QTest::addRow("test6") << espchrono::utc_clock::time_point{sys_days{2020_y/December/31}.time_since_epoch()}
                               << espchrono::local_clock::time_point{sys_days{2020_y/December/31}.time_since_epoch() + 1h, testTimeZone, false};

        QTest::addRow("test7") << espchrono::utc_clock::time_point{sys_days{2020_y/November/11}.time_since_epoch() + 17h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2020_y/November/11}.time_since_epoch() + 18h + 30min, testTimeZone, false};



        QTest::addRow("test8") << espchrono::utc_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 0h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 2h + 30min, testTimeZone, true};
        QTest::addRow("test9") << espchrono::utc_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 1h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 2h + 30min, testTimeZone, false};
        QTest::addRow("test10") << espchrono::utc_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 2h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 3h + 30min, testTimeZone, false};
        QTest::addRow("test11") << espchrono::utc_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 3h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2022_y/October/30}.time_since_epoch() + 4h + 30min, testTimeZone, false};

        QTest::addRow("test12") << espchrono::utc_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 0h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 1h + 30min, testTimeZone, false};
        QTest::addRow("test13") << espchrono::utc_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 1h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 3h + 30min, testTimeZone, true};
        QTest::addRow("test14") << espchrono::utc_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 2h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 4h + 30min, testTimeZone, true};
        QTest::addRow("test15") << espchrono::utc_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 3h + 30min}
                               << espchrono::local_clock::time_point{sys_days{2023_y/March/Sunday[last]}.time_since_epoch() + 5h + 30min, testTimeZone, true};
    }

    void test_utcToLocal()
    {
        QFETCH(espchrono::utc_clock::time_point, utc);
        QFETCH(espchrono::local_clock::time_point, expected);

        FIXEDCOMPARE(utcToLocal(utc, testTimeZone), expected);
    }
};

QTEST_APPLESS_MAIN(TstEspChrono)

#include "tst_espchrono.moc"
