/*
 * Contains a set of background functions used to create
 * tiled backgrounds on the Nintendo DS.  These type of backgrounds
 * are very nice because they scroll and don't really have major size
 * limits.
 * Created by: Gerald McAlister
*/

#ifndef _BACKGROUND_FUNCTIONS_H_
#define _BACKGROUND_FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include "generic.h"

/*
 * Defines for a single tile type.
*/
typedef unsigned int tile_t;

/*
 * Defines a type for tile data.
*/
typedef tile_t* tileData_t;
/*
 * Defines a type for map data.
*/
typedef unsigned short* mapData_t;
/*
 * Defines a type for palette data.
*/
typedef unsigned short* paletteData_t;

/*
 * Creates a background and returns an integer that points to it.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param mapBase The map base to use for the background.  (Usually the same as the index)
 * @param tileBase The tile base to use for the background. (Usually the index + 1)
 * @return An integer that points to the background.
 */
extern int createBg(int screen, int index, u32 width, u32 height);

/*
 * Deletes the desired background.
 * @param screen The screen to delete the background on.
 * @param index The index (layer) to delete the background on.
*/
extern void deleteBg(int screen, int index);

/*
 * Adds a set of tiles to a tile base.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param tiles A pointer to the tiles' data.
 * @param tileSize The size of the tiles' data.
 */
extern void setBgTiles(int screen, int index, const unsigned int* tiles, u32 tileSize);

/*
 * Sets the desired background's map.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param map A pointer to the map's data.
 * @param width The width of the background.
 * @param height The height of the background.
 */
extern void setBgMap(int screen, int index, const unsigned short* map, u32 mapSize);

/*
 * Sets the desired background's map with the desired
 * scroll amount.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param x The x position to scroll to.
 * @param y The y position to scroll to.
 */
extern void setBgPosition(int screen, int index, s32 x, s32 y);

/*
 * Sets the desired backgrounds palette to be grayscale.
 * @param screen The screen that the background is on.
 * @param index The index (layer) of the background.
 * @param use Wehther to use grayscale or not
 */
extern void setBgUseGrayscale(int screen, int index, bool use);

/*
 * Sets the desired backgrounds palette.
 * @param screen The screen that the background is on.
 * @param index The index (layer) of the background.
 * @param pal The palette data for the background
 */
extern void setBgPalette(int screen, int index, const unsigned short* pal);

/*
 * Sets the desired backgrounds palette.
 * @param screen The screen that the background is on.
 * @param index The index (layer) of the background.
 * @param spot The color's index in the palette.
 * @param color The color to switch to.
 */
extern void setBgPaletteColor(int screen, int index, int colorIndex, color_t color);

/*
 * Sets the desired background's parallax value with the
 * desired value.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param x The x speed to set the background's parallax value to.
 * @param y The y speed to set the background's parallax value to.
 */
extern void setBgParallaxSpeeds(int screen, int index, int x, int y);

/*
 * Get's the desired background's X parallax value.
 * @param screen The screen to get the parallax value from.
 * @param index The index (layer) to get the parallax value from.
 */
extern int getBgParallaxSpeedsX(int screen, int index);

/*
 * Get's the desired background's Y parallax value.
 * @param screen The screen to get the parallax value from.
 * @param index The index (layer) to get the parallax value from.
 */
extern int getBgParallaxSpeedsY(int screen, int index);

/*
 * Get's the desired background's size.
 * @param screen The screen to get the size from.
 * @param index The index (layer) to get the size from.
 */
extern boundarySize_t getBgSize(int screen, int index);

/*
 * Updates the background system.
*/
extern void updateBackgrounds();

/*
 * Sets the desired background's collision map data.  This data
 * is used for collision detection.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param colMap A pointer to the collision map's map data.
 * @param colMapLen The size of the collision map's map data.
 * @param colTiles A pointer to the collision map's tile data.
 * @param colTilesLen The size of the collision map's tile data.
 * @param colPal A pointer to the collision map's palette data.
*/
extern void setBgCollisionMap(int screen, int index, u32 width, u32 height, const unsigned short* colMap, u32 colMapLen, const unsigned int* colTiles, u32 colTilesLen, const unsigned short* colPal);

/*
 * Deletes the collision map data for the desired background.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
*/
extern void deleteBgCollisionMap(int screen, int index);

/*
 * Gets the tile index at a specific position on a collision background.
 * @param screen The screen to use.
 * @param index The index (layer) to use.
 * @param x The X position to use.
 * @param y The Y position to use.
*/
unsigned int getTileIndex(int screen, int index, int x, int y);

/*
 * Gets the palette index at a specific position on a collision background.
 * @param screen The screen to use.
 * @param index The index (layer) to use.
 * @param x The X position to use.
 * @param y The Y position to use.
*/
unsigned int getPalIndex(int screen, int index, int x, int y);

/*
 * Gets the pixel color at a specific position on a collision background.
 * @param screen The screen to use.
 * @param index The index (layer) to use.
 * @param x The X position to use.
 * @param y The Y position to use.
*/
unsigned int getPixelColor(int screen, int index, int x, int y);

#ifdef __cplusplus
}
#endif

#endif
