/*
 * This file contains functions used to create and read achievements.  These
 * achievements can be used as rewards for players in games, and then
 * can be read by the an application that makes use of this.
 * Each Achievement contains a point value, which can be used for
 * various DLC (Downloadable Content) in other games, or whatever else.
 * Created by: Gerald McAlister
 */

#ifndef _ACHIEVEMENTS_H_
#define _ACHIEVEMENTS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

/*
 * The include for the time functions so that it can retrieve the date
 * that the achievement was unlocked when creating one.
 */
#include "timeFunctions.h"

/*
 * The max length of an achievement's title.
 */
#define MAX_ACHIEVEMENT_TITLE_LENGTH 64

/*
 * The structure for each achievement.
 * Contains:
 * pointValue - The value of the achievement
 * in points.
 * title - The title of the game.
 * gameID - A 4 character code representing
 * the game.
 * unlockedDate - The date that the achievement
 * was unlocked.
 */
typedef struct achievement_t
{
	int pointValue;
	char gameID[8];
	char achievementTitle[MAX_ACHIEVEMENT_TITLE_LENGTH];
	specificDate_t unlockedDate;
} achievement_t;

/*
 * Adds an achievement to the user's save profile.
 * @param pointValue The point value of how many points
 * the achievement is worth.
 * @param gameID The identification for the game.  Should be
 * 8 characters long.
 * @param achievementTitle The title of the achievement.
 */
extern void addAchievement(int pointValue, const char* gameID,
		const char* achievementTitle);

/*
 * Gets an achievement based on it's index (they are sorted by date.
 * @param index The index of the achievement.
 * @return The achievement as a structure.
 */
extern achievement_t getAchievementFromIndex(int index);

#ifdef __cplusplus
}
#endif

#endif

