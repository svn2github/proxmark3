#include <proxmark3.h>
#include "apps.h"
#include "LCD.h"

void LCDSend(unsigned int data)
{
	// 9th bit set for data, clear for command
	while ((SPI_STATUS & SPI_STATUS_TX_EMPTY) == 0);	// wait for the transfer to complete
	// For clarity's sake we pass data with 9th bit clear and commands with 9th
	// bit set since they're implemented as defines, se we need to invert bit
	SPI_TX_DATA = data^0x100;							// Send the data/command
}

void LCDSetXY(unsigned char x, unsigned char y)
{
	LCDSend(PPASET);			// page start/end ram
	LCDSend(y);					// Start Page to display to
	LCDSend(131);				// End Page to display to

	LCDSend(PCASET);			// column start/end ram
	LCDSend(x);					// Start Column to display to
	LCDSend(131);				// End Column to display to
}

void LCDSetPixel(unsigned char x, unsigned char y, unsigned char color)
{
	LCDSetXY(x,y);				// Set position
	LCDSend(PRAMWR);			// Now write the pixel to the display
	LCDSend(color);				// Write the data in the specified Color
}

void LCDFill (unsigned char xs,unsigned char ys,unsigned char width,unsigned char height, unsigned char color)
{
    unsigned char i,j;

    for (i=0;i < height;i++)	// Number of horizontal lines
    {
		LCDSetXY(xs,ys+i);		// Goto start of fill area (Top Left)
		LCDSend(PRAMWR);		// Write to display

		for (j=0;j < width;j++)	// pixels per line
			LCDSend(color);
    }
}

void LCDString (char *lcd_string, const char *font_style,unsigned char x, unsigned char y, unsigned char fcolor, unsigned char bcolor)
{
	unsigned int  i;
	unsigned char mask=0, px, py, xme, yme, offset;
	const char *data;

	data = font_style;			// point to the start of the font table

	xme = *data;				// get font x width
	data++;
	yme = *data;				// get font y length
	data++;
	offset = *data;				// get data bytes per font

	do
	{
		// point to data in table to be loaded
		data =  (font_style + offset) + (offset * (int)(*lcd_string - 32));

		for (i=0;i < yme;i++) {
			mask |=0x80;

			for (px=x; px < (x + xme); px++) {
				py= y + i;

				if (*data & mask)	LCDSetPixel (px,py,fcolor);
				else				LCDSetPixel (px,py,bcolor);

				mask>>=1;
			}
			data++;
		}
		x+=xme;

		lcd_string++;						// next character in string

	} while(*lcd_string !='\0');			// keep spitting chars out until end of string
}

void LCDReset(void)
{
	LED_A_ON();
	SetupSpi(SPI_LCD_MODE);
	LCD_RESET_LOW();
	SpinDelay(100);

	LCD_RESET_HIGH();
	SpinDelay(100);
	LED_A_OFF();
}

void LCDInit(void)
{
	int i;

	LCDReset();

	LCDSend(PSWRESET);			// software reset
	SpinDelay(100);
	LCDSend(PSLEEPOUT);			// exit sleep mode
	LCDSend(PBSTRON);			// booster on
	LCDSend(PDISPON);			// display on
	LCDSend(PNORON);			// normal on
	LCDSend(PMADCTL);			// rotate display 180 deg
	LCDSend(0xC0);

	LCDSend(PCOLMOD);			// color mode
	LCDSend(0x02);				// 8bpp color mode

    LCDSend(PSETCON);			// set contrast
    LCDSend(0xDC);
    
	// clear display
    LCDSetXY(0,0);
	LCDSend(PRAMWR);			// Write to display
	i=LCD_XRES*LCD_YRES;
	while(i--) LCDSend(WHITE);
}
