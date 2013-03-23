/*
 * A set of functions for dealing with the text engine.  Uses
 * the tiled bg system.
 * Created by: Gerald McAlister
 */
#include "textFunctions.h"
#include "generic.h"

/*
 * The print console for the top screen.
 */
PrintConsole topScreen;
/*
 * The print console for the bottom screen.
 */
PrintConsole bottomScreen;

int topId = 0;
int bottomId = 0;

/*
 * Initializes the text system.
 * @param loadDefaultFonts Indicates whether or not it should
 * load the default fonts.
 * @param topScreenLayer The background layer to display the text on on the
 * top screen. (Use -1 to not load the top screen's text system)
 * @param bottomScreenLayer The background layer to display the text on on the
 * bottom screen. (Use -1 to not load the bottom screen's text system)
 */
void initTextSystem(bool loadDefaultFonts, int topScreenLayer,
		int bottomScreenLayer)
{
	/*
	 * Checks to make sure the top screen layer is > -1 and is < 4.
	 */
	if (topScreenLayer > -1 && topScreenLayer < 4)
	{
		/*
		 * If it does meet the requirements above, then its console
		 * is initialized.
		 */
		PrintConsole* c = consoleInit(&topScreen, topScreenLayer, BgType_ExRotation,
				BgSize_ER_256x256, topScreenLayer * 4, (topScreenLayer < 3) ? topScreenLayer * 2 + 2 : topScreenLayer * 2 + 1, true,
				loadDefaultFonts);
		topId = c->bgId;
	}

	/*
	 * Checks to make sure the bottom screen layer is > -1 and is < 4.
	 */
	if (bottomScreenLayer > -1 && bottomScreenLayer < 4)
	{
		/*
		 * If it does meet the requirements above, then its console
		 * is initialized.
		 */
		PrintConsole* c = consoleInit(&bottomScreen, bottomScreenLayer, BgType_ExRotation,
				BgSize_ER_256x256, bottomScreenLayer * 4, (bottomScreenLayer < 3) ? bottomScreenLayer * 2 + 2 : bottomScreenLayer * 2 + 1,
				false, loadDefaultFonts);
		bottomId = c->bgId;
	}
}

/*
 * Sets the font on the desired screen with the chosen properties.
 * @param screen The screen to set the font for.
 * @param offSet The offset of the first character.
 * @param numberOfCharacters The total number of characters available for display.
 * @param fontTiles The tiles for the font.
 * @param fontPal The palette for the font.
 */
void setFont(int screen, int offSet, int numberOfCharacters,
		const unsigned int* fontTiles, const unsigned short* fontPal)
{
	/*
	 * Creates a temporary font.
	 */
	ConsoleFont font;

	/*
	 * Sets the font's graphics.
	 */
	font.gfx = (u16*) fontTiles;
	/*
	 * Sets the font's palette.
	 */
	font.pal = (u16*) fontPal;
	/*
	 * Sets the number of characters in the font.
	 */
	font.numChars = numberOfCharacters;
	/*
	 * Sets the number of colors in the font.
	 */
	font.numColors = 32 / 2;
	/*
	 * Sets the bits per pixel for the font's graphics.
	 */
	font.bpp = 4;
	/*
	 * Sets the offset of the first ASCII character.
	 */
	font.asciiOffset = offSet;
	/*
	 * Tells it to use multiple colors.
	 */
	font.convertSingleColor = false;

	/*
	 * Checks if the screen is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If it is, then set the font for the bottom screen.
		 */
		consoleSetFont(&bottomScreen, &font);
	}
	else
	{
		/*
		 * Otherwise, the font for the top screen is set.
		 */
		consoleSetFont(&topScreen, &font);
	}
}

/*
 * Sets the position of the text.
 * @param screen The screen to select.
 * @param x The x position to put the text at.
 * @param y The y position to put the text at.
 */
void setText(int screen, int x, int y)
{
	/*
	 * Checks if the screen is <= 0.
	 */
	if (screen <= 0)
	{
		/*
		 * If so, then the bottom screen is selected.
		 */
		consoleSelect(&bottomScreen);
	}
	else
	{
		/*
		 * Otherwise, the top screen is selected.
		 */
		consoleSelect(&topScreen);
	}

	/*
	 * Sets the font position.  The format is \x1b[y;xH
	 */
	printf("\x1b[%d;%dH", y, x);
}

