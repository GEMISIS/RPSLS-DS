/*
 * Contains a set of generic types, mainly for math related
 * stuff (IE: Coordinates, shapes, etc.)
 * Created by: Gerald McAlister
 */

#ifndef _GENERIC_TYPES_H_
#define _GENERIC_TYPES_H_

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
	int x;
	/*
	 *  The y coordinate.
	 */
	int y;
} coordinates_t;

/*
 *  A struct for sizes.
 */
typedef struct
{
	/*
	 *  The width variable.
	 */
	int width;
	/*
	 *  The height variable.
	 */
	int height;
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
	int radius;
} circle_t;

/*
 * A struct for colors.
*/
typedef struct
{
	/*
	 * The red component for the color.
	*/
	u8 r;
	/*
	 * The green component for the color.
	*/
	u8 g;
	/*
	 * The blue component for the color.
	*/
	u8 b;
} color_t;

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
bool withinArea(int x1, int y1, int x2, int y2, int width, int height);

/*
 * Checks to see if the first point is within the second point and it's boundaries.
 * @param x The point's x position.
 * @param y The point's y position.
 * @param boundingRectangle The location's properties as a rectangle_t object.
 * @return Returns true if there was a collision, false otherwise.
 */
bool withinRectangle(int x, int y, rectangle_t boundingRectangle);

#ifdef __cplusplus
}
#endif

#endif
