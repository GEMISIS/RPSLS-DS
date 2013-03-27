/*
 * Contains a set of background functions used to create
 * tiled backgrounds on the Nintendo DS.  These type of backgrounds
 * are very nice because they scroll and don't really have major size
 * limits.
 * Created by: Gerald McAlister
*/

/*
 * The basic includes for backgrounds.c.
*/
#include "backgrounds.h"

/*
 * Keeps track of which layers each background index is on.
*/
int indexLayers[2][4];

/*
 * Keeps track of each background.
*/
int bgTracker[2][4] = {{-1, -1, -1, -1}, {-1, -1, -1, -1}};

/*
 * Keeps track of the X blocks for each background.
 * This is used when scrolling a background.
*/
u32 xBlocks[2][4];
/*
 * Keeps track of the Y blocks for each background.
 * This is used when scrolling a background.
*/
u32 yBlocks[2][4];

/*
 * Keeps track of the coordinates of each background.
*/
coordinates_t bgPositions[2][4];

/*
 * Keeps track of the sizes of each background.
*/
boundarySize_t bgSizes[2][4];
/*
 * Keeps track of the sizes of each background.
*/
boundarySize_t colBgSizes[2][4];

/*
 * The parallax speeds on the X axis for the backgrounds.
*/
int bgParallaxXSpeeds[2][4];
/*
 * The parallax speeds on the Y axis for the backgrounds.
*/
int bgParallaxYSpeeds[2][4];

/*
 * Tells whether to use grayscale colors for the backgrounds or not.
 */
bool useGrayscale[2][4];

/*
 * A holder for the various backgrounds' map data.
*/
mapData_t mapData[2][4];
/*
 * A holder for the various backgrounds' tile data.
*/
tileData_t tileData[2][4];
/*
 * A holder for the various backgrounds' palette data.
*/
paletteData_t paletteData[2][4];
/*
 * A holder for the various backgrounds' grayscale palette data.
*/
paletteData_t grayscalePaletteData[2][4];
/*
 * A holder for the various background's collision map map data.
*/
mapData_t colMapData[2][4];
/*
 * A holder for the various backgrounds' collision map tile data.
*/
tileData_t colTileData[2][4];
/*
 * A holder for the various backgrounds' collision map palette data.
*/
paletteData_t colPaletteData[2][4];

/*
 * Creates a background and returns an integer that points to it.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param width The width of the background.
 * @param height The height of the background.
 * @return An integer that points to the background.
*/
int createBg(int screen, int index, u32 width, u32 height)
{
	xBlocks[screen][index] = 0;
	yBlocks[screen][index] = 0;

	/*
	 * Sets the background's X coordinate.
	*/
	bgPositions[screen][index].x = 0;
	/*
	 * Sets the background's Y coordinate.
	*/
	bgPositions[screen][index].y = 0;

	/*
	 * Set the width of the background.
	*/
	bgSizes[screen][index].width = width;
	/*
	 * Set the height of the background.
	*/
	bgSizes[screen][index].height = height;

	/*
	 * Sets the background's parallax speed on the X axis
	 * to 1.
	*/
	bgParallaxXSpeeds[screen][index] = 0;
	/*
	 * Sets the background's parallax speed on the Y axis
	 * to 1.
	*/
	bgParallaxYSpeeds[screen][index] = 0;

	/*
	 * Sets the default layer to the index.
	*/
	indexLayers[screen][index] = index;

	BgSize size = (bgSizes[screen][index].width > 256 && bgSizes[screen][index].height > 256) ? BgSize_T_512x512 :
		(bgSizes[screen][index].width > 256) ? BgSize_T_512x256 :
		(bgSizes[screen][index].width > 256) ? BgSize_T_256x512 :
		BgSize_T_256x256;

	/*
	 * Checks to see if the screen variable is <= 0, if it is then...
	 */
	if (screen <= 0 && bgTracker[screen][index] == -1)
	{
		/*
		 * the background on the sub screen is initialized with the default settings of a text bg.
		 * If it is > 0, then...
		 */
		bgTracker[screen][index] = bgInitSub(index, BgType_Text8bpp, size, index * 4, (index < 3) ? index * 2 + 2 : index * 2 + 1);
	}
	else if(bgTracker[screen][index] == -1)
	{
		/*
		 * The background on the main screen is initialized with the default settings of a text bg.
		 */
		bgTracker[screen][index] = bgInit(index, BgType_Text8bpp, size, index * 4, (index < 3) ? index * 2 + 2 : index * 2 + 1);
	}

	/*
	 * Sets the backgrounds priority to the index.
	 */
	bgSetPriority(bgTracker[screen][index], index);

	/*
	 * Makes sure to show the background.
	*/
	bgShow(bgTracker[screen][index]);

	/*
	 * Returns the background so that it can be used outside of this file.
	*/
	return bgTracker[screen][index];
}

