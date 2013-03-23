/*
 * A set of functions for dealing with the Nintendo DS/DSi's
 * real time clock (RTC).
 * Created by: Gerald McAlister
 */
#include "timeFunctions.h"

/*
 *  Holds the time of according to the DS.
 */
struct tm* DSTime;
time_t unixTime;

/*
 *  Gets the current year according to the DS.
 */
int getTimeYear()
{
	return DSTime->tm_year + 1900;
}

/*
 *  Gets the current month according to the DS.
 */
int getTimeMonth()
{
	return DSTime->tm_mon + 1;
}

/*
 *  Gets the current day of the month according to the DS.
 */
int getTimeDayOfMonth()
{
	return DSTime->tm_mday + 1;
}

/*
 *  Gets the current day of the week according to the DS.
 */
int getTimeDayOfWeek()
{
	return DSTime->tm_wday + 1;
}

/*
 *  Gets the current day of the year according to the DS.
 */
int getTimeDayOfYear()
{
	return DSTime->tm_yday;
}

/*
 *  Gets the current hours according to the DS.
 */
int getTimeHours(bool military)
{
	if (military)
	{
		return DSTime->tm_hour;
	}
	else
	{
		if (DSTime->tm_hour > 12)
		{
			if(DSTime->tm_hour - 12 == 0)
			{
				return 12;
			}
			return DSTime->tm_hour - 12;
		}
		else
		{
			if(DSTime->tm_hour == 0)
			{
				return 12;
			}
			return DSTime->tm_hour;
		}
	}
}

/*
 *  Gets the current minutes passed the hour according to the DS.
 */
int getTimeMinutes()
{
	return DSTime->tm_min;
}

/*
 *  Gets the current secconds past according to the DS.
 */
int getTimeSeconds()
{
	if (DSTime->tm_sec < 60)
	{
		return DSTime->tm_sec;
	}
	else
		return 0;
}

/*
 *  Gets the current time according to the DS as text.
 */
char* getTimeText()
{
	return asctime(DSTime);
}

/*
 *  Updates the current time according to the DS.
 */
void updateTime()
{
	unixTime = time(NULL);
	DSTime = gmtime((const time_t *) &unixTime);
}

/*
 *  Updates the graphics based on the current time of day.
 */
void updateGameTime()
{
	updateTime();
}

