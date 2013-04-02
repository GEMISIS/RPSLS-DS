/*
 * A sprite manager file.  Using this, a developer can create sprites!  There can
 * be a max of 128 per screen and 16 palettes total!
 * Created by: Gerald McAlister
 */
#include "sprites.h"

/*
 *Gets the size of the sprite if the sizes are equal.  Used for the GET_SIZE define.
 */
#define GET_SIZE_TYPE_EQUAL(s) ((s == 8) ? 0 : (s == 16) ? 1 : (s == 32) ? 2 : 3)
/*
 *Gets the size of the sprite if the sizes are not equal.  Used for the GET_SIZE define.
 */
#define GET_SIZE_TYPE_NEQUAL(w, h) ((w == 16) ? 0 : (w == 32 && h == 8) ? 1 : (w == 32 && h == 16) ? 2 : 3)

/*
 *Gets the size of the bigger sprite.  Used for the GET_SIZE define.
 */
#define GET_BIGGER_SIZE(w, h) ((w > h) ? w : h)

/*
 *Gets the shape of the sprite.  Used for the GET_SIZE define.
 */
#define GET_SHAPE(w, h) (((w == h) * OBJSHAPE_SQUARE) + ((w > h) * OBJSHAPE_WIDE) + ((w < h) * OBJSHAPE_TALL))

/*
 *Gets the size of the sprite.  Used for the GET_SIZE define.
 */
#define GET_SIZE(w, h) ((GET_SHAPE(w, h) == 0) ? GET_SIZE_TYPE_EQUAL(GET_BIGGER_SIZE(w, h)) : GET_SIZE_TYPE_NEQUAL(w, h))

/*
 *Gets the size of the sprite.  This is used for creating sprites.
 */
#define SPRITE_PIXELS_SIZE(w, h) ((GET_SIZE(w, h) << 14) | (GET_SHAPE(w, h) << 12) | ((w*h) >> 5))

/*
 * This is a structure for creating sprites.
 * Contains things necessary for creating
 * and maintaining sprites.
 */
typedef struct _sprite_t_
{
	/*
	 * The frames per second
	 * for the animations.
	 */
	int fps;
	/*
	 * The layer that the
	 * sprite is on.
	 */
	int layer;
	/*
	 * The angle of the sprite's rotation.
	 */
	int angle;
	/*
	 * The rotation slot index for the sprite.
	 */
	int rotationIndex;
	/*
	 * The sprite's current frame.
	 */
	int currentFrame;
	/*
	 * The slot that the palette data
	 * is in.
	 */
	int paletteSlot;

	/*
	 * Tells whether the sprite is flipped
	 * vertically.
	 */
	bool vFlip;
	/*
	 * Tells whether the sprite is flipped
	 * horizontally.
	 */
	bool hFlip;
	/*
	 * Tells whether the sprite is visible.
	 */
	bool visible;
	/*
	 * Tells whether the sprite is active or not.
	 */
	bool active;
	/*
	 * Tells whether the sprite is a copy of another sprite
	 * or not.
	 */
	bool isCopy;

	/*
	 * The graphics memory for where the
	 * sprite's graphics data is located.
	 */
	u16* gfxMemory;
	/*
	 * The current frame's data.  Holds the
	 * graphical data for the current frame.
	 */
	u16* frameData;
	/*
	 * The graphic's data.  Holds the
	 * graphical data for the sprite.
	 */
	u16* gfxData;
	/*
	 * The palette memory for where the
	 * sprite's palette data is located.
	 */
	u16* paletteData;
	/*
	 * The palette memory for where the
	 * sprite's grayscale palette data is located.
	 */
	u16* grayscalePaletteData;
	/*
	 * Tells whether the sprite is grayscale
	 * colored or not.
	 */
	bool useGrayscale;
	/*
	 * The bounding rectangle for the
	 * sprite.
	 */
	rectangle_t bRect;
	/*
	 * The bounding rectangle for the
	 * sprite's source image.
	 */
	rectangle_t sRect;
	/*
	 * The bounding rectangle for the
	 * sprite's collisision.
	 */
	rectangle_t cRect;
} sprite_t;

