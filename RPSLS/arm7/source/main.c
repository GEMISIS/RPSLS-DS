#include <nds.h>
#include <dswifi7.h>
#include <maxmod7.h>

void vBlankHandler(void)
{
	Wifi_Update();
}

void vCountHandler()
{
	inputGetAndSend();
}

volatile bool exitProgram = false;

void powerButtonHandler()
{
	fifoSendValue32(FIFO_USER_03, 0xfe);
}

static void fifoHandlerFunction(u32 value, void* userData)
{
	if (value == 0xff)
	{
		i2cWriteRegister(I2C_PM, 0x70, 1);
		i2cWriteRegister(I2C_PM, 0x11, 1);
	}
	if (value == 0xfe)
	{
		fifoSendValue32(FIFO_USER_03, 0xfd);
		fifoSendValue32(FIFO_USER_03, i2cReadRegister(I2C_PM, I2CREGPM_BATTERY));
	}
	if (value == 0xfd)
	{
		fifoSendValue32(FIFO_USER_03, 0xfc);
		fifoSendValue32(FIFO_USER_03, i2cReadRegister(I2C_PM, I2CREGPM_BATTERY)
				& BIT(7));
	}
}

int main()
{
	readUserSettings();

	irqInit();
	initClockIRQ();
	fifoInit();

	mmInstall(FIFO_MAXMOD);

	SetYtrigger(80);

	installWifiFIFO();
	installSoundFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, vCountHandler);
	irqSet(IRQ_VBLANK, vBlankHandler);
	irqEnableAUX(IRQ_I2C);

	irqEnable(IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	setPowerButtonCB(powerButtonHandler);

	fifoSetValue32Handler(FIFO_USER_03, fifoHandlerFunction, 0);

	while (!exitProgram)
	{
		if (0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R)))
		{
			exitProgram = true;
		}

		swiWaitForVBlank();
	}
	return 0;
}
