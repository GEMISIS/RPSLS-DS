/*
 * Contains a set of functions for drawing text.  Nice because it allows for easy
 * positioning, as well as different fonts.
 * Created by: Gerald McAlister
 */

#ifndef _TEXT_FUNCTIONS_H_
#define _TEXT_FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include <stdio.h>
#include <stdarg.h>

/*
 * Initializes the text system.
 * @param loadDefaultFonts Indicates whether or not it should
 * load the default fonts.
 * @param topScreenLayer The background layer to display the text on on the
 * top screen. (Use -1 to not load the top screen's text system)
 * @param bottomScreenLayer The background layer to display the text on on the
 * bottom screen. (Use -1 to not load the bottom screen's text system)
 */
extern void initTextSystem(bool loadDefaultFonts, int topScreenLayer,
		int bottomScreenLayer);

/*
 * Sets the font on the desired screen with the chosen properties.
 * @param screen The screen to set the font for.
 * @param offSet The offset of the first character.
 * @param numberOfCharacters The total number of characters available for display.
 * @param fontTiles The tiles for the font.
 * @param fontPal The palette for the font.
 */
extern void setFont(int screen, int offSet, int numberOfCharacters,
		const unsigned int* fontTiles, const unsigned short* fontPal);

/*
 * Sets the position of the text.
 * @param screen The screen to select.
 * @param x The x position to put the text at.
 * @param y The y position to put the text at.
 */
extern void setText(int screen, int x, int y);

/*
 * Draws text on the screen with the desired properties.
 * @param screen The screen to draw the text on.
 * @param x The x position to put the text at.
 * @param y The y position to put the text at.
 * @param text The text to draw.  Supports string formating.
 */
extern void drawText(int screen, int x, int y, const char* text, ...);

#ifdef __cplusplus
}
#endif

#endif