sprite_t spriteList[2][MAX_SPRITES];

/*
 * Loads the sprite with on desired screen
 * and given index's data.
 * @param screen The screen to load the sprite data on.
 * @param index The index of the sprite to load the data into.
 * @param gfx True to load the graphics data.
 * @param pal True to load the palette data.
 */
void loadData(int screen, int index, bool gfx, bool pal)
{
	/*
	 * Checks to see if the screen is equal to 0.
	 */
	if (screen == 0)
	{
		/*
		 * Then it checks to see if it should load the palette
		 * data.
		 */
		if (pal)
		{
			/*
			 * If it should, then since this is the bottom screen, and since
			 * extended palettes are being used, then VRAM Bank I is set to
			 * map the LCD while the data is written to it.
			 */
			vramSetBankI(VRAM_I_LCD);
			/*
			 * This is where the data is copied to the screen. Palettes
			 * have a length of 512, and the data is written to VRAM Bank
			 * I's extended palette area, at the sprite's palette index.
			 */
			if(spriteList[screen][index].useGrayscale)
			{
				memcpy(VRAM_I_EXT_SPR_PALETTE[spriteList[screen][index].paletteSlot], spriteList[screen][index].grayscalePaletteData, 512);
			}
			else
			{
				memcpy(VRAM_I_EXT_SPR_PALETTE[spriteList[screen][index].paletteSlot], spriteList[screen][index].paletteData, 512);
			}
			/*
			 * Finally, VRAM Bank I is set to map the extended palette
			 * slot instead, so that it will be ready to use.
			 */
			vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
		}
		/*
		 * Then, it checks to see if it should allocate memory for the
		 * sprite's graphics.
		 */
		if (gfx)
		{
			/*
			 * Frees the sprite's graphical data in the memory.
			 */
			if(spriteList[screen][index].gfxMemory)
			{
				oamFreeGfx(&oamSub, spriteList[screen][index].gfxMemory);
				spriteList[screen][index].gfxMemory = NULL;
			}
		
			/*
			 * If it should, then it does so, using 256 Colors, and using the
			 * SPRITE_PIXELS_SIZE define to get the size of the sprite as according
			 * to the oamAllocateGfx specifications.
			 */
			spriteList[screen][index].gfxMemory
					= oamAllocateGfx(
							&oamSub,
							(SpriteSize) SPRITE_PIXELS_SIZE(spriteList[screen][index].bRect.size.width, spriteList[screen][index].bRect.size.height),
							SpriteColorFormat_256Color);
		}
		/*
		 * Then, we return out so that it doesn't check for the other screen since
		 * the data has already been created.
		 */
		return;
	}

	/*
	 * Then, it checks to see if the sprite is on screen 1.
	 */
	if (screen == 1)
	{
		/*
		 * After that, the palette boolean variable is checked to see
		 * if it needs to load the palette data.
		 */
		if (pal)
		{
			/*
			 * If so, then VRAM Bank G is set to map the LCD for the top screen.
			 */
			vramSetBankG(VRAM_G_LCD);
			/*
			 * Then, the data is copied with a length of 512 to VRAM Bank F's
			 * extended palette slot at the sprite's palette index.
			 */
			if(spriteList[screen][index].useGrayscale)
			{
				memcpy(VRAM_G_EXT_SPR_PALETTE[spriteList[screen][index].paletteSlot], spriteList[screen][index].grayscalePaletteData, 512);
			}
			else
			{
				memcpy(VRAM_G_EXT_SPR_PALETTE[spriteList[screen][index].paletteSlot], spriteList[screen][index].paletteData, 512);
			}
			/*
			 * Finally, VRAM Bank G is set to map the extended palette
			 * slot instead.
			 */
			vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);
		}
		/*
		 * Then, it checks to see if it should allocate for the
		 * graphics data.
		 */
		if (gfx)
		{
			/*
			 * Frees the sprite's graphical data in the memory.
			 */
			if(spriteList[screen][index].gfxMemory)
			{
				oamFreeGfx(&oamMain, spriteList[screen][index].gfxMemory);
				spriteList[screen][index].gfxMemory = NULL;
			}

			/*
			 * If so, then it will allocate memory for the graphics data, using
			 * 256 Colors, and getting the size using the SPRITE_PIXELS_SIZE define.
			 */
			spriteList[screen][index].gfxMemory
					= oamAllocateGfx(
							&oamMain,
							(SpriteSize) SPRITE_PIXELS_SIZE(spriteList[screen][index].bRect.size.width, spriteList[screen][index].bRect.size.height),
							SpriteColorFormat_256Color);
		}
	}
}

