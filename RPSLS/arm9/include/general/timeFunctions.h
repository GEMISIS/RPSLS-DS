/*
 * Contains a set of functions and structures for
 * managing the time on the Nintendo DS/DSi.
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
 *  Gets the current amount of seconds gone by according to the DS.
 *  @return The current amount of seconds gone by.
 */
extern int getTimeSeconds();

/*
 *  Gets the current amount of minutes gone by according to the DS.
 *  @return The current amount of minutes gone by
 */
extern int getTimeMinutes();

/*
 *  Gets the current amount of hours gone by according to the DS.
 *  @return The current amount of hours gone by
 */
extern int getTimeHours(bool military);

/*
 *  Gets the current day of the year according to the DS.
 *  @return The current day of the year.
 */
extern int getTimeDayOfYear();

/*
 *  Gets the current day of the week according to the DS.
 *  @return The current day of the week.
 */
extern int getTimeDayOfWeek();

/*
 *  Gets the current day of the month according to the DS.
 *  @return The current day of the month.
 */
extern int getTimeDayOfMonth();

/*
 *  Gets the current month according to the DS.
 *  @return The current month.
 */
extern int getTimeMonth();

/*
 *  Gets the current year according to the DS.
 *  @return The current year.
 */
extern int getTimeYear();

/*
 *  Gets the current time according to the DS as text.
 *  @return A character array containing the time.
 */
extern char* getTimeText();

/*
 *  Updates the current time according to the DS.
 */
extern void updateTime();

/*
 *  Updates the graphics based on the current time of day.
 */
extern void updateGameTime();

#ifdef __cplusplus
}
#endif

#endif
