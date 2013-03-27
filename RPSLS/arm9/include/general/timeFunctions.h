/*
 * A set of functions for dealing with the Nintendo DS/DSi's
 * real time clock (RTC).
 * Created by: Gerald McAlister
 */

#ifndef _TIME_FUNCTIONS_H_
#define _TIME_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
#include <time.h>

/*
 * A structure for storing a specific date.
 * day - The day of the date.
 * month - The month of the date.
 * year - The year of the date.
 */
typedef struct specificDate_t
{
	int day;
	int month;
	int year;
}specificDate_t;

/*
 * This is a structure used for storing
 * events.  Events can be used in games to
 * plan certain things, such as special events, days
 * of importance in game, etc.
 * name - The name of the event.
 * isActive - Whether the event is active currently or not.
 * message - The message for the event.
 * date - The specific date of the event.s
 */
typedef struct event_t
{
	char name[256];
	bool isActive;
	char message[256];
	specificDate_t date;
}event_t;

/*
 *  Holds the time of according to the DS.
 */
extern struct tm* DSTime;

/*
 * Gets the current secconds past according to the DS.
 * @return Returns the seconds as an integer.
 */
extern int getTimeSeconds();

/*
 * Gets the current minutes passed the hour according to the DS.
 * @return Returns the minutes as an integer.
 */
extern int getTimeMinutes();

/*
 * Gets the current hours according to the DS.
 * @param military Whether to get the hours in military time or not (IE: 24 hour vs 12 hour).
 * @return Returns the current hour as an integer in military time if military is true (1-24) or
 * in 12 hour format (1-12) if military is false.
 */
extern int getTimeHours(bool military);

/*
 * Gets the current day of the year according to the DS.
 * @return Returns the day of the year (1-365) as an integer.
 */
extern int getTimeDayOfYear();

/*
 * Gets the current day of the week according to the DS.
 * @return Returns the day of the week (1-7) as an integer.
 */
extern int getTimeDayOfWeek();

/*
 * Gets the current day of the month according to the DS.
 * @return Returns the day of the month (1-31) as an integer.
 */
extern int getTimeDayOfMonth();

/*
 * Gets the current month according to the DS.
 * @return Returns the month as an integer.
 */
extern int getTimeMonth();

/*
 * Gets the current year according to the DS.
 * @return Returns the year as an integer.
 */
extern int getTimeYear();

/*
 * Gets the current time according to the DS as text.
 * @return Returns the time as a character array of ascii text.
 */
extern char* getTimeText();

/*
 *  Updates the current time according to the DS.
 */
extern void updateTime();

#ifdef __cplusplus
}
#endif

#endif