/*
 * Creates a sprite on the chosen screen.
 * @param screen The screen to create the sprite on.
 * @param index The index of the sprite.
 * @param palSlot The slot for the palette data.
 * @param gfxData The graphical data.
 * @param gfxDataSize The size of the graphical data.
 * @param palData The palette data.
 * @param width The width of the sprite.
 * @param height The height of the sprite.
 */
void createSprite(int screen, int index, int palSlot, const unsigned int* gfxData, u32 gfxDataSize, const unsigned short* palData,
		int width, int height)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	if(spriteList[screen][index].active)
	{
		return;
	}

	/*
	 * Sets the palette slot to the desired one.
	 */
	spriteList[screen][index].paletteSlot = palSlot;
	/*
	 * Sets the frames per second to 1.
	 */
	spriteList[screen][index].fps = 1;
	/*
	 * Sets the sprite's angle to the chosen
	 * one.
	 */
	spriteList[screen][index].angle = -1;
	/*
	 * Makes the sprite visible.
	 */
	spriteList[screen][index].visible = true;

	/*
	 * Sets the bounding rectangle's X position.
	 */
	spriteList[screen][index].bRect.position.x = 0;
	/*
	 * Sets the bounding rectangle's Y position.
	 */
	spriteList[screen][index].bRect.position.y = 0;

	int i = 0;

	// Loop through the sprites widths, finding the correct max size for a sprite.
	// This is due to the fact that the sprite must have a width of 8, 16, 32, 64, or 128.
	for(i = 8;i <= 128;i *= 2)
	{
		if(i >= width)
		{
			spriteList[screen][index].bRect.size.width = i;
			break;
		}
	}
	// Loop through the sprites heights, finding the correct max size for a sprite.
	// This is due to the fact that the sprite must have a height of 8, 16, 32, 64, or 128.
	for(i = 8;i <= 128;i *= 2)
	{
		if(i >= height)
		{
			spriteList[screen][index].bRect.size.height = i;
			break;
		}
	}

	/*
	 * Sets the bounding rectangle's width.
	 */
	spriteList[screen][index].sRect.size.width = width;
	spriteList[screen][index].cRect.size.width = width;
	/*
	 * Sets the bounding rectangle's height.
	 */
	spriteList[screen][index].sRect.size.height = height;
	spriteList[screen][index].cRect.size.height = height;

	/*
	 * Sets the sprite's graphics memory.
	 */
	if(spriteList[screen][index].gfxData != NULL)
	{
		free(spriteList[screen][index].gfxData);
		spriteList[screen][index].gfxData = NULL;
	}
	spriteList[screen][index].gfxData = (u16*)calloc(gfxDataSize, sizeof(u16));
	memcpy(spriteList[screen][index].gfxData, gfxData, gfxDataSize);

	/*
	 * Sets the sprite's palette memory.
	 */
	if(spriteList[screen][index].paletteData != NULL)
	{
		free(spriteList[screen][index].paletteData);
		spriteList[screen][index].paletteData = NULL;
	}
	spriteList[screen][index].paletteData = (u16*)calloc(512, sizeof(u16));
	memcpy(spriteList[screen][index].paletteData, palData, 512);

	/*
	 * Sets the sprite to active.
	 */
	spriteList[screen][index].active = true;

	/*
	 * Make sure to tell that this sprite is not a copy.
	 */
	spriteList[screen][index].isCopy = false;

	/*
	 * Load the sprite's palette and graphical memory.
	 */
	loadData(screen, index, true, true);

	/*
	 * Load the grayscale usage info.
	 */
	setSpriteUseGrayscale(screen, index, spriteList[screen][index].useGrayscale);
}

