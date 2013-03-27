/*
 * This file contains some generic functions for handling certain things such
 * as checking a point within a specified area.
 * Created by: Gerald McAlister
 */
#include "GEM_functions.h"

/*
 * Compares a color and rgb values.
 * @param c The color to compare with.
 * @param r The red component.
 * @param g The green component.
 * @param b The blue component.
 * @return Returns true if equal, false otherwise.
*/
bool compareColor(color_t c, int r, int g, int b)
{
	/*
	 * Returns the comparisons of the red, green, and blue
	 * values, all ANDed together.
	*/
	return c.r == r && c.g == g && c.b == b;
}

/*
 * Checks to see if a set of coordinates
 * are within a specified area.
 * @param xPoint The X position of the coordinates.
 * @param yPoint The Y position of the coordinates.
 * @param xRect The X position of the area to search within.
 * @param yRect The Y position of the area to search within.
 * @param rectWidth The width of the area to search within.
 * @param rectHeight The height of the area to search within.
 * @return Returns true if the point is within the area, false otherwise.
*/
bool withinArea(int xPoint, int yPoint, int xRect, int yRect, int rectWidth, int rectHeight)
{
	/*
	 * Returns whether the first set of points point is
	 * within the second set of points according to the
	 * width and height desired.
	 */
	return (xPoint >= xRect && xPoint <= xRect + rectWidth) && (yPoint >= yRect && yPoint <= yRect + rectHeight);
}

/*
 * Checks to see if a point is within a rectangle.
 * @param x The X position of the point.
 * @param y The Y position of the point.
 * @param rectangle The rectangle to check within.
 * @return Returns true if the point is within the rectangle, false otherwise.
*/
bool withinRectangle(int x, int y, rectangle_t rectangle)
{
	/*
	 * Returns whether the first set of points point is
	 * within the desired bounding rectangle.
	 */
	return (x >= rectangle.position.x && x
			<= rectangle.position.x + rectangle.size.width)
			&& (y >= rectangle.position.y && y
					<= rectangle.position.y
							+ rectangle.size.height);
}

/*
 * Updates all of the game's elements.
*/
void updateAll()
{
	/*
	 * Updates the game time.
	 */
	updateTime();

	/*
	 * Updates the user's data.
	 */
	updateUserData();

	/*
	 * Updates the background menu.
	 */
	//updateBackgroundMenu();

	/*
	 * Updates all of the backgrounds.
	 */
	updateBackgrounds();

	/*
	 * Updates all of the sprites.
	 */
	updateSprites();

	/*
	 * Wait for the next vertical blank interrupt.
	 */
	swiWaitForVBlank();

	/*
	 * Updates the top screen's OAM.
	 */
	oamUpdate(&oamMain);
	/*
	 * Updates the bottom screen's OAM.
	 */
	oamUpdate(&oamSub);
}

