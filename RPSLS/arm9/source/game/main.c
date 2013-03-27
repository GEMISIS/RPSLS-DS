// Main includes for the game.
#include <nds.h>
#include <maxmod9.h>
#include <filesystem.h>

// Inlcude the functions from the game library.
#include "GEM_functions.h"

// Include the background images.
#include "top_main.h"
#include "gameover_top.h"
#include "gameover_bottom.h"

// Include the bottom screen's sprites.
#include "rock.h"
#include "paper.h"
#include "scissors.h"
#include "lizard.h"
#include "spock.h"

// Include the top screen's sprites.
#include "healthbar.h"

// The total number of buttons to choose from.
#define TOTAL_BUTTONS 5

// A enumorator for the game mode.
// Has a single player and multiplayer mode.
// Multiplayer mode is not implemented in the game yet.
typedef enum
{
	SINGLE_NORMAL = 1,
	MULTI_NORMAL = 2
}playerMode;

// This table determines who wins the match.
// Prevents the need of giant switch statements.
const int resultsTable[5][5] = {
	{0, -1, 1, 1, -1},
	{1, 0, -1, -1, 1},
	{-1, 1, 0, 1, -1},
	{-1, 1, -1, 0, 1},
	{1, -1, 1, -1, 0}
};

// An array holding pointers to the graphics for each selection.
const unsigned int* selectionSprites[5] = {rockTiles, paperTiles, scissorsTiles, lizardTiles, spockTiles};
// An array holding pointers to the graphic's size for each selection.
const u32 selectionSpritesSizes[5] = {rockTilesLen, paperTilesLen, scissorsTilesLen, lizardTilesLen, spockTilesLen};
// An array holding pointers to the palette for each selection.
const unsigned short* selectionSpritesPal[5] = {rockPal, paperPal, scissorsPal, lizardPal, spockPal};

/*
 * Gets what button is pressed at a given X and Y position.
 * @param x The X coordinate to check.
 * @param y The Y coordinate to check.
 * @return Returns the index of the sprite button being touched.
 */
unsigned int buttonTouched(int x, int y)
{
	// Initialize i for the for loop.
	int i = 0;
	// Loop through all of the buttons.
	for(i = 0; i < TOTAL_BUTTONS;i += 1)
	{
		// Check if the x and y coordinates are touching
		// the current button sprite.
		if(isSpriteTouchingPoint(0, i, x, y))
		{
			// If so, update the sprite's animation frame.
			setSpriteFrame(0, i, 1);
			// Then update the entire game itself.
			updateAll();
			// Finally, return the index of the button's sprite.
			return i;
		}
		// Otherwise, set the sprite frame to the non touched frame.
		setSpriteFrame(0, i, 0);
	}
	// Return -1 if no button is being touched.
	return -1;
}

/*
 * Loads a background into the game with the given data.
 * @param screen The screen to load the background on to.
 * @param layer The layer to set the background to.
 * @param tiles The tiles for the background.
 * @param tilesLen The size of the tiles.
 * @param map The background's map data.
 * @param mapLen The size of the map data.
 * @param pal The backgrounds palette.
 */
void loadBg(int screen, int layer, const unsigned int* tiles, u32 tilesLen,
	const unsigned short* map, u32 mapLen, const unsigned short* pal)
{
	// Set the backgrounds palette data.
	setBgPalette(screen, layer, pal);
	// Set the backgrounds tiles.
	setBgTiles(screen, layer, tiles, tilesLen);
	// Map out the backgrounds tiles with the map data.
	setBgMap(screen, layer, map, mapLen);
}

/*
 * Chooses a winner for the match.
 * @param choice1 The first player's choice.
 * @param choice2 The second player's choice.
 * @return Returns the winner of the match from the resultsTable.
 */
int chooseWinner(int choice1, int choice2)
{
	return resultsTable[choice1][choice2];
}

/*
 * Animates the results of the match.
 * @param choice1 The first player's choice.
 * @param choice2 The second player's choice.
 */
