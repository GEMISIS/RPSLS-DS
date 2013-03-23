/*
 * A set of functions made to handle user data.  Certain
 * info comes from the Nintendo DS/DSi itself, but parts
 * of it are custom things added to add support for more
 * features.
 * Created by: Gerald McAlister
 */
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>

#include "GEM_functions.h"

/*
 * A struct to hold the current user's data.
 */
userData_t* currentUser;

/*
 * Create user data with the given file name.
 * @param user The name of the usr file to create.
 */
void createUserData(const char* user)
{
	/*
	 * Initializes variable i to 0.
	 */
	int i = 0;
	/*
	 * Empties the current user's name and sets its length to the
	 * size of the personal data from the user's Nintendo DS/DSi.
	 */
	memset(currentUser->name, 0, PersonalData->nameLen);

	/*
	 * Goes through each letter in the user's DS/DSi name
	 */
	for (i = 0; i < PersonalData->nameLen; i += 1)
	{
		/*
		 * And sets each letter from the user's name.
		 */
		currentUser->name[i] = PersonalData->name[i];
	}
	/*
	 * Then sets the last letter to a null terminated character.
	 */
	currentUser->name[PersonalData->nameLen] = '\0';

	/*
	 * Empties the current user's message and sets its length to the
	 * length of the personal data from the user's Nintendo DS/DSi.
	 */
	memset(currentUser->message, 0, PersonalData->messageLen);

	/*
	 * Goes hrough each latter in the user's DS/DSi message.
	 */
	for (i = 0; i < PersonalData->messageLen; i += 1)
	{
		/*
		 * And sets each letter in the user's message accordingly.
		 */
		currentUser->message[i] = PersonalData->message[i];
	}
	/*
	 * Then, the last character is set to be null terminated.
	 */
	currentUser->message[PersonalData->messageLen] = '\0';

	/*
	 * Then, the day of the user's birthday is set.
	 */
	currentUser->birthday.day = PersonalData->birthDay;
	/*
	 * After that, the user's birthday month is set for the
	 * user.
	 */
	currentUser->birthday.month = PersonalData->birthMonth;

	/*
	 * Then, it checks if the user is not using a Nintendo DSi.
	 */
	if (!REG_DSIMODE)
	{
		/*
		 * And saves if the user isn't (DSi saving currently doesn't work :/)
		 */
		saveUserData(user);
	}

	/*
	 * Finally, the game time is updated for the user's birthday.
	 */
	updateGameTime();
}

/*
 * Loads user data with the given file name.
 * @param user The name of the usr file to create.
 */
void loadUserData(const char* user)
{
	/*
	 * Creates a temporary file name for the file.
	 */
	char fileName[65];

	/*
	 * Copies the user's name to the file name.
	 */
	strncpy(fileName, user, (strlen(user) > 60) ? 60 : strlen(user));
	/*
	 * Concatenates the .usr extension to the end of the user file.
	 */
	strncat(fileName, ".usr", 4);
	/*
	 * Finally, the last character in the file name is set to be null terminated.
	 */
	fileName[strlen(user) + 4] = '\0';

	/*
	 * Then, it attempts to switch to the data directory.
	 */
	if (chdir("data") != 0)
	{
		/*
		 * If it fails to switch to the data directory, then
		 * the data directory is created.
		 */
		mkdir("data", 0777);
		/*
		 * It is then switched to again.
		 */
		chdir("data");
	}
	/*
	 * It then attempts to switch to the applications speicifc data
	 * directory.
	 */
	if (chdir(GAME_TITLE) != 0)
	{
		/*
		 * If that fails, then it creates the directory.
		 */
		mkdir(GAME_TITLE, 0777);
		/*
		 * Then, it goes into it.
		 */
		chdir(GAME_TITLE);
	}

	/*
	 * Then, the file is opened in reading binary mode.
	 */
	FILE* file = fopen(fileName, "rb");

	/*
	 * Checks if the file exists.
	 */
	if (file != NULL)
	{
		/*
		 * If so, then the current users is allocated to the size of a user.
		 */
		currentUser = (userData_t*) malloc(sizeof(userData_t));
		/*
		 * Then, the user's data is read into the current user structure.
		 */
		fread(currentUser, 1, sizeof(userData_t), file);

		/*
		 * Afterwards, the file is closed.
		 */
		fclose(file);

		/*
		 * And the system goes back to the root directory.
		 */
		chdir("../..");
	}
	else
	{
		/*
		 * Otherwise, the system goes back to the root directory.
		 */
		chdir("../..");

		/*
		 * And the user's data file is created.
		 */
		createUserData(user);
	}

	/*
	 * Finally, the system checks whether it is currently the user's birthday.
	 */
	currentUser->isBirthdayToday = isUserBirthday();
}