/*
 * Deletes the desired background.
 * @param screen The screen to delete the background on.
 * @param index The index (layer) to delete the background on.
*/
void deleteBg(int screen, int index)
{
	if(bgTracker[screen][index] == -1)
	{
		return;
	}

	if(mapData[screen][index] != NULL)
	{
		free(mapData[screen][index]);
		mapData[screen][index] = NULL;
	}
	if(tileData[screen][index] != NULL)
	{
		free(tileData[screen][index]);
		tileData[screen][index] = NULL;
	}
	if(paletteData[screen][index] != NULL)
	{
		free(paletteData[screen][index]);
		paletteData[screen][index] = NULL;
	}
	if(grayscalePaletteData[screen][index] != NULL)
	{
		free(grayscalePaletteData[screen][index]);
		grayscalePaletteData[screen][index] = NULL;
	}
	deleteBgCollisionMap(screen, index);

	if(bgTracker[screen][index] != -1)
	{
		bgHide(bgTracker[screen][index]);
	}
}

/*
 * Sets the for the background to use.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param tiles A pointer to the tiles' data.
 * @param tileSize The size of the tiles' data.
 */
void setBgTiles(int screen, int index, const unsigned int* tiles, u32 tileSize)
{
	if(tileData[screen][index] != NULL)
	{
		free(tileData[screen][index]);
		tileData[screen][index] = NULL;
	}

	/*
	 * Allocate the appropriate amount of memory for the tiles.
	 */
	tileData[screen][index] = calloc(tileSize, sizeof(tile_t));

	/*
	 * This copies the tiles to the desired background slot's
	 * tile data pointer.
	 */
	memcpy(tileData[screen][index], tiles, tileSize);
	
	/*
	 * This copies the tiles to the desired background slot's
	 * graphics pointer.
	 */
	memcpy(bgGetGfxPtr(bgTracker[screen][index]), tileData[screen][index], tileSize);
}

/*
 * Sets the desired background's map.  This data draws the tiles at chosen positions.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param map A pointer to the map's data.
 * @param mapSize The size of the map's data.
 */
void setBgMap(int screen, int index, const unsigned short* map, u32 mapSize)
{
	if(mapData[screen][index] != NULL)
	{
		free(mapData[screen][index]);
		mapData[screen][index] = NULL;
	}

	/*
	 * Allocate the appropriate amount of memory for the map.
	 */
	mapData[screen][index] = calloc(mapSize, sizeof(unsigned short));

	// Copy the map to the map data pointer.
	memcpy(mapData[screen][index], map, mapSize);

	/*
	 * If one of the block values has changed, then the data for the background is copied over again, starting with the top half due to how the data is layed out.
	*/
	memcpy(((unsigned short*)bgGetMapPtr(bgTracker[screen][index])), mapData[screen][index] + ((0 + (0 * (bgSizes[screen][index].width >> 8))) << 10), 4096);

	/*
	 * Then copy the second half.
	*/
	memcpy(((unsigned short*)bgGetMapPtr(bgTracker[screen][index])) + 2048, mapData[screen][index] + ((0 + (0 * (bgSizes[screen][index].width >> 8))) << 10) + ((bgSizes[screen][index].width >> 8) << 10), 4096);
}

/*
 * Sets the desired background's map with the desired
 * scroll amount.
 * @param screen The screen to create the background on.
 * @param index The index (layer) to create the background on.
 * @param x The x position to scroll to.
 * @param y The y position to scroll to.
 */
