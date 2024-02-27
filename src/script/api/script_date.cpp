/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file script_date.cpp Implementation of ScriptDate. */

#include "../../stdafx.h"
#include "script_date.hpp"
#include "script_timemode.hpp"
#include "../../date_func.h"
#include "../../settings_type.h"

#include <time.h>

#include <time.h>

#include "../../safeguards.h"

/* static */ bool ScriptDate::IsValidDate(Date date)
{
	return date >= 0;
}

/* static */ ScriptDate::Date ScriptDate::GetCurrentDate()
{
	if (ScriptTimeMode::IsCalendarMode()) return (ScriptDate::Date)::CalTime::CurDate().base();

	return (ScriptDate::Date)EconTime::CurDate().base();
}

/* static */ SQInteger ScriptDate::GetDayLengthFactor()
{
	return DayLengthFactor();
}

/* static */ SQInteger ScriptDate::GetYear(ScriptDate::Date date)
{
	if (date < 0) return DATE_INVALID;

	if (ScriptTimeMode::IsCalendarMode()) {
		::CalTime::YearMonthDay ymd = ::CalTime::ConvertDateToYMD(date);
		return ymd.year.base();
	}

	::EconTime::YearMonthDay ymd = ::EconTime::ConvertDateToYMD(date);
	return ymd.year.base();
}

/* static */ SQInteger ScriptDate::GetMonth(ScriptDate::Date date)
{
	if (date < 0) return DATE_INVALID;

	if (ScriptTimeMode::IsCalendarMode()) {
		::CalTime::YearMonthDay ymd = ::CalTime::ConvertDateToYMD(date);
		return ymd.month + 1;
	}

	::EconTime::YearMonthDay ymd = ::EconTime::ConvertDateToYMD(date);
	return ymd.month + 1;
}

/* static */ SQInteger ScriptDate::GetDayOfMonth(ScriptDate::Date date)
{
	if (date < 0) return DATE_INVALID;

	if (ScriptTimeMode::IsCalendarMode()) {
		::CalTime::YearMonthDay ymd = ::CalTime::ConvertDateToYMD(date);
		return ymd.day;
	}

	::EconTime::YearMonthDay ymd = ::EconTime::ConvertDateToYMD(date);
	return ymd.day;
}

/* static */ ScriptDate::Date ScriptDate::GetDate(SQInteger year, SQInteger month, SQInteger day_of_month)
{
	if (month < 1 || month > 12) return DATE_INVALID;
	if (day_of_month < 1 || day_of_month > 31) return DATE_INVALID;
	if (year < 0 || year > CalTime::MAX_YEAR) return DATE_INVALID;

	if (ScriptTimeMode::IsCalendarMode()) return (ScriptDate::Date)::CalTime::ConvertYMDToDate(year, month - 1, day_of_month).base();

	return (ScriptDate::Date)::EconTime::ConvertYMDToDate(year, month - 1, day_of_month).base();
}

/* static */ SQInteger ScriptDate::GetSystemTime()
{
	time_t t;
	time(&t);
	return t;
}

/* static */ bool ScriptDate::IsTimeShownInMinutes()
{
	return _settings_game.game_time.time_in_minutes;
}

/* static */ SQInteger ScriptDate::GetTicksPerMinute()
{
	return _settings_game.game_time.ticks_per_minute;
}

/* static */ SQInteger ScriptDate::GetCurrentScaledDateTicks()
{
	return _state_ticks.base();
}

/* static */ SQInteger ScriptDate::GetHour(SQInteger ticks)
{
	TickMinutes minutes = _settings_game.game_time.ToTickMinutes(StateTicks(ticks));
	return minutes.ClockHour();
}

/* static */ SQInteger ScriptDate::GetMinute(SQInteger ticks)
{
	TickMinutes minutes = _settings_game.game_time.ToTickMinutes(StateTicks(ticks));
	return minutes.ClockMinute();
}
