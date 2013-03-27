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
 * Runs the match and animates the results of the match.
 * @param choice1 The first player's choice.
 * @param choice2 The second player's choice.
 * @return Returns the results of the match.
 */
int runMatch(int choice1, int choice2)
{
	// Create the first player's sprite based on the choice.
	createSprite(1, 0, 0, selectionSprites[choice1], selectionSpritesSizes[choice1], selectionSpritesPal[choice1], 64, 64);
	// Create the second player's sprite based on the choice.
	createSprite(1, 1, 1, selectionSprites[choice2], selectionSpritesSizes[choice2], selectionSpritesPal[choice2], 64, 64);

	// The X positions of the sprites.
	int x1 = -64;
	int x2 = 256;
	// The Y positions of the sprites.
	// This is the same for both sprites, and thus
	// only needs one value.
	int y = 128 - 32;
	// The rotation of the losing sprite.
	int rotation = 0;

	// The the sprites to the correct positions.
	setSpriteXY(1, 0, x1, y);
	setSpriteXY(1, 1, x2, y);

	// Set the speed to be 4 pixels per movement.
	int speed = 4;

	// Move the sprites together until they touch.
	while(x1 + 64 != x2)
	{
		// Update the positions.
		x1 += speed;
		x2 -= speed;

		// Set the positions.
		setSpriteXY(1, 0, x1, y);
		setSpriteXY(1, 1, x2, y);

		// Update the game's display.
		updateAll();
	}

	// Get the winner of the match.
	int winner = chooseWinner(choice1, choice2);

	// If the winner is player 1, then player 2's sprite spirals off screen.
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
	// If the winner is player 2, then player 1's sprite spirals off screen.
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
	// If the match is a tie, both sprite's spiral off screen.
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

	// Delete the player's sprites.
	deleteSprite(1, 0);
	deleteSprite(1, 1);

	return winner;
}

/*
 * Initializes the game's graphics.
 * @param mode The mode of the game (Single player or multi player).
 */
void initializeMainGameGraphics(playerMode mode)
{
	// Delete the two screens backgrounds in case of game over.
	deleteBg(0, 1);
	deleteBg(1, 1);

	// Set the backdrop color to be a dark shade of blue.
	setBackdropColor(RGB15(1, 5, 9));
	setBackdropColorSub(RGB15(1, 5, 9));

	// Check if the mode is for a single player.
	if(mode == SINGLE_NORMAL)
	{
		// Player 1's choices.
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
		// Player 2's choice don't need to be shown.
	}
	// Check if the mode is for multiple player.
	else if(mode == MULTI_NORMAL)
	{
		// Player 1's choices.
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

		// Player 2's choices.
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

/*
 * Create's the game over screen.
 */
void gameOverScreen()
{
	int i = 0;
	// Loop through and delete all of the bottom screen's sprites.
	for(i = 0;i < 5;i += 1)
	{
		deleteSprite(0, i);
	}
	// Delete the health bar sprite.
	deleteSprite(1, 4);

	// Delete the default backgrounds.
	deleteBg(0, 1);
	deleteBg(1, 1);

	// Create the game over backgrounds.
	createBg(0, 1, 256, 192);
	createBg(1, 1, 256, 192);

	// Load the game over backgrounds' data.
	loadBg(0, 1, gameover_bottomTiles, gameover_bottomTilesLen,
		gameover_bottomMap, gameover_bottomMapLen, gameover_bottomPal);
	loadBg(1, 1, gameover_topTiles, gameover_topTilesLen,
		gameover_topMap, gameover_topMapLen, gameover_topPal);
}

int main()
{
	// Initialize exception handling for debugging purposes.
	defaultExceptionHandler();

	// Initialize video (IE: Video Ram).
	initVideo();

	// Initialize the text system.
	initTextSystem(true, 3, 3);

	// Enable sound.
	soundEnable();

	// Seed the random method.
	srand(time(NULL));

	// Initialize the game's graphics for single player.
	initializeMainGameGraphics(SINGLE_NORMAL);

	// Set the damage, wins, and ties to 0.
	int damage = 0, wins = 0, ties = 0;

	// Create a touch position variable for the touch screen.
	touchPosition touch;

	// Clear the console of any possible old text.
	consoleClear();

	// Draw the text for the top screen.
	drawText(1, 16, 2, "Wins: %d", wins);
	drawText(1, 16, 4, "Ties: %d", ties);
	drawText(1, 8, 8, "You			Computer");

	while(1)
	{
		// Read the keys being pressed and touch screen data.
		scanKeys();
		touchRead(&touch);

		// Get the player's button choice.
		int choice = buttonTouched(touch.px, touch.py);

		// If the choie is not negative (IE: The player pressed a button), then run a match.
		if(choice > -1)
		{
			// Run the match with a random number (IE: A "CPU") and store the results
			// in the choice variable.
			choice = runMatch(choice, rand() % 5);

			// Check the value of hte choice.
			switch(choice)
			{
			case -1:
				// Result of -1 does damage.
				damage += 1;
				break;
			default:
				// Anything else is a tie.
				ties += 1;
				break;
			case 1:
				// Result of 1 is a win.
				wins += 1;
				break;
			}

			// Wait until the player is not holding down on the touch screen.
			while(keysHeld() & KEY_TOUCH)
			{
				scanKeys();
			}

			// Check that the player has not died yet.
			if(damage > -1 && damage < 4)
			{
				// If so, then set the health bar's frame.
				setSpriteFrame(1, 4, damage);
			}

			// Clear the console's old text.
			consoleClear();

			// Display the current results after the match.
			drawText(1, 16, 2, "Wins: %d", wins);
			drawText(1, 16, 4, "Ties: %d", ties);
			drawText(1, 8, 8, "You			Computer");
		}

		// Check if the player has recieved too much damage.
		if(damage > 3)
		{
			// If so, clear the console and display the game over screen.
			consoleClear();
			gameOverScreen();

			// While the touchscreen is not being touched, wait
			// and update the graphics.
			while(!(keysHeld() & KEY_TOUCH))
			{
				scanKeys();
				updateAll();
			}
			// Once it has been touched, wait until it no longer is.
			while(keysHeld() & KEY_TOUCH)
			{
				scanKeys();
			}

			// Then, initialize the graphics for single player again.
			initializeMainGameGraphics(SINGLE_NORMAL);
			// Reset the damage, wins', and ties' variables.
			damage = wins = ties = 0;

			// Set the heatlhbar frame to the initial frame.
			setSpriteFrame(1, 4, damage);

			// Reset the text.
			drawText(1, 16, 2, "Wins: %d", wins);
			drawText(1, 16, 4, "Ties: %d", ties);
			drawText(1, 8, 8, "You			Computer");
		}

		// Update all of the game.
		updateAll();
	}
	// Return 0 when done.
	return 0;
}