void setBgPosition(int screen, int index, s32 x, s32 y)
{
	/*
	 * Adjusts the backgrounds X position based on the backgroudnd's speed.
	*/
	if(bgParallaxXSpeeds[screen][index] < 0)
	{
		x = x >> (abs(bgParallaxXSpeeds[screen][index]));
	}
	else if(bgParallaxXSpeeds[screen][index] > 0)
	{
		x = x << (abs(bgParallaxXSpeeds[screen][index]));
	}
	/*
	 * Adjusts the backgrounds Y position based on the backgroudnd's speed.
	*/
	if(bgParallaxYSpeeds[screen][index] < 0)
	{
		y = y >> (abs(bgParallaxYSpeeds[screen][index]));
	}
	else if(bgParallaxYSpeeds[screen][index] > 0)
	{
		y = y << (abs(bgParallaxYSpeeds[screen][index]));
	}

	/*
	 * Sets the background's X coordinate.
	*/
	bgPositions[screen][index].x = x;
	/*
	 * Sets the background's Y coordinate.
	*/
	bgPositions[screen][index].y = y;

	/*
	 * Gets the current X block.
	*/
	s32 blockX = (x >> 8);
	/*
	 * Gets the current Y block.
	*/
	s32 blockY = (y >> 8);

	/*
	 * Gets the current scrolling for the X axis.
	*/
	s32 sx = x - (blockX << 8);
	/*
	 * Gets the current scrolling for the Y axis.
	*/
	s32 sy = y - (blockY << 8);

	/*
	 * Checks that the X coordinate chosen is within the backgrounds
	 * right boundaries.
	*/
	if(x > bgSizes[screen][index].width - 256)
	{
		/*
		 * If it is not within it, then the scrolling X value is set
		 * to the max X coordinate value.
		*/
		sx = bgSizes[screen][index].width - 256;
		/*
		 * Then, the X block value is set to its proper value based
		 * on the new scrolling X value.
		*/
		blockX = sx >> 8;
		/*
		 * Finally, the scrolling X value is set to its proper value.
		*/
		sx = sx - (blockX << 8);
	}
	/*
	 * Checks that the Y coordinate chosen is within the backgrounds
	 * bottom boundaries.
	*/
	if(y > bgSizes[screen][index].height - 192)
	{
		/*
		 * If it is not within it, then the scrolling Y value is set
		 * to the max Y coordinate value.
		*/
		sy = bgSizes[screen][index].height - 192;
		/*
		 * Then, the Y block value is set to its proper value based
		 * on the new scrolling Y value.
		*/
		blockY = sy >> 8;
		/*
		 * Finally, the scrolling Y value is set to its proper value.
		*/
		sy = sy - (blockY << 8);
	}

	/*
	 * Checks that the X coordinate chosen is within the backgrounds
	 * left boundaries.
	*/
	if(x < 0)
	{
		/*
		 * If it is not within it, then the The scrolling X
		 * value if set to 0.
		*/
		sx = 0;
		/*
		 * Then, the X block value is set to its proper value based
		 * on the new scrolling X value.
		*/
		blockX = sx >> 8;
		/*
		 * Finally, the scrolling X value is set to its proper value.
		*/
		sx = sx - (blockX << 8);
	}
	/*
	 * Checks that the Y coordinate chosen is within the backgrounds
	 * top boundaries.
	*/
	if(y < 0)
	{
		/*
		 * If it is not within it, then the The scrolling Y
		 * value if set to 0.
		*/
		sy = 0;
		/*
		 * Then, the Y block value is set to its proper value based
		 * on the new scrolling Y value.
		*/
		blockY = sy >> 8;
		/*
		 * Finally, the scrolling Y value is set to its proper value.
		*/
		sy = sy - (blockY << 8);
	}

	/*
	 * This checks to see if the block values have changed
	 * for the background.
	*/
	if(xBlocks[screen][index] != blockX || yBlocks[screen][index] != blockY)
	{
		/*
		 * If one of the block values has changed, then the data for the background is copied over again, starting with the top half due to how the data is layed out.
		*/
		memcpy(((unsigned short*)bgGetMapPtr(bgTracker[screen][index])), mapData[screen][index] + ((blockX + (blockY * (bgSizes[screen][index].width >> 8))) << 10), 4096);

		/*
		 * Then copy the second half.
		*/
		memcpy(((unsigned short*)bgGetMapPtr(bgTracker[screen][index])) + 2048, mapData[screen][index] + ((blockX + (blockY * (bgSizes[screen][index].width >> 8))) << 10) + ((bgSizes[screen][index].width >> 8) << 10), 4096);

		/*
		 * Sets the X block for the background to the new X block value.
		*/
		xBlocks[screen][index] = blockX;
		/*
		 * Sets the Y block for the background to the new Y block value.
		*/
		yBlocks[screen][index] = blockY;
	}
	/*
	 * Scrolls the background.
	*/
	bgSetScroll(bgTracker[screen][index], sx, sy);
}