/*
 * Creates 2 temporary characters for getting the string
 * formating parameters.
 */
char temp[2];

/*
 * Draws text on the screen with the desired properties.
 * @param screen The screen to draw the text on.
 * @param x The x position to put the text at.
 * @param y The y position to put the text at.
 * @param text The text to draw.  Supports string formating.
 */
void drawText(int screen, int x, int y, const char* text, ...)
{
	/*
	 * Creates a temporary variable i for looping through the
	 * characters in the text.
	 */
	int i = 0;

	/*
	 * Sets the text's position.
	 */
	setText(screen, x, y);

	/*
	 * Creates a temporary variable list for getting
	 * the string formating variables.
	 */
	va_list variableList;
	/*
	 * Then, it initializes the variable list with
	 * the same size as the text.
	 */
	va_start(variableList, text);

	/*
	 * Then, the text's characters are looped through in
	 * a for loop.
	 */
	for (i = 0; text[i]; i += 1)
	{
		/*
		 * Each character is checked to see if it begins with a % sign.
		 */
		switch (text[i])
		{
		case '%':
			temp[0] = '%';
			temp[1] = text[i + 1];
			/*
			 * If the first character is a % character, then
			 * the second character is switched through.
			 */
			switch (temp[1])
			{
			case 'c':
				/*
				  * Checks to see if the next letters are 'o' and 'l' for colors.
				*/
				if(text[i + 2] == 'o' && text[i + 3] == 'l')
				{
					/*
					 * If they are, then it it's a color and a temp color is created.
					*/
					color_t c = va_arg(variableList, color_t);
					/*
					 * Then it is written to the screen depending on its format.
					*/
					if(text[i + 4] == 'x')
					{
						printf("0x%x, 0x%x, 0x%x", c.r, c.g, c.b);
						i += 3;
					}
					else if(text[i + 4] == 'X')
					{
						printf("0x%X, 0x%X, 0x%X", c.r, c.g, c.b);
						i += 3;
					}
					else
					{
						printf("%d, %d, %d", c.r, c.g, c.b);
						i += 2;
					}
				}
				else
				{
					/*
					 * If it is a character it is printed as an integer (same thing).
					 */
					printf(temp, va_arg(variableList, int));
				}
				break;
			case 'd':
				/*
				 * If it is an integer it is printed as that.
				 */
				printf(temp, va_arg(variableList, int));
				break;
			case 'i':
				/*
				 * If it is an integer it is printed as that.
				 */
				printf(temp, va_arg(variableList, int));
				break;
			case 'e':
				/*
				 * If it is a long it is printed as that.
				 */
				printf(temp, va_arg(variableList, long));
				break;
			case 'E':
				/*
				 * If it is a integer it is printed as that.
				 */
				printf(temp, va_arg(variableList, int));
				break;
			case 's':
				/*
				 * If it is a character string it is printed as that.
				 */
				printf(temp, va_arg(variableList, char*));
				break;
			case 'u':
				/*
				 * If it is an unsigned integer it is printed as that.
				 */
				printf(temp, va_arg(variableList, unsigned int));
				break;
			case 'f':
				/*
				 * If it is a float it is printed as that.
				 */
				printf(temp, va_arg(variableList, double));
				break;
			case 'x':
				/*
				 * If it is 'x', print as hex.
				 */
				printf(temp, va_arg(variableList, int));
				break;
			case 'X':
				/*
				 * If it is 'x', print as hex.
				 */
				printf(temp, va_arg(variableList, int));
				break;
			case '%':
				/*
				 * If it is a % character it is printed.
				 */
				printf("%%");
				break;
			}
			/*
			 * i is incremented by 1 as well since the next character
			 * was checked.
			 */
			i += 1;
			/*
			 * Finally, a break is called.
			 */
			break;
		default:
			/*
			 * Otherwise, if it is not a % character, the character
			 * is printed as normal.
			 */
			printf("%c", text[i]);
			break;
		}
	}
	/*
	 * Finally, the variable list is ended.
	 */
	va_end(variableList);
}

