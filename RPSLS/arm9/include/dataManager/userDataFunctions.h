/*
 * A set of functions used to manage user data.  Contains
 * everything from the current user that is on, to their friends list
 * to their events and achievements.
 * Created by: Gerald McAlister
 */

#ifndef _USER_DATA_FUNCTIONS_H_
#define _USER_DATA_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
#include "timeFunctions.h"

/*
 * A structure containing the user's data.
 * isBirthdayToday - Tells whether the user's
 * birthday is that day.
 * name - The name of the user. (Found on DS/DSi)
 * message - The user's message. (Found on DS/DSi)
 * birthday - A specific date containing the user's
 * birthday. (Found on DS/DSi)
 */
typedef struct userData_t
{
	bool isBirthdayToday;
	char name[10];
	char message[26];
	specificDate_t birthday;
}userData_t;

/*
 * The currently logged in user.
 */
extern userData_t* currentUser;

/*
 * Create user data with the given file name.
 * @param user The name of the usr file to create.
 */
extern void createUserData(const char* user);

/*
 * Loads user data with the given file name.
 * @param user The name of the usr file to create.
 */
extern void loadUserData(const char* user);

/*
 * Saves user data with the given file name.
 * @param user The name of the usr file to create.
 */
extern void saveUserData(const char* user);

/*
 * Deletes user data with the given file name.
 * @param user The name of the usr file to create.
 */
extern void deleteUserData(const char* user);

/*
 * Updates the user's data (IE: birthday, events, etc.)
 */
extern void updateUserData();

/*
 * Tells whether it is the current user's birthday or not.
 */
extern bool isUserBirthday();

#ifdef __cplusplus
}
#endif

#endif