void animateActions(int choice1, int choice2)
{
	// Create the first player's sprite based on the choice.
	createSprite(1, 0, 0, selectionSprites[choice1], selectionSpritesSizes[choice1], selectionSpritesPal[choice1], 64, 64);
	// Create the second player's sprite based on the choice.
	createSprite(1, 1, 1, selectionSprites[choice2], selectionSpritesSizes[choice2], selectionSpritesPal[choice2], 64, 64);

	int x1 = -64;
	int x2 = 256;
	int y = 128 - 32;
	int rotation = 0;

	setSpriteXY(1, 0, x1, y);
	setSpriteXY(1, 1, x2, y);

	int speed = 4;

	while(x1 + 64 != x2)
	{
		x1 += speed;
		x2 -= speed;

		setSpriteXY(1, 0, x1, y);
		setSpriteXY(1, 1, x2, y);

		updateAll();
	}

	int winner = chooseWinner(choice1, choice2);

	if(winner == 1)
	{
		while(x2 < 256 && y < 192)
		{
			x2 += 1;
			y += 1;
			if(rotation >= 360)
			{
				rotation = 0;
			}
			rotation -= 4;

			setSpriteXY(1, 1, x2, y);
			setSpriteAngle(1, 1, 0, rotation);

			updateAll();
		}
	}
	else if(winner == -1)
	{
		while(x1 > -64 && y < 192)
		{
			x1 -= 1;
			y += 1;
			if(rotation >= 360)
			{
				rotation = 0;
			}
			rotation += 4;

			setSpriteXY(1, 0, x1, y);
			setSpriteAngle(1, 0, 0, rotation);

			updateAll();
		}
	}
	else
	{
		while(x1 > -64 && x2 < 256 && y < 192)
		{
			x1 -= 1;
			x2 += 1;
			y += 1;
			if(rotation >= 360)
			{
				rotation = 0;
			}
			rotation += 4;

			setSpriteXY(1, 0, x1, y);
			setSpriteXY(1, 1, x2, y);
			setSpriteAngle(1, 0, 0, rotation);
			setSpriteAngle(1, 1, 1, -rotation);

			updateAll();
		}
	}

	deleteSprite(1, 0);
	deleteSprite(1, 1);
}

int decideMatch_basic(int choice)
{
	int cpuChoice = rand() % 5;

	animateActions(choice, cpuChoice);

	int winner = chooseWinner(choice, cpuChoice);

	return winner;
}

void initializeMainGameGraphics(playerMode mode)
{
	deleteBg(0, 1);
	deleteBg(1, 1);

	setBackdropColor(RGB15(1, 5, 9));
	setBackdropColorSub(RGB15(1, 5, 9));

	if(mode == SINGLE_NORMAL)
	{
		createSprite(0, 0, 0, rockTiles, rockTilesLen, rockPal, 64, 64);
		setSpriteXY(0, 0, 32 + (64 * 0), 32 + (64 * 0));

		createSprite(0, 1, 1, paperTiles, paperTilesLen, paperPal, 64, 64);
		setSpriteXY(0, 1, 32 + (64 * 1), 32 + (64 * 0));

		createSprite(0, 2, 2, scissorsTiles, scissorsTilesLen, scissorsPal, 64, 64);
		setSpriteXY(0, 2, 32 + (64 * 2), 32 + (64 * 0));

		createSprite(0, 3, 3, lizardTiles, lizardTilesLen, lizardPal, 64, 64);
		setSpriteXY(0, 3, 32 + 32 + (64 * 0), 32 + (64 * 1));

		createSprite(0, 4, 4, spockTiles, spockTilesLen, spockPal, 64, 64);
		setSpriteXY(0, 4, 32 + 32 + (64 * 1), 32 + (64 * 1));

		createSprite(1, 4, 4, healthbarTiles, healthbarTilesLen, healthbarPal, 64, 64);
		setSpriteXY(1, 4, 0, 0);
	}
	else if(mode == MULTI_NORMAL)
	{
		// Player 1
		createSprite(0, 0, 0, rockTiles, rockTilesLen, rockPal, 64, 64);
		setSpriteXY(0, 0, 0, 64 * 0);

		createSprite(0, 1, 1, paperTiles, paperTilesLen, paperPal, 64, 64);
		setSpriteXY(0, 1, 0, 64 * 1);

		createSprite(0, 2, 2, scissorsTiles, scissorsTilesLen, scissorsPal, 64, 64);
		setSpriteXY(0, 2, 0, 64 * 2);

		createSprite(0, 3, 3, lizardTiles, lizardTilesLen, lizardPal, 64, 64);
		setSpriteXY(0, 3, 64, 64 * 0 + 32);

		createSprite(0, 4, 4, spockTiles, spockTilesLen, spockPal, 64, 64);
		setSpriteXY(0, 4, 64, 64 * 1 + 32);

		createSprite(1, 4, 4, healthbarTiles, healthbarTilesLen, healthbarPal, 64, 64);
		setSpriteXY(1, 4, 0, 0);

		// Player 2
		copySprite(0, 5, 5, 0, 0);
		setSpriteXY(0, 5, 256 - 64, 64 * 0);

		copySprite(0, 6, 6, 0, 1);
		setSpriteXY(0, 6, 256 - 64, 64 * 1);

		copySprite(0, 7, 7, 0, 2);
		setSpriteXY(0, 7, 256 - 64, 64 * 2);

		copySprite(0, 8, 8, 0, 3);
		setSpriteXY(0, 8, 256 - 128, 64 * 0 + 32);

		copySprite(0, 9, 9, 0, 4);
		setSpriteXY(0, 9, 256 - 128, 64 * 1 + 32);

		createSprite(1, 5, 5, healthbarTiles, healthbarTilesLen, healthbarPal, 64, 64);
		setSpriteXY(1, 5, 256 - 64, 0);
	}
}

