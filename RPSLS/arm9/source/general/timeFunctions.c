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
 * Gets the current year according to the DS.
 * @return Returns the year as an integer.
 */
int getTimeYear()
{
	return DSTime->tm_year + 1900;
}

/*
 * Gets the current month according to the DS.
 * @return Returns the month as an integer.
 */
int getTimeMonth()
{
	return DSTime->tm_mon + 1;
}

/*
 * Gets the current day of the month according to the DS.
 * @return Returns the day of the month (1-31) as an integer.
 */
int getTimeDayOfMonth()
{
	return DSTime->tm_mday + 1;
}

/*
 * Gets the current day of the week according to the DS.
 * @return Returns the day of the week (1-7) as an integer.
 */
int getTimeDayOfWeek()
{
	return DSTime->tm_wday + 1;
}

/*
 * Gets the current day of the year according to the DS.
 * @return Returns the day of the year (1-365) as an integer.
 */
int getTimeDayOfYear()
{
	return DSTime->tm_yday;
}

/*
 * Gets the current hours according to the DS.
 * @param military Whether to get the hours in military time or not (IE: 24 hour vs 12 hour).
 * @return Returns the current hour as an integer in military time if military is true (1-24) or
 * in 12 hour format (1-12) if military is false.
 */
int getTimeHours(bool military)
{
	// Check if military time is desired.
	if (military)
	{
		// If so, just return the time's hour.
		return DSTime->tm_hour;
	}
	else
	{
		// If not, check if the time is greater than 12.
		if (DSTime->tm_hour > 12)
		{
			// If it is, then check if it is exactly 12 hours.
			if(DSTime->tm_hour - 12 == 0)
			{
				// If so, return 12 (12pm).
				return 12;
			}
			// Otherwise, return the time minus 12 (1pm-11pm).
			return DSTime->tm_hour - 12;
		}
		else
		{
			// Check if the time is 0 (Midnight or 12am).
			if(DSTime->tm_hour == 0)
			{
				// Return 12 if this is the case (12am).
				return 12;
			}
			// Otherwise, return the time (1am-11am).
			return DSTime->tm_hour;
		}
	}
}

/*
 * Gets the current minutes passed the hour according to the DS.
 * @return Returns the minutes as an integer.
 */
int getTimeMinutes()
{
	return DSTime->tm_min;
}

/*
 * Gets the current secconds past according to the DS.
 * @return Returns the seconds as an integer.
 */
int getTimeSeconds()
{
	// Check if the time is less than 60.
	if (DSTime->tm_sec < 60)
	{
		// Return the time's seconds if it is.
		return DSTime->tm_sec;
	}
	// Return 0 otherwise.
	return 0;
}

/*
 * Gets the current time according to the DS as text.
 * @return Returns the time as a character array of ascii text.
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