/*
 * Sets the desired backgrounds palette.
 * @param screen The screen that the background is on.
 * @param index The index (layer) of the background.
 * @param pal The palette data for the background
 */
void setBgPalette(int screen, int index, const unsigned short* pal)
{
	if(paletteData[screen][index] != NULL)
	{
		free(paletteData[screen][index]);
		paletteData[screen][index] = NULL;
	}
	paletteData[screen][index] = calloc(512, sizeof(unsigned short));
	memcpy(paletteData[screen][index], pal, 512);

	setBgUseGrayscale(screen, index, useGrayscale[screen][index]);

	/*
	 * Checks to see if the screen variable is <= 0.  If it is...
	 */
	if (screen <= 0)
	{
		/*
		 * Then VRAM Bank H is used for the palette.
		 * The first thing done is that the bank is
		 * set to map the LCD.
		 */
		vramSetBankH(VRAM_H_LCD);
		/*
		 * Then, the palette is copied to the bank's
		 * extended palette at the indicated slot.
		 */
		if(useGrayscale[screen][index])
		{
			memcpy((void*)(0x6898000 + (index << 13)), grayscalePaletteData[screen][index], 512);
		}
		else
		{
			memcpy((void*)(0x6898000 + (index << 13)), paletteData[screen][index], 512);
		}
		/*
		 * Finally, the bank is set to map its extended
		 * palette slot.
		 */
		vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	}
	else
	{
		/*
		 * If it is > 0, then VRAM Bank E is sued for the palette.
		 * The first thing done is that the bank is
		 * set to map the LCD.
		 */
		vramSetBankE(VRAM_E_LCD);
		/*
		 * Then, the palette is copied to the bank's
		 * extended palette at the indicated slot.
		 */
		if(useGrayscale[screen][index])
		{
			memcpy((void*)(0x6880000 + (index << 13)), grayscalePaletteData[screen][index], 512);
		}
		else
		{
			memcpy((void*)(0x6880000 + (index << 13)), paletteData[screen][index], 512);
		}
		/*
		 * Finally, the bank is set to map its extended
		 * palette slot.
		 */
		vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	}
}

/*
 * Sets the desired backgrounds palette to be grayscale.
 * @param screen The screen that the background is on.
 * @param index The index (layer) of the background.
 * @param use Wehther to use grayscale or not
 */