void gameOverScreen()
{
	int i = 0;
	for(i = 0;i < 5;i += 1)
	{
		deleteSprite(0, i);
	}
	deleteSprite(1, 4);

	deleteBg(1, 1);
	createBg(1, 1, 256, 192);
	loadBg(1, 1, gameover_topTiles, gameover_topTilesLen,
		gameover_topMap, gameover_topMapLen, gameover_topPal);

	deleteBg(0, 1);
	createBg(0, 1, 256, 192);
	loadBg(0, 1, gameover_bottomTiles, gameover_bottomTilesLen,
		gameover_bottomMap, gameover_bottomMapLen, gameover_bottomPal);
}

int main()
{
	defaultExceptionHandler();
	initVideo();
	soundEnable();
	srand(time(NULL));

	initTextSystem(true, 3, 3);
	//setFont(0, 32, 95, fontTiles, fontPal);
	//setFont(1, 32, 95, fontTiles, fontPal);

	initializeMainGameGraphics(SINGLE_NORMAL);

	int damage = 0;
	int wins = 0;
	int ties = 0;

	touchPosition touch;

	consoleClear();
	drawText(1, 16, 2, "Wins: %d", wins);
	drawText(1, 16, 4, "Ties: %d", ties);
	drawText(1, 8, 8, "You			Computer");

	while(1)
	{
		scanKeys();
		touchRead(&touch);

		int choice = buttonTouched(touch.px, touch.py);
		if(choice > -1)
		{
			choice = decideMatch_basic(choice);
			switch(choice)
			{
			case -1:
				damage += 1;
				break;
			case 0:
				ties += 1;
				break;
			case 1:
				wins += 1;
				break;
			}
			while(keysHeld() & KEY_TOUCH)
			{
				scanKeys();
			}
			if(damage > -1 && damage < 4)
			{
				setSpriteFrame(1, 4, damage);
			}

			consoleClear();
			drawText(1, 16, 2, "Wins: %d", wins);
			drawText(1, 16, 4, "Ties: %d", ties);
			drawText(1, 8, 8, "You			Computer");
		}

		if(damage > 3)
		{
			consoleClear();
			gameOverScreen();

			damage = -1;

			while(!(keysHeld() & KEY_TOUCH))
			{
				scanKeys();
				updateAll();
			}
			while(keysHeld() & KEY_TOUCH)
			{
				scanKeys();
			}

			initializeMainGameGraphics(SINGLE_NORMAL);
			damage = 0;
			wins = 0;
			ties = 0;

			setSpriteFrame(1, 4, damage);

			consoleClear();
			drawText(1, 16, 2, "Wins: %d", wins);
			drawText(1, 16, 4, "Ties: %d", ties);
			drawText(1, 8, 8, "You			Computer");
		}

		updateAll();
	}
	return 0;
}
