/*
 * This file contains some generic functions for handling certain things such
 * as checking a point within a specified area.
 * Created by: Gerald McAlister
 */

#ifndef _GENERIC_H_
#define _GENERIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>

/*
 *  A struct for x and y coordinates.
 */
typedef struct
{
	/*
	 *  The x coordinate.
	 */
	s32 x;
	/*
	 *  The y coordinate.
	 */
	s32 y;
} coordinates_t;

/*
 *  A struct for sizes.
 */
typedef struct
{
	/*
	 *  The width variable.
	 */
	s32 width;
	/*
	 *  The height variable.
	 */
	s32 height;
} boundarySize_t;

/*
 *  A struct for rectangles.
 */
typedef struct
{
	/*
	 *  The position of the rectangle.
	 */
	coordinates_t position;
	/*
	 *  The size of the rectangle.
	 */
	boundarySize_t size;
} rectangle_t;

/*
 *  A struct for rectangle sides.
 */
enum sides_t { NO_COLLISION = 1, LEFT = 2, RIGHT = 3, TOP = 4, BOTTOM = 5};

/*
 *  A struct for circles.
 */
typedef struct
{
	/*
	 *  The position of the circle.
	 */
	coordinates_t position;
	/*
	 *  The radius of the circle.
	 */
	u32 radius;
} circle_t;

/*
 * A struct for colors.
*/
typedef struct
{
	/*
	 * The red component for the color.
	*/
	int r;
	/*
	 * The green component for the color.
	*/
	int g;
	/*
	 * The blue component for the color.
	*/
	int b;
} color_t;

/*
 * Compares a color and rgb values.
 * @param c The color to compare with.
 * @param r The red component.
 * @param g The green component.
 * @param b The blue component.
 * @return Returns true if equal, false otherwise.
*/
extern bool compareColor(color_t c, int r, int g, int b);

/*
 * Checks to see if the first point is within the second point and it's boundaries.
 * @param x1 The point's x position.
 * @param y1 The point's y position.
 * @param x2 The location's x position.
 * @param y2 The location's y position.
 * @param width The location's width.
 * @param height The location's height.
 * @return Returns true if there was a collision, false otherwise.
 */
extern bool withinArea(int x1, int y1, int x2, int y2, int width, int height);

/*
 * Checks to see if the first point is within the second point and it's boundaries.
 * @param x The point's x position.
 * @param y The point's y position.
 * @param boundingRectangle The location's properties as a rectangle_t object.
 * @return Returns true if there was a collision, false otherwise.
 */
extern bool withinRectangle(int x, int y, rectangle_t boundingRectangle);

/*
 * Checks to see if there is a collision between two rectangles.
 * @param rect1 The first rectangle to use.
 * @param rect2 The second rectangle to use.
 * @return Returns true if there was a collision, false otherwise.
 */
extern bool rectangleCollision(rectangle_t rect1, rectangle_t rect2);

/*
 *  Updates everything that is in need of updating.
 */
extern void updateAll();

#ifdef __cplusplus
}
#endif

#endif
