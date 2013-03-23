/*
 * Contains a set of functions for using the 2D video system.
 * Support for the 3D system is not supported (But may be later).
 * Created by: Gerald McAlister
 */
#include "videoFunctions.h"

/*
*  A boolean value that represents whether the screens
*  are flipped or not.
*/
bool screensFlipped = false;

/*
*  Initializes the video for the nintendo DS.
*  Sets up the screens to use backgrounds and sprites with the
*  2D engine.
*/
void initVideo()
{
	/*
	*  Sets the video mode for the main screen to 2D mode, and sets up the vram for bgs and sprites
	*/
	videoSetMode(MODE_0_2D);
	//VRAM Bank A is setup for use by the main screen's background system.
	vramSetBankA(VRAM_A_MAIN_BG);
	//VRAM Bank B is setup for use by the main screen's sprite system.
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	//VRAM Bank E is setup for use by the main screen's extended background palette system.
	vramSetBankE(VRAM_E_BG_EXT_PALETTE);
	//VRAM Bank G is setup for use by the main screen's extended sprite palette system.
	vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);

	/*
	*  Sets the video mode for the sub screen to 2D mode, and sets up the vram for bgs and sprites
	*/
	videoSetModeSub(MODE_0_2D);
	//VRAM Bank C is setup for use by the sub screen's background system.
	vramSetBankC(VRAM_C_SUB_BG);
	//VRAM Bank D is setup for use by the sub screen's sprite system.
	vramSetBankD(VRAM_D_SUB_SPRITE);
	//VRAM Bank H is setup for use by the sub screen's extended background palette system.
	vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
	//VRAM Bank I is setup for use by the sub screen's extended sprite palette system.
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	/*
	*  Initializes the register controls to use extended palettes for bgs
	*  on both the main screen and sub screen
	*/
	REG_DISPCNT |= (DISPLAY_BG_EXT_PALETTE);
	REG_DISPCNT_SUB |= (DISPLAY_BG_EXT_PALETTE);

	/*
	*  Initializes the main screen and sub screen's oams.
	*  The oam is used for sprites
	*/
	oamInit(&oamMain, SpriteMapping_1D_256, true);
	oamInit(&oamSub, SpriteMapping_1D_256, true);
}

/*
*  This function switches the sub screen and the main screen.
*  Can be used for using 3D on the sub screen.
*/
void switchScreens()
{
	REG_POWERCNT ^= (1 << 15);
	screensFlipped = !screensFlipped;
}

