/*
 * Contains a set of includes needed for the library.
 * Created by: GEMISIS
 */

#ifndef _GEM_FUNCTIONS_H_
#define _GEM_FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Tells whether the application has a custom title.
 */
#define HAS_TITLE 1

/*
 * The custom title for the application.  Used for saving data.
 */
#define GAME_TITLE "RPSLS"

/*
 * The title of the game is checked for to make sure
 * that an error is not created when using user data
 * or achievements.
 */
#if HAS_TITLE == 0
#error In GEM_functions.h: Please create a global define that is called GAME_TITLE with\
	a defination of the title of the game. Then set\
	the HAS_TITLE definition to 1.
#endif

#include "generic.h"
#include "videoFunctions.h"
#include "textFunctions.h"
#include "backgrounds.h"
#include "sprites.h"
#include "multitasking.h"
#include "timeFunctions.h"
#include "achievements.h"
#include "fileIO.h"
#include "userDataFunctions.h"

#ifdef __cplusplus
}
#endif

#endif