/*
 * Creates a sprite on the screen.
 * @param screen The screen to create the sprite on.
 * @param index The index of the sprite.
 * @param palSlot The slot for the palette data.
 * @param screen2 The screen to copy the sprite from.
 * @param index2 The index of the sprite to copy from.
 */
void copySprite(int screen, int index, int palSlot, int screen2, int index2)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Checks to see if the index is too small.
	 */
	if (index2 <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index2 = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index2 >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index2 = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen2 <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen2 = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen2 = 1;
	}

	/*
	 * Check if the sprite is already active, if so, return from this method.
	 */
	if(spriteList[screen][index].active)
	{
		return;
	}

	/*
	 * Sets the palette slot to the desired one.
	 */
	spriteList[screen][index].paletteSlot = palSlot;
	/*
	 * Sets the frames per second to 1.
	 */
	spriteList[screen][index].fps = 1;
	/*
	 * Sets the sprite's angle to the chosen
	 * one.
	 */
	spriteList[screen][index].angle = -1;
	/*
	 * Makes the sprite visible.
	 */
	spriteList[screen][index].visible = true;

	/*
	 * Sets the bounding rectangle's X position.
	 */
	spriteList[screen][index].bRect.position.x = 0;
	/*
	 * Sets the bounding rectangle's Y position.
	 */
	spriteList[screen][index].bRect.position.y = 0;

	// Set the sprite's sizes to be the same too.
	spriteList[screen][index].bRect.size.width = spriteList[screen2][index2].bRect.size.width;
	spriteList[screen][index].bRect.size.height = spriteList[screen2][index2].bRect.size.height;

	/*
	 * Sets the bounding rectangle's width.
	 */
	spriteList[screen][index].sRect.size.width = spriteList[screen2][index2].sRect.size.width;
	spriteList[screen][index].cRect.size.width = spriteList[screen2][index2].cRect.size.width;
	/*
	 * Sets the bounding rectangle's height.
	 */
	spriteList[screen][index].sRect.size.height = spriteList[screen2][index2].sRect.size.height;
	spriteList[screen][index].cRect.size.height = spriteList[screen2][index2].cRect.size.height;

	/*
	 * Sets the sprite's graphics memory.
	 * No need to copy the memory since they are duplicates and can
	 * share the pointer to the memory.
	 */
	spriteList[screen][index].gfxData = spriteList[screen2][index2].gfxData;

	/*
	 * Sets the sprite's palette memory.
	 * No need to copy the memory since they are duplicates and can
	 * share the pointer to the memory.
	 */
	spriteList[screen][index].paletteData = spriteList[screen2][index2].paletteData;

	/*
	 * Sets the sprite to active.
	 */
	spriteList[screen][index].active = true;

	/*
	 * Make sure to tell that this sprite is a copy.
	 */
	spriteList[screen][index].isCopy = true;

	/*
	 * Load the sprite's palette and graphical memory.
	 */
	loadData(screen, index, true, true);

	/*
	 * Load the grayscale usage info.
	 */
	setSpriteUseGrayscale(screen, index, spriteList[screen][index].useGrayscale);
}

/*
 * Deletes the desired sprite's palette at the given index and screen.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
void deleteSpritePalette(int screen, int index)
{
	/*
	 * Check if the sprite is a copy of another sprite.
	 */
	if(spriteList[screen][index].isCopy)
	{
		/*
		 * If so, just set the pointer to NULL.
		 */
		spriteList[screen][index].paletteData = NULL;
	}
	else
	{
		/*
		 * Otherwise, frees the sprite's palette data.
		 */
		if(spriteList[screen][index].paletteData)
		{
			free(spriteList[screen][index].paletteData);
			spriteList[screen][index].paletteData = NULL;
		}
	}
}

