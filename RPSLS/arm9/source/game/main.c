#include <nds.h>
#include <maxmod9.h>
#include <filesystem.h>

#include "GEM_functions.h"

#include "top_main.h"
#include "gameover_top.h"
#include "gameover_bottom.h"

#include "rock.h"
#include "paper.h"
#include "scissors.h"
#include "lizard.h"
#include "spock.h"

#include "healthbar.h"

#define TOTAL_BUTTONS 5

typedef enum
{
	SINGLE_NORMAL = 1,
	MULTI_NORMAL = 2
}playerMode;

unsigned int buttonTouched(int x, int y)
{
	int i = 0;
	for(i = 0; i < TOTAL_BUTTONS;i += 1)
	{
		if(isSpriteTouchingPoint(0, i, x, y))
		{
			setSpriteFrame(0, i, 1);
			updateAll();
			return i;
		}
		setSpriteFrame(0, i, 0);
	}
	return -1;
}

void loadSprite(int screen, int index, const unsigned int* tiles, u32 tilesLen,
	const unsigned short* pal, unsigned int width, unsigned int height)
{
	createSprite(screen, index, index, tiles, tilesLen, pal, width, height);
}

void loadBg(int screen, int layer, const unsigned int* tiles, u32 tilesLen,
	const unsigned short* map, u32 mapLen, const unsigned short* pal)
{
	setBgPalette(screen, layer, pal);
	setBgTiles(screen, layer, tiles, tilesLen);
	setBgMap(screen, layer, map, mapLen);
}

int chooseWinner(int choice, int choice2)
{
	switch(choice)
	{
	case 0:
		switch(choice2)
		{
		case 0:
			return 0;
		case 1:
			return -1;
		case 2:
			return 1;
		case 3:
			return 1;
		case 4:
			return -1;
		}
	case 1:
		switch(choice2)
		{
		case 0:
			return 1;
		case 1:
			return 0;
		case 2:
			return -1;
		case 3:
			return -1;
		case 4:
			return 1;
		}
	case 2:
		switch(choice2)
		{
		case 0:
			return -1;
		case 1:
			return 1;
		case 2:
			return 0;
		case 3:
			return 1;
		case 4:
			return -1;
		}
	case 3:
		switch(choice2)
		{
		case 0:
			return -1;
		case 1:
			return 1;
		case 2:
			return -1;
		case 3:
			return 0;
		case 4:
			return 1;
		}
	case 4:
		switch(choice2)
		{
		case 0:
			return 1;
		case 1:
			return -1;
		case 2:
			return 1;
		case 3:
			return -1;
		case 4:
			return 0;
		}
	}
	return 0;
}

void animateActions(int choice1, int choice2)
{
	switch(choice1)
	{
	case 0:
		loadSprite(1, 0, rockTiles, rockTilesLen, rockPal, 64, 64);
		break;
	case 1:
		loadSprite(1, 0, paperTiles, paperTilesLen, paperPal, 64, 64);
		break;
	case 2:
		loadSprite(1, 0, scissorsTiles, scissorsTilesLen, scissorsPal, 64, 64);
		break;
	case 3:
		loadSprite(1, 0, lizardTiles, lizardTilesLen, lizardPal, 64, 64);
		break;
	case 4:
		loadSprite(1, 0, spockTiles, spockTilesLen, spockPal, 64, 64);
		break;
	default:
		loadSprite(1, 0, rockTiles, rockTilesLen, rockPal, 64, 64);
		break;
	}
	switch(choice2)
	{
	case 0:
		loadSprite(1, 1, rockTiles, rockTilesLen, rockPal, 64, 64);
		break;
	case 1:
		loadSprite(1, 1, paperTiles, paperTilesLen, paperPal, 64, 64);
		break;
	case 2:
		loadSprite(1, 1, scissorsTiles, scissorsTilesLen, scissorsPal, 64, 64);
		break;
	case 3:
		loadSprite(1, 1, lizardTiles, lizardTilesLen, lizardPal, 64, 64);
		break;
	case 4:
		loadSprite(1, 1, spockTiles, spockTilesLen, spockPal, 64, 64);
		break;
	default:
		loadSprite(1, 1, rockTiles, rockTilesLen, rockPal, 64, 64);
		break;
	}

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
		loadSprite(0, 0, rockTiles, rockTilesLen, rockPal, 64, 64);
		setSpriteXY(0, 0, 32 + (64 * 0), 32 + (64 * 0));

		loadSprite(0, 1, paperTiles, paperTilesLen, paperPal, 64, 64);
		setSpriteXY(0, 1, 32 + (64 * 1), 32 + (64 * 0));

		loadSprite(0, 2, scissorsTiles, scissorsTilesLen, scissorsPal, 64, 64);
		setSpriteXY(0, 2, 32 + (64 * 2), 32 + (64 * 0));

		loadSprite(0, 3, lizardTiles, lizardTilesLen, lizardPal, 64, 64);
		setSpriteXY(0, 3, 32 + 32 + (64 * 0), 32 + (64 * 1));

		loadSprite(0, 4, spockTiles, spockTilesLen, spockPal, 64, 64);
		setSpriteXY(0, 4, 32 + 32 + (64 * 1), 32 + (64 * 1));

		loadSprite(1, 4, healthbarTiles, healthbarTilesLen, healthbarPal, 64, 64);
		setSpriteXY(1, 4, 0, 0);
	}
	else if(mode == MULTI_NORMAL)
	{
		// Player 1
		loadSprite(0, 0, rockTiles, rockTilesLen, rockPal, 64, 64);
		setSpriteXY(0, 0, 0, 64 * 0);

		loadSprite(0, 1, paperTiles, paperTilesLen, paperPal, 64, 64);
		setSpriteXY(0, 1, 0, 64 * 1);

		loadSprite(0, 2, scissorsTiles, scissorsTilesLen, scissorsPal, 64, 64);
		setSpriteXY(0, 2, 0, 64 * 2);

		loadSprite(0, 3, lizardTiles, lizardTilesLen, lizardPal, 64, 64);
		setSpriteXY(0, 3, 64, 64 * 0 + 32);

		loadSprite(0, 4, spockTiles, spockTilesLen, spockPal, 64, 64);
		setSpriteXY(0, 4, 64, 64 * 1 + 32);

		loadSprite(1, 4, healthbarTiles, healthbarTilesLen, healthbarPal, 64, 64);
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

		loadSprite(1, 5, healthbarTiles, healthbarTilesLen, healthbarPal, 64, 64);
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