void setBgUseGrayscale(int screen, int index, bool use)
{
	if(grayscalePaletteData[screen][index] != NULL)
	{
		free(grayscalePaletteData[screen][index]);
		grayscalePaletteData[screen][index] = NULL;
	}
	grayscalePaletteData[screen][index] = calloc(512, sizeof(unsigned short));

	uint32_t i = 0;
	for(i = 0;i < 512;i += 1)
	{
		u16 color_hex = ((u16*)paletteData[screen][index])[i];

		color_t color;
		color.r = (((color_hex & 0x1F) + ((color_hex & 0x1F) & 0x1)) << 3) - ((color_hex & 0x1F) & 0x1);
		color.g = ((((color_hex & 0x3E0) >> 5) + (((color_hex & 0x3E0) >> 5) & 0x1)) << 3) - (((color_hex & 0x3E0) >> 5) & 0x1);
		color.b = ((((color_hex & 0x7C00) >> 10) + (((color_hex & 0x7C00) >> 10) & 0x1)) << 3) - (((color_hex & 0x7C00) >> 10) & 0x1);

		u16 color_value = (((u16)((((double)color.r) * 0.3) + (((double)color.g) * 0.59) + (((double)color.b) * 0.11))) >> 3);

		grayscalePaletteData[screen][index][i] = RGB15(color_value, color_value, color_value);
	}

	useGrayscale[screen][index] = use;

	/*
	 * Checks to see if the screen variable is <= 0.  If it is...
	 */
	if (screen <= 0)
	{
		/*
		 * Then VRAM Bank H is used for the palette.
		 * The first thing done is that the bank is
		 * set to map the LCD.
		 */
		vramSetBankH(VRAM_H_LCD);
		/*
		 * Then, the palette is copied to the bank's
		 * extended palette at the indicated slot.
		 */
		if(useGrayscale[screen][index])
		{
			memcpy((void*)(0x6898000 + (index << 13)), grayscalePaletteData[screen][index], 512);
		}
		else
		{
			memcpy((void*)(0x6898000 + (index << 13)), paletteData[screen][index], 512);
		}
		/*
		 * Finally, the bank is set to map its extended
		 * palette slot.
		 */
		vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	}
	else
	{
		/*
		 * If it is > 0, then VRAM Bank E is sued for the palette.
		 * The first thing done is that the bank is
		 * set to map the LCD.
		 */
		vramSetBankE(VRAM_E_LCD);
		/*
		 * Then, the palette is copied to the bank's
		 * extended palette at the indicated slot.
		 */
		if(useGrayscale[screen][index])
		{
			memcpy((void*)(0x6880000 + (index << 13)), grayscalePaletteData[screen][index], 512);
		}
		else
		{
			memcpy((void*)(0x6880000 + (index << 13)), paletteData[screen][index], 512);
		}
		/*
		 * Finally, the bank is set to map its extended
		 * palette slot.
		 */
		vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	}
}

/*
 * Sets the desired backgrounds palette.
 * @param screen The screen that the background is on.
 * @param index The index (layer) of the background.
 * @param colorIndex The color index to change.
 * @param color The color to switch to.
 */
void setBgPaletteColor(int screen, int index, int colorIndex, color_t color)
{
	/*
	 * Updates the color at the desired color index.
	*/
	paletteData[screen][index][colorIndex] = RGB15(color.r >> 3, color.g >> 3, color.b >> 3);

	setBgUseGrayscale(screen, index, useGrayscale[screen][index]);
}

/*
 * Sets the desired background's parallax value with the
 * desired value.
 * @param screen The screen to set the parallax speeds on.
 * @param index The index (layer) to set the parallax speeds on.
 * @param x The x speed to set the background's parallax value to.
 * @param y The y speed to set the background's parallax value to.
 */
void setBgParallaxSpeeds(int screen, int index, int x, int y)
{
	/*
	 * Sets the background's parallax speed on the X axis
	 * to 1.
	*/
	bgParallaxXSpeeds[screen][index] = x;
	/*
	 * Sets the background's parallax speed on the Y axis
	 * to 1.
	*/
	bgParallaxYSpeeds[screen][index] = y;
}

/*
 * Get's the desired background's X parallax value.
 * @param screen The screen to get the parallax value from.
 * @param index The index (layer) to get the parallax value from.
 * @return Returns the parallax X speed.
 */
int getBgParallaxSpeedsX(int screen, int index)
{
	return bgParallaxXSpeeds[screen][index];
}

/*
 * Get's the desired background's Y parallax value.
 * @param screen The screen to get the parallax value from.
 * @param index The index (layer) to get the parallax value from.
 * @return Returns the parallax Y speed.
 */
int getBgParallaxSpeedsY(int screen, int index)
{
	return bgParallaxYSpeeds[screen][index];
}

/*
 * Get's the desired background's size.
 * @param screen The screen to get the size from.
 * @param index The index (layer) to get the size from.
 * @return Returns the background's boundary size.
 */
boundarySize_t getBgSize(int screen, int index)
{
	return bgSizes[screen][index]; 
}