/*
 * Deletes the desired sprite at the given index and screen.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
void deleteSprite(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	if(!spriteList[screen][index].active)
	{
		return;
	}

	/*
	 * Sets the sprite to not active.
	 */
	spriteList[screen][index].active = false;

	/*
	 * Clears the sprite in the OAM data.
	 */
	oamClearSprite((screen == 0) ? &oamSub : &oamMain, index);

	/*
	 * Check if the sprite is a copy of another sprite.
	 */
	if(spriteList[screen][index].isCopy)
	{
		/*
		 * If so, just set the pointers to NULL.
		 */
		spriteList[screen][index].gfxData = NULL;
		spriteList[screen][index].gfxMemory = NULL;
	}
	else
	{
		/*
		 * Otherwise, frees the sprite's graphical data.
		 */
		if(spriteList[screen][index].gfxData)
		{
			free(spriteList[screen][index].gfxData);
			spriteList[screen][index].gfxData = NULL;
		}

		/*
		 * Also frees the sprite's graphical data in the memory.
		 */
		if(spriteList[screen][index].gfxMemory)
		{
			oamFreeGfx((screen <= 0) ? &oamSub : &oamMain, spriteList[screen][index].gfxMemory);
			spriteList[screen][index].gfxMemory = NULL;
		}
	}
}

/*
 * Gets the desired sprite's bounding box.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns the sprite's bounding box.
 */
rectangle_t getBoundingBox(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}
	/*
	 * Returns the sprite's bounding rectangle.
	 */
	return spriteList[screen][index].bRect;
}

/*
 * Gets the desired sprite's source box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns the sprite's source box.
 */
rectangle_t getSourceBox(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}
	/*
	 * Returns the sprite's bounding rectangle.
	 */
	return spriteList[screen][index].sRect;
}

/*
 * Sets the desired sprite's source box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param rect The new source box.
 */
void setSourceBox(int screen, int index, rectangle_t rect)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}
	/*
	 * Sets the sprite's source rectangle.
	 */
	spriteList[screen][index].sRect = rect;
}

/*
 * Gets the desired sprite's collision box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns the sprite's collision box.
 */
rectangle_t getCollisionBox(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}
	/*
	 * Returns the sprite's bounding rectangle.
	 */
	return spriteList[screen][index].cRect;
}

/*
 * Sets the desired sprite's collision box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param rect The new collision box.
 */
void setCollisionBox(int screen, int index, rectangle_t rect)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}
	/*
	 * Sets the sprite's source rectangle.
	 */
	spriteList[screen][index].cRect = rect;
}

/*
 * Gets the desired sprite's angle of rotation.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns the sprite's angle of rotation.
 */
int getAngle(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Returns the sprite's angle of rotation.
	 */
	return spriteList[screen][index].angle;
}

/*
 * Gets the desired sprite's horizontal flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns true if flipped horizontally, false otherwise.
 */
bool getHFlip(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Returns the sprite's horizontal flip value.
	 */
	return spriteList[screen][index].hFlip;
}

/*
 * Gets the desired sprite's vertical flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns true if flipped vertically, false otherwise.
 */
bool getVFlip(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Returns the sprite's vertical flip value.
	 */
	return spriteList[screen][index].vFlip;
}

/*
 * Sets the desired sprite's X position.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to use.
 */
void setSpriteX(int screen, int index, int x)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's X position.
	 */
	spriteList[screen][index].bRect.position.x = x;
}

/*
 * Sets the desired sprite's Y position.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param y The y position to use.
 */
void setSpriteY(int screen, int index, int y)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's Y position.
	 */
	spriteList[screen][index].bRect.position.y = y;
}

/*
 * Sets the desired sprite's X and Y position.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to use.
 * @param y The y position to use.
 */
void setSpriteXY(int screen, int index, int x, int y)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's X position.
	 */
	setSpriteX(screen, index, x);
	/*
	 * Sets the sprite's Y position.
	 */
	setSpriteY(screen, index, y);
}

