/*
 * A sprite manager file.  Using this, a developer can create sprites!  There can
 * be a max of 128 per screen and 16 palettes total!
 * Created by: Gerald McAlister
 */

#ifndef _SPRITE_CLASS_H_
#define _SPRITE_CLASS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include "generic.h"
#include "textFunctions.h"

/*
 *  This is the max amount of sprites per screen.
 */
#define MAX_SPRITES 128

/*
 * This is the max amount of palettes allowed for sprites.
 */
#define MAX_PALETTES 16

/*
 * Creates a sprite on the screen.
 * @param screen The screen to create the sprite on.
 * @param index The index of the sprite.
 * @param palSlot The slot for the palette data.
 * @param gfxData The graphical data.
 * @param palData The palette data.
 * @param width The width of the sprite.
 * @param height The height of the sprite.
 */
extern void createSprite(int screen, int index, int palSlot, const unsigned int* gfxData,
		u32 gfxDataSize, const unsigned short* palData, int width, int height);

/*
 * Creates a sprite on the screen.
 * @param screen The screen to create the sprite on.
 * @param index The index of the sprite.
 * @param palSlot The slot for the palette data.
 * @param screen2 The screen to copy the sprite from.
 * @param index2 The index of the sprite to copy from.
 */
void copySprite(int screen, int index, int palSlot, int screen2, int index2);

/*
 * Deletes the desired sprite's palette at the given index and screen.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
extern void deleteSpritePalette(int screen, int index);

/*
 * Deletes the desired sprite at the given index and screen.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
void deleteSprite(int screen, int index);

/*
 * Gets the desired sprite's bounding box.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns the sprite's bounding box.
 */
extern rectangle_t getBoundingBox(int screen, int index);

/*
 * Gets the desired sprite's source box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns the sprite's source box.
 */
extern rectangle_t getSourceBox(int screen, int index);

/*
 * Sets the desired sprite's source box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param rect The new source box.
 */
extern void setSourceBox(int screen, int index, rectangle_t rect);

/*
 * Gets the desired sprite's collision box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns the sprite's collision box.
 */
extern rectangle_t getCollisionBox(int screen, int index);

/*
 * Sets the desired sprite's collision box
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param rect The new collision box.
 */
extern void setCollisionBox(int screen, int index, rectangle_t rect);

/*
 * Gets the desired sprite's angle of rotation.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
extern int getAngle(int screen, int index);
/*
 * Gets the desired sprite's horizontal flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
extern bool getHFlip(int screen, int index);
/*
 * Gets the desired sprite's vertical flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 */
extern bool getVFlip(int screen, int index);

/*
 * Sets the desired sprite's X position.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to use.
 */
extern void setSpriteX(int screen, int index, int x);
/*
 * Sets the desired sprite's Y position.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param y The y position to use.
 */
extern void setSpriteY(int screen, int index, int y);
/*
 * Sets the desired sprite's X and Y position.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to use.
 * @param y The y position to use.
 */
extern void setSpriteXY(int screen, int index, int x, int y);

/*
 * Sets the desired sprite's layer.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param layer The layer to put the sprite on.
 */
extern void setSpriteLayer(int screen, int index, int layer);

/*
 * Sets the desired sprite's frame.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param frame The frame to use.
 */
extern void setSpriteFrame(int screen, int index, int frame);
/*
 * Sets the desired sprite's angle of rotation.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param angle The angle to use.
 */
extern void setSpriteAngle(int screen, int index, int rotationIndex, int angle);
/*
 * Sets the desired sprite's horizontal flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param hFlip The flip value to use.
 */
extern void setSpriteHFlip(int screen, int index, bool hFlip);
/*
 * Sets the desired sprite's vertical flip value.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param vFlip The flip value to use.
 */
extern void setSpriteVFlip(int screen, int index, bool vFlip);
/*
 * Sets the desired sprite's visibility.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param visible Whether the sprite is visible or not.
 */
extern void setSpriteVisible(int screen, int index, bool visible);

/*
 * Gets whether a sprite is grayscaled or not.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @return Returns true if the sprite is using
 * grayscale.  False otherwise.
 */
extern bool getSpriteUseGrayscale(int screen, int index);
/*
 * Sets whether a sprite is grayscaled or not.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param use Tells whether to use grayscale or not.
 */
extern void setSpriteUseGrayscale(int screen, int index, bool use);

/*
 * Checks if the sprite is touching a specific point.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to check.
 * @param y The y position to check.
 */
extern bool isSpriteTouchingPoint(int screen, int index, int x, int y);
/*
 * Checks if the sprite if within a circle.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The circle's x centerposition.
 * @param y The circle's y center position.
 * @param radius The radius of the circle.
 */
extern bool isSpriteTouchingCircle(int screen, int index, int x, int y,
		int radius);

/*
 * Checks if the sprite if within another sprite with pixel collisions.
 * @param screen The screen the sprite is on.
 * @param index1 The index of the first sprite.
 * @param index2 The index of the second sprite.
 * @return Returns true of touching, false otherwise.
 */
extern bool isSpriteTouchingSpritePerfect(int screen, int index1, int index2);

/*
 * Checks if the sprite if within another sprite with pixel collisions
 and returns the point of collision.
 * @param screen The screen the sprite is on.
 * @param index1 The index of the first sprite.
 * @param index2 The index of the second sprite.
 * @return Returns the point of intersection (-1, -1 if no point)
 */
extern coordinates_t getSpriteTouchingSpritePerfect(int screen, int index1, int index2);

/*
 * Checks if the sprite if within a circle.
 * @param screen The screen the sprite is on.
 * @param index1 The index of the first sprite.
 * @param index2 The index of the second sprite.
 * @return Returns true of touching, false otherwise.
 */
extern bool isSpriteTouchingSprite(int screen, int index1, int index2);

/*
 * Checks if the sprite if within another sprite at a basic level.
 * @param screen The screen the sprite is on.
 * @param index1 The index of the first sprite.
 * @param index2 The index of the second sprite.
 * @return Returns true of touching, false otherwise.
 */
extern enum sides_t isSpriteTouchingSpriteSide(int screen, int index1, int index2);

/*
 * Gets the pixel color at the desired position on the sprite.
 * @param screen The screen the sprite is on.
 * @param index The index of the sprite.
 * @param x The x position to use.
 * @param y The y position to use.
 */
extern color_t getSpritePixel(int screen, int index, int x, int y);

/*
 * Updates all of the sprites.
 */
extern void updateSprites();

#ifdef __cplusplus
}
#endif

#endif
