/*
 * This is a DSi only file.  It contains functions for dealing with multitasking which is
 * done with the fifo by checking if the power button has been pressed.
 * Created by: Gerald McAlister
 */
#include "multitasking.h"
#include "textFunctions.h"
#include "generic.h"

/*
 * A static variable for getting the paused value in
 * the fifo.
 */
static bool pausedValue = false;

/*
 * The actual is paused variable.
 */
bool isPaused = false;

/*
 * The battery level.
 */
u32 batteryLevel = 0;

/*
 * Stores the previous value for the multitasking fifo function.
 */
u32 previousValue = 0;

/*
 * Tells whether to display the text that tells the user what to do.
 */
bool useText = true;

/*
 * The actual multitasking functions.  Just checks for fifo data.
 */
static void multiTaskFifoFunction(u32 value, void* userData)
{
	/*
	 * Checks if the value is 0xfe
	 */
	if (value == 0xfe)
	{
		/*
		 * If it is, the whole system is paused/unpaused.
		 */
		pausedValue = !pausedValue;
	}
	/*
	 * Then, the previous value is set to the current value.
	 */
	previousValue = value;
}

/*
 * Used to initialize the background menu.  DSi only.
 */
void initBackgroundMenu(bool canUseText)
{
	/*
	 * Checks if the user is using a Nintendo DSi.
	 */
	if (REG_DSIMODE != 0)
	{
		/*
		 * Sets the multitasking fifo handler function.
		 */
		fifoSetValue32Handler(FIFO_USER_03, multiTaskFifoFunction, 0);
	}
	/*
	 * Sets the system to not paused.
	 */
	isPaused = false;

	/*
	 * Sets to use text.
	 */
	useText = canUseText;
}

/*
 * Used to update the background menu.  DSi only.
 */
void updateBackgroundMenu()
{
	/*
	 * Checks if the system is a DSi.
	*/
	if (REG_DSIMODE == 0)
	{
		/*
		 * If it is not a DSi, then we exit the method.
		*/
		return;
	}

	/*
	 * Sets the battery's current charge.  This is done
	 * because when charging, things tend to get messed up if
	 * the batter level is gotten.
	 */
	batteryLevel = getBatteryLevel();
	/*
	 * Sets whether the game should be paused or not.
	 */
	isPaused = pausedValue;

	/*
	 * Creates a while loop while the system is paused.
	 */
	while (isPaused)
	{
		/*
		 * Sets the battery's current charge.  This is done
		 * because when charging, things tend to get messed up if
		 * the batter level is gotten.
		 */
		batteryLevel = getBatteryLevel();
		/*
		 * Sets whether the game should be paused or not.
		 */
		isPaused = pausedValue;

		/*
		 * Updates the keys.
		 */
		scanKeys();

		/*
		 * Each time through, swiWaitForVBlank is called.
		 */
		swiWaitForVBlank();
	}
}