/*
 * Sets the desired sprite's layer.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param layer The layer to put the sprite on.
 */
void setSpriteLayer(int screen, int index, int layer)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's layer.
	 */
	spriteList[screen][index].layer = layer;
}

/*
 * Sets the desired sprite's frame.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param frame The frame to use.
 */
void setSpriteFrame(int screen, int index, int frame)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	spriteList[screen][index].currentFrame = frame;
}

/*
 * Sets the desired sprite's angle of rotation.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param angle The angle to use.
 */
void setSpriteAngle(int screen, int index, int rotationIndex, int angle)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's rotation slot index.
	 */
	spriteList[screen][index].rotationIndex = rotationIndex;

	/*
	 * Sets the sprite's angle of rotation.
	 */
	spriteList[screen][index].angle = angle;
}

/*
 * Sets the desired sprite's horizontal flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param hFlip The flip value to use.
 */
void setSpriteHFlip(int screen, int index, bool hFlip)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's horizontal flip value.
	 */
	spriteList[screen][index].hFlip = hFlip;
}

/*
 * Sets the desired sprite's vertical flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param vFlip The flip value to use.
 */
void setSpriteVFlip(int screen, int index, bool vFlip)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's vertical flip.
	 */
	spriteList[screen][index].vFlip = vFlip;
}

/*
 * Sets the desired sprite's visibility.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param visible Whether the sprite is visible or not.
 */
void setSpriteVisible(int screen, int index, bool visible)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Sets the sprite's visibility.
	 */
	spriteList[screen][index].visible = visible;
}

/*
 * Gets whether a sprite is grayscaled or not.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns true if the sprite is using
 * grayscale, false otherwise.
 */
bool getSpriteUseGrayscale(int screen, int index)
{
	return spriteList[screen][index].useGrayscale;
}

/*
 * Sets whether a sprite is grayscaled or not.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param use Tells whether to use grayscale or not.
 */
void setSpriteUseGrayscale(int screen, int index, bool use)
{
	if(spriteList[screen][index].grayscalePaletteData != NULL)
	{
		free(spriteList[screen][index].grayscalePaletteData);
		spriteList[screen][index].grayscalePaletteData = NULL;
	}
	spriteList[screen][index].grayscalePaletteData = calloc(512, sizeof(unsigned short));

	uint32_t i = 0;
	for(i = 0;i < 512;i += 1)
	{
		u16 color_hex = ((u16*)spriteList[screen][index].paletteData)[i];

		color_t color;
		color.r = (((color_hex & 0x1F) + ((color_hex & 0x1F) & 0x1)) << 3) - ((color_hex & 0x1F) & 0x1);
		color.g = ((((color_hex & 0x3E0) >> 5) + (((color_hex & 0x3E0) >> 5) & 0x1)) << 3) - (((color_hex & 0x3E0) >> 5) & 0x1);
		color.b = ((((color_hex & 0x7C00) >> 10) + (((color_hex & 0x7C00) >> 10) & 0x1)) << 3) - (((color_hex & 0x7C00) >> 10) & 0x1);

		u16 color_value = (((u16)((((double)color.r) * 0.3) + (((double)color.g) * 0.59) + (((double)color.b) * 0.11))) >> 3);

		spriteList[screen][index].grayscalePaletteData[i] = RGB15(color_value, color_value, color_value);
	}

	spriteList[screen][index].useGrayscale = use;

	loadData(screen, index, false, true);
}

/*
 * Checks if the sprite is touching a specific point.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to check.
 * @param y The y position to check.
 * @return Returns true if the sprite is touching the point, 
 * false otherwise.
 */
bool isSpriteTouchingPoint(int screen, int index, int x, int y)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Returns whether the sprite is active, and if the point
	 * is touching the sprite.
	 */
	return spriteList[screen][index].active && (x
			> spriteList[screen][index].bRect.position.x && x
			< spriteList[screen][index].bRect.position.x
					+ spriteList[screen][index].bRect.size.width) && (y
			> spriteList[screen][index].bRect.position.y && y
			< spriteList[screen][index].bRect.position.y
					+ spriteList[screen][index].bRect.size.height);
}