/*
 * Saves user data with the given file name.
 * @param user The name of the usr file to create.
 */
void saveUserData(const char* user)
{
	/*
	 * Creates a temporary file name for the file.
	 */
	char fileName[65];

	/*
	 * Copies the user's name to the file name.
	 */
	strncpy(fileName, user, (strlen(user) > 60) ? 60 : strlen(user));
	/*
	 * Concatenates the .usr extension to the end of the user file.
	 */
	strncat(fileName, ".usr", 4);
	/*
	 * Finally, the last character in the file name is set to be null terminated.
	 */
	fileName[strlen(user) + 4] = '\0';

	/*
	 * Then, it attempts to switch to the data directory.
	 */
	if (chdir("data") != 0)
	{
		/*
		 * If it fails to switch to the data directory, then
		 * the data directory is created.
		 */
		mkdir("data", 0777);
		/*
		 * It is then switched to again.
		 */
		chdir("data");
	}
	/*
	 * It then attempts to switch to the applications speicifc data
	 * directory.
	 */
	if (chdir(GAME_TITLE) != 0)
	{
		/*
		 * If that fails, then it creates the directory.
		 */
		mkdir(GAME_TITLE, 0777);
		/*
		 * Then, it goes into it.
		 */
		chdir(GAME_TITLE);
	}

	/*
	 * Then, the file is opened in writing binary mode.
	 */
	FILE* file = fopen(fileName, "wb");

	/*
	 * Afterwards, the current user's data is written to the
	 * file.
	 */
	fwrite(currentUser, 1, sizeof(userData_t), file);

	/*
	 * The the file is closed.
	 */
	fclose(file);

	/*
	 * And the Nintendo DS/DSi switches back to the root directory.
	 */
	chdir("../..");
}

/*
 * Deletes user data with the given file name.
 * @param user The name of the usr file to create.
 */
void deleteUserData(const char* user)
{
	/*
	 * Creates a temporary file name for the file.
	 */
	char fileName[65];

	/*
	 * Copies the user's name to the file name.
	 */
	strncpy(fileName, user, (strlen(user) > 60) ? 60 : strlen(user));
	/*
	 * Concatenates the .usr extension to the end of the user file.
	 */
	strncat(fileName, ".usr", 4);
	/*
	 * Finally, the last character in the file name is set to be null terminated.
	 */
	fileName[strlen(user) + 4] = '\0';

	/*
	 * Then, it attempts to switch to the data directory.
	 */
	if (chdir("data") != 0)
	{
		/*
		 * If it fails to switch to the data directory, then
		 * the data directory is created.
		 */
		mkdir("data", 0777);
		/*
		 * It is then switched to again.
		 */
		chdir("data");
	}
	/*
	 * It then attempts to switch to the applications speicifc data
	 * directory.
	 */
	if (chdir(GAME_TITLE) != 0)
	{
		/*
		 * If that fails, then it creates the directory.
		 */
		mkdir(GAME_TITLE, 0777);
		/*
		 * Then, it goes into it.
		 */
		chdir(GAME_TITLE);
	}

	/*
	 * The file is then removed from the device.
	 */
	remove(fileName);

	/*
	 * And the directory is set to the root directory again.
	 */
	chdir("../..");
}

/*
 * Updates the user's data (IE: birthday, events, etc.)
 */
void updateUserData()
{
	/*
	 * Updates the isBirthdayToday boolean value each time so that
	 * if the user's starts playing on their birthday, then it updates
	 * correctly.
	 */
	currentUser->isBirthdayToday = isUserBirthday();
}

/*
 * Tells whether it is the current user's birthday or not.
 */
bool isUserBirthday()
{
	/*
	 * Returns whether the current month and current day of the month are the same
	 * as the user's birthday day and month.
	 */
	return currentUser->birthday.month == getTimeMonth()
			&& currentUser->birthday.day == getTimeDayOfMonth();
}