/*
 * Updates the background system.
*/
void updateBackgrounds()
{
	/*
	 * Updates the background so that the scrolling is updated.
	*/
	bgUpdate();
}


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
void setBgCollisionMap(int screen, int index, u32 width, u32 height, const unsigned short* colMap, u32 colMapLen, const unsigned int* colTiles, u32 colTilesLen, const unsigned short* colPal)
{
	/*
	 * Set the width of the collision map.
	*/
	colBgSizes[screen][index].width = width;
	/*
	 * Set the height of the collision map.
	*/
	colBgSizes[screen][index].height = height;

	if(colMapData[screen][index] != NULL)
	{
		free(colMapData[screen][index]);
		colMapData[screen][index] = NULL;
	}
	/*
	 * Allocate the appropriate amount of memory for the tiles.
	 */
	colMapData[screen][index] = calloc(colMapLen, sizeof(unsigned short));
	/*
	 * This copies the tiles to the desired background slot's
	 * collision tile data pointer.
	 */
	memcpy(colMapData[screen][index], colMap, colMapLen);

	if(colTileData[screen][index] != NULL)
	{
		free(colTileData[screen][index]);
		colTileData[screen][index] = NULL;
	}
	/*
	 * Allocate the appropriate amount of memory for the tiles.
	 */
	colTileData[screen][index] = calloc(colTilesLen, sizeof(tile_t));
	/*
	 * This copies the tiles to the desired background slot's
	 * collision tile data pointer.
	 */
	memcpy(colTileData[screen][index], colTiles, colTilesLen);

	if(colPaletteData[screen][index] != NULL)
	{
		free(colPaletteData[screen][index]);
		colPaletteData[screen][index] = NULL;
	}
	/*
	 * Allocate the appropriate amount of memory for the tiles.
	 */
	colPaletteData[screen][index] = calloc(512, sizeof(unsigned short));
	/*
	 * This copies the tiles to the desired background slot's
	 * collision tile data pointer.
	 */
	memcpy(colPaletteData[screen][index], colPal, 512);
}

/*
 * Deletes the collision map data for the desired background.
 * @param screen The screen to delete the background collision map on.
 * @param index The index (layer) to delete the background collision map on.
*/
void deleteBgCollisionMap(int screen, int index)
{
	if(colMapData[screen][index] != NULL)
	{
		free(colMapData[screen][index]);
		colMapData[screen][index] = NULL;
	}
	if(colTileData[screen][index] != NULL)
	{
		free(colTileData[screen][index]);
		colTileData[screen][index] = NULL;
	}
	if(colPaletteData[screen][index] != NULL)
	{
		free(colPaletteData[screen][index]);
		colPaletteData[screen][index] = NULL;
	}
}

/*
 * Gets the tile index at a specific position on a collision background.
 * @param screen The screen to use.
 * @param index The index (layer) to use.
 * @param x The X position to use.
 * @param y The Y position to use.
 * @return Returns the index of the tile, otherwise it returns 0.
*/
unsigned int getTileIndex(int screen, int index, int x, int y)
{
	if(x < 0 || x > colBgSizes[screen][index].width || y < 0 || y > colBgSizes[screen][index].height)
	{
		return 0;
	}
	int blockX = x >> 8;
	int blockY = y >> 8;
	x -= blockX << 8;
	y -= blockY << 8;
	return colMapData[screen][index][((x >> 3) + ((y >> 3) * 32)) + ((blockX + (blockY * (colBgSizes[screen][index].width >> 8))) * 1024)];
}

/*
 * Gets the palette index at a specific position on a collision background.
 * @param screen The screen to use.
 * @param index The index (layer) to use.
 * @param x The X position to use.
 * @param y The Y position to use.
 * @return Returns the index of the palette at the position x,y, otherwise
 * it returns 0.
*/
unsigned int getPalIndex(int screen, int index, int x, int y)
{
	if(x < 0 || x > colBgSizes[screen][index].width || y < 0 || y > colBgSizes[screen][index].height)
	{
		return 0;
	}
	return ((unsigned char*)colTileData[screen][index])[getTileIndex(screen, index, x, y) * 64 + ((x - ((x >> 3) << 3)) + ((y - ((y >> 3) << 3)) * 8))];
}

/*
 * Gets the pixel color at a specific position on a collision background.
 * @param screen The screen to use.
 * @param index The index (layer) to use.
 * @param x The X position to use.
 * @param y The Y position to use.
 * @return Returns the pixel color at the position (x, y) if there is one, otherwise
 * it returns 0.
*/
unsigned int getPixelColor(int screen, int index, int x, int y)
{
	if(x < 0 || x > colBgSizes[screen][index].width || y < 0 || y > colBgSizes[screen][index].height)
	{
		return 0;
	}
	return colPaletteData[screen][index][getPalIndex(screen, index, x, y)];
}