/*
 * Checks if the sprite if within a circle.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The circle's x centerposition.
 * @param y The circle's y center position.
 * @param radius The radius of the circle.
 * @return Returns true the sprite is touching the circle,
 * false otherwise.
 */
bool isSpriteTouchingCircle(int screen, int index, int x, int y, int radius)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Checks if the sprite is within the radius of the circle.
	 */
	return (((x > spriteList[screen][index].bRect.position.x && x
			< spriteList[screen][index].bRect.position.x
					+ spriteList[screen][index].bRect.size.width) && (y
			> spriteList[screen][index].bRect.position.y && y
			< spriteList[screen][index].bRect.position.y
					+ spriteList[screen][index].bRect.size.height)) || ((x
			+ radius > spriteList[screen][index].bRect.position.x && x + radius
			< spriteList[screen][index].bRect.position.x
					+ spriteList[screen][index].bRect.size.width) && (y
			> spriteList[screen][index].bRect.position.y && y
			< spriteList[screen][index].bRect.position.y
					+ spriteList[screen][index].bRect.size.height)) || ((x
			> spriteList[screen][index].bRect.position.x && x
			< spriteList[screen][index].bRect.position.x
					+ spriteList[screen][index].bRect.size.width) && (y
			+ radius > spriteList[screen][index].bRect.position.y && y + radius
			< spriteList[screen][index].bRect.position.y
					+ spriteList[screen][index].bRect.size.height)) || ((x
			+ radius > spriteList[screen][index].bRect.position.x && x + radius
			< spriteList[screen][index].bRect.position.x
					+ spriteList[screen][index].bRect.size.width) && (y
			+ radius > spriteList[screen][index].bRect.position.y && y + radius
			< spriteList[screen][index].bRect.position.y
					+ spriteList[screen][index].bRect.size.height)))
			&& spriteList[screen][index].active;
}

/*
 * Gets the pixel color at the desired position on the sprite.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to use.
 * @param y The y position to use.
 * @return Returns the color structure with the pixel's color.
 */
color_t getSpritePixel(int screen, int index, int x, int y)
{
	if(x < 0 || x > spriteList[screen][index].sRect.size.width || y < 0 || y > spriteList[screen][index].sRect.size.height)
	{
		color_t t;
		t.r = -1;
		t.g = -1;
		t.b = -1;
		return t;
	}
	if(spriteList[screen][index].angle > -1)
	{
		x = ((x << 12) * cosLerp(degreesToAngle(spriteList[screen][index].angle))) >> 12;
		y = ((y << 12) * sinLerp(degreesToAngle(spriteList[screen][index].angle))) >> 12;
	}
	int32_t xBlock = x >> 3;
	int32_t yBlock = y >> 3;
	int32_t tilePosition = (x - (xBlock << 3)) + (xBlock << 6) + ((y - (yBlock << 3)) + (yBlock << 5)) * 8;

	u16 color_hex = ((u16*)spriteList[screen][index].paletteData)[((u8*)spriteList[screen][index].frameData)[tilePosition]];

	color_t color;
	color.r = (((color_hex & 0x1F) + ((color_hex & 0x1F) & 0x1)) << 3) - ((color_hex & 0x1F) & 0x1);
	color.g = ((((color_hex & 0x3E0) >> 5) + (((color_hex & 0x3E0) >> 5) & 0x1)) << 3) - (((color_hex & 0x3E0) >> 5) & 0x1);
	color.b = ((((color_hex & 0x7C00) >> 10) + (((color_hex & 0x7C00) >> 10) & 0x1)) << 3) - (((color_hex & 0x7C00) >> 10) & 0x1);

	return color;
}

