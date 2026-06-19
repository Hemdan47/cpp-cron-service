#include "utils/TimeUtils.h"
#include "exceptions/InvalidScheduleException.h"
#include <stdexcept>


std::chrono::sys_seconds iso_to_sys_seconds(const std::string& iso_date) {
    try {
        if (iso_date.length() < 19 || iso_date[4] != '-' || iso_date[7] != '-' || iso_date[10] != 'T' || iso_date[13] != ':' || iso_date[16] != ':') {
            throw InvalidScheduleException("Invalid ISO date format: '" + iso_date + "'. Expected format: YYYY-MM-DDTHH:MM:SS");
        }
        int _year   = std::stoi(iso_date.substr(0, 4));
        int _month  = std::stoi(iso_date.substr(5, 2));
        int _day    = std::stoi(iso_date.substr(8, 2));
        int _hour   = std::stoi(iso_date.substr(11, 2));
        int _minute = std::stoi(iso_date.substr(14, 2));
        int _second = std::stoi(iso_date.substr(17, 2));

        using namespace std::chrono;

        return sys_days{
            year_month_day{
                year{_year},
                month{static_cast<unsigned>(_month)},
                day{static_cast<unsigned>(_day)}
            }
        }
        + hours{_hour}
        + minutes{_minute}
        + seconds{_second};
    }
    catch (const std::invalid_argument& e) {
        throw InvalidScheduleException("Invalid numeric characters in ISO date: '" + iso_date + "'");
    }
    catch (const std::out_of_range& e) {
        throw InvalidScheduleException("ISO date values out of range: '" + iso_date + "'");
    }
}

std::tuple<unsigned, unsigned, int, int, int> sys_seconds_to_calendar(const std::chrono::sys_seconds tp) {
    using namespace std::chrono;

    const auto days_part = floor<days>(tp);
    const year_month_day ymd{days_part};
    const weekday wd{days_part};
    const hh_mm_ss hms{tp - days_part};

    return {
        static_cast<unsigned>(ymd.month()),
        static_cast<unsigned>(ymd.day()),
        static_cast<int>(hms.hours().count()),
        static_cast<int>(hms.minutes().count()),
        static_cast<int>(wd.c_encoding())
    };
}