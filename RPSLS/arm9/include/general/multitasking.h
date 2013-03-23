/*
 * A multitasking class.  Used with the power button to bring up a standard
 * set of options in a menu.
 * Created by: Gerald McAlister
 */

#ifndef _MULTITASKING_H_
#define _MULTITASKING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
#include <fat.h>

/*
 * Used to check whether the game is paused or not.
 */
extern bool isPaused;

/*
 * Used to check the battery level.
 */
extern u32 batteryLevel;

/*
 * Used to initialize the background menu.  DSi only.
 */
extern void initBackgroundMenu(bool canUseText);

/*
 * Used to update the background menu.  DSi only.
 */
extern void updateBackgroundMenu();

#ifdef __cplusplus
}
#endif

#endif