/*
 * Draws the desired sprite at the chosen index on the desired screen.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
void drawSprite(int screen, int index)
{
	/*
	 * Checks to see if the index is too small.
	 */
	if (index <= 0)
	{
		/*
		 * If so, set it to 0.
		 */
		index = 0;
	}
	/*
	 * Checks to see if the sprite index is too big.
	 */
	if (index >= MAX_SPRITES)
	{
		/*
		 * If so, set it to 127.
		 */
		index = 127;
	}
	/*
	 * Checks to see if the screen variable is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then the screen is set to 0.
		 */
		screen = 0;
	}
	else
	{
		/*
		 * Otherwise, set the screen to 1.
		 */
		screen = 1;
	}

	/*
	 * Checks if the sprite is active.
	 */
	if (spriteList[screen][index].active)
	{
		/*
		 * Sets the sprite's frame data.
		 */
		if(spriteList[screen][index].frameData != NULL)
		{
			free(spriteList[screen][index].frameData);
			spriteList[screen][index].frameData = NULL;
		}
		spriteList[screen][index].frameData = (u16*)calloc(spriteList[screen][index].bRect.size.width * spriteList[screen][index].bRect.size.height, sizeof(u16));

		memcpy(spriteList[screen][index].frameData,
			spriteList[screen][index].gfxData + (spriteList[screen][index].currentFrame *
			(spriteList[screen][index].sRect.size.width * spriteList[screen][index].sRect.size.height / 2)),
			spriteList[screen][index].sRect.size.width * spriteList[screen][index].sRect.size.height);

		/*
		 * Copies the sprite's frame graphics to the
		 * graphical memory.
		 */
		memcpy(spriteList[screen][index].gfxMemory, spriteList[screen][index].frameData, 
			spriteList[screen][index].bRect.size.width * spriteList[screen][index].bRect.size.height);

		/*
		 * Checks if the rotation is not -1.
		 */
		if (spriteList[screen][index].angle != -1)
		{
			/*
			 * Then it sets the sprite's angle of rotation.
			 */
			oamRotateScale((screen == 0) ? &oamSub : &oamMain, spriteList[screen][index].rotationIndex,
					degreesToAngle(spriteList[screen][index].angle),
					intToFixed(1, 8), intToFixed(1, 8));
		}
		/*
		 * Sets the sprite's OAM info.
		 */
		oamSet(
				(screen == 0) ? &oamSub : &oamMain,
				index,
				(spriteList[screen][index].angle == -1) ? spriteList[screen][index].bRect.position.x : spriteList[screen][index].bRect.position.x - (spriteList[screen][index].bRect.size.width / 2),
				(spriteList[screen][index].angle == -1) ? spriteList[screen][index].bRect.position.y : spriteList[screen][index].bRect.position.y - (spriteList[screen][index].bRect.size.height / 2),
				spriteList[screen][index].layer,
				spriteList[screen][index].paletteSlot,
				(SpriteSize) SPRITE_PIXELS_SIZE(spriteList[screen][index].bRect.size.width, spriteList[screen][index].bRect.size.height),
				SpriteColorFormat_256Color,
				spriteList[screen][index].gfxMemory,
				(spriteList[screen][index].angle == -1) ? -1 : spriteList[screen][index].rotationIndex,
				(spriteList[screen][index].angle != -1),
				!spriteList[screen][index].visible,
				spriteList[screen][index].hFlip,
				spriteList[screen][index].vFlip, false);
	}
}

/*
 * Updates all of the sprites.
 */
void updateSprites()
{
	/*
	 * Creates a variable s for going through
	 * the screens.
	 */
	int s = 0;
	/*
	 * Creates a variable i for going through
	 * the sprites at the specified index.
	 */
	int i = 0;

	/*
	 * This part loops through the screens.
	 */
	for (s = 0; s < 2; s += 1)
	{
		/*
		 * This part loops through the sprites.
		 */
		for (i = 0; i < MAX_SPRITES; i += 1)
		{
			/*
			 * Checks if the sprite is active.
			 */
			if (spriteList[s][i].active)
			{
				/*
				 * If it is, then it is drawn.
				 */
				drawSprite(s, i);
			}
		}
	}
}
