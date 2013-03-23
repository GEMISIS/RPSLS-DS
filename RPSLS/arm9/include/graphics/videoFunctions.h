/*
 * A set of functions used for video management.  IE: Vram, screen flipping, etc.
 * Created by: Gerald McAlister
 */

#ifndef __VIDEO_FUNCTIONS_H__
#define __VIDEO_FUNCTIONS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>

/*
 *  A boolean value that represents whether the screens
 *  are flipped or not.
 */
extern bool screensFlipped;

/*
 *  Initializes the video for the nintendo DS.
 *  Sets up the screens to use backgrounds and sprites with the
 *  2D engine.
 */
extern void initVideo();

/*
 *  This function switches the sub screen and the main screen.
 *  Can be used for using 3D on the sub screen.
 */
extern void switchScreens();

#ifdef __cplusplus
}
#endif

#endif
