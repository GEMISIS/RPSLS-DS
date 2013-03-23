/*
 * This file contains functions for creating and reading achievements for various
 * games.  These achievements each contain an amount of points that can be used
 * for various in game things.
 * Created by: Gerald McAlister
 */
#include "GEM_functions.h"

/*
 * Adds an achievement to the user's save profile.
 * @param pointValue The point value of how many points
 * the achievement is worth.
 * @param gameID The identification for the game.  Should be
 * 8 characters long.
 */
void addAchievement(int pointValue, const char* gameID,
		const char* achievementTitle)
{
	/*
	 * A temporary holding spot for a file name.
	 */
	char fileName[65];
	/*
	 * Copies the current user name to the first part of the file name.
	 */
	strncpy(fileName, currentUser->name, 10);
	/*
	 * Copies the achievement extension (.ach) to the end of the file name.
	 */
	strncat(fileName, ".ach", 4);
	/*
	 * Makes the end of the file name null terminated.
	 */
	fileName[strlen(currentUser->name) + 4] = '\0';

	/*
	 * Changes the directory to the data directory and checks
	 * to make sure it changed correctly.
	 */
	if (chdir("data") != 0)
	{
		/*
		 * If it did not change correctly, then
		 * the data folder is created.
		 */
		mkdir("data", 0777);

		/*
		 * Then it is switched to again.
		 */
		chdir("data");
	}

	/*
	 * Changes the directory to the game's data directory and checks
	 * to make sure it changed correctly.
	 */
	if (chdir(GAME_TITLE) != 0)
	{
		/*
		 * If it did not change correctly, then
		 * the game's data folder is created.
		 */
		mkdir(GAME_TITLE, 0777);

		/*
		 * Then it is switched to again.
		 */
		chdir(GAME_TITLE);
	}

	/*
	 * The file is then opened for appending in binary mode.
	 */
	FILE* file = fopen(fileName, "a+b");

	/*
	 * Checks if the file was opened correctly.
	 */
	if (file != NULL)
	{
		/*
		 * Then, a temporary achievement is created.
		 */
		achievement_t* achievement = (achievement_t*) NULL;
		/*
		 * Then, variable i is created and set to 0.
		 */
		int i = 0;
		/*
		 * Then, i is incremented by 1 as long as
		 * i is < 8.
		 */
		for (i = 0; i < 8; i += 1)
		{
			/*
			 * Each time, a character from the game's id is set.
			 */
			achievement->gameID[i] = gameID[i];
		}
		/*
		 * Then, i is incremented by 1 as long as it is less then the max length
		 * of a achievement title.
		 */
		for (i = 0; i < MAX_ACHIEVEMENT_TITLE_LENGTH && i < strlen(
				achievementTitle); i += 1)
		{
			/*
			 * Each time, a character from the achievement title is set.
			 */
			achievement->achievementTitle[i] = achievementTitle[i];
		}
		/*
		 * The point value is then set.
		 */
		achievement->pointValue = pointValue;
		/*
		 * After that, the current day of the month as the unlocked date.
		 */
		achievement->unlockedDate.day = getTimeDayOfMonth();
		/*
		 * Then, the current month as the unlocked date.
		 */
		achievement->unlockedDate.month = getTimeMonth();
		/*
		 * Finally, the year that it was unlocked (aka: the current year) is set.
		 */
		achievement->unlockedDate.year = getTimeYear();

		/*
		 * Then, the end of the file is gone to.
		 */
		fseek(file, 0, SEEK_END);
		/*
		 * Then the achievement is written to the file.
		 */
		fwrite(achievement, 1, sizeof(achievement_t), file);
		/*
		 * After words, the file is closed.
		 */
		fclose(file);
	}
	/*
	 * Finally, the directory is changed back to the root directory.
	 */
	chdir("../../");
}

/*
 * Gets an achievement based on it's index (they are sorted by date.
 * @param index The index of the achievement.
 * @return The achievement as a structure.
 */
achievement_t getAchievementFromIndex(int index)
{
	/*
	 * A temporary holding spot for a file name.
	 */
	char fileName[65];
	/*
	 * Copies the current user name to the first part of the file name.
	 */
	strncpy(fileName, currentUser->name, 10);
	/*
	 * Copies the achievement extension (.ach) to the end of the file name.
	 */
	strncat(fileName, ".ach", 4);
	/*
	 * Makes the end of the file name null terminated.
	 */
	fileName[strlen(currentUser->name) + 4] = '\0';

	/*
	 * Changes the directory to the data directory and checks
	 * to make sure it changed correctly.
	 */
	if (chdir("data") != 0)
	{
		/*
		 * If it did not change correctly, then
		 * the data folder is created.
		 */
		mkdir("data", 0777);

		/*
		 * Then it is switched to again.
		 */
		chdir("data");
	}

	/*
	 * Changes the directory to the game's data directory and checks
	 * to make sure it changed correctly.
	 */
	if (chdir(GAME_TITLE) != 0)
	{
		/*
		 * If it did not change correctly, then
		 * the game's data folder is created.
		 */
		mkdir(GAME_TITLE, 0777);

		/*
		 * Then it is switched to again.
		 */
		chdir(GAME_TITLE);
	}

	/*
	 * Then, the file with the chosen name is opened in reading binary mode.
	 */
	FILE* file = fopen(fileName, "rb");

	/*
	 * Then, a temporary achievement is created.
	 */
	achievement_t* achievement = (achievement_t*) NULL;

	/*
	 * Checks if the file was opened correctly.
	 */
	if (file != NULL)
	{
		/*
		 * If it was, then the index of the achievement is gotten to.
		 */
		fseek(file, index * sizeof(achievement_t), SEEK_CUR);

		/*
		 * Then, the achievement is read to the temporary achievement.
		 */
		fread(achievement, 1, sizeof(achievement_t), file);

		/*
		 * Afterwards, the file is closed.
		 */
		fclose(file);
	}

	/*
	 * Then, the directory is changed to the root directory.
	 */
	chdir("../../");

	/*
	 * Finally, the achievement is returned.
	 */
	return *achievement;
}

