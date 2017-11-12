#ifndef _LCD_HD44780
#define _LCD_HD44780

#include "stm32f1xx_hal.h"

#include "lcd.h"

#define LENLONG 12						// The maximum number (including minus) of decimal digits in a variable of type int32_t


void LCD_PulseEN(void);
void LCD_SendByte(uint8_t, uint8_t);
void LCD_Send4bits(uint8_t);


void LCD_Init(void);						// Initializing the display
void LCD_Clear(void);						// Cleaning the display
void LCD_GoTo(uint8_t Row, uint8_t Col);			// Set the cursor to the specified coordinates
void LCD_PrintStr(uint8_t *Text);				// Print a string
void LCD_PrintDec(int32_t data);				// Print a decimal number

void LCD_GoTo(uint8_t Row, uint8_t Col)    			// Set the cursor to the specified coordinates
{
    char address;
	switch(Row)
	{
		case 0:	address = 0;
				break;
		case 1:	address = 0x40;
				break;
		case 2:	address = 0x14;
				break;
		case 3:	address = 0x54;
				break;
		default:address = 0;
	}
    address |= Col;						// Col - zero based col number
    LCD_SendByte(0x80 | address, FALSE);
} // LCD_GoTo

void LCD_Clear()						// Cleaning the display
{
    LCD_SendByte(0x01, FALSE);
    LCD_SendByte(0x02, FALSE);
		HAL_Delay(2);
} // LCD_Clear

void LCD_Init(void)						// Initializing the display (16*2, 5*7, Display on, cursor off, blink cursor off)
{

	HAL_Delay(15);
    LCD_Send4bits(3);
    LCD_PulseEN();
	HAL_Delay(5);
    LCD_Send4bits(3);
    LCD_PulseEN();
	HAL_Delay(1);
    LCD_Send4bits(3);
    LCD_PulseEN();
	LCD_Send4bits(2);
    LCD_PulseEN();
    LCD_SendByte(0x28, FALSE);
    LCD_SendByte(0x0C, FALSE);
    LCD_SendByte(0x06, FALSE);
} // LCD_Init

void LCD_PrintStr(uint8_t *Text)				// Print a string
{
    uint8_t *c;

    c = Text;

    while ((c != 0) && (*c != 0))
    {
        LCD_SendByte(*c, TRUE);
        c++;
    }
} // LCD_PrintStr

void LCD_PrintDec(int32_t data){				// Print a decimal number

	uint8_t s[LENLONG],					// Array of symbols for output
	sign=0,							// Flag of the sign of the output number
	i=LENLONG-1;						// The index points to the last element of the array
	int32_t tmp_data;					// Temporary variable to store the output number

	tmp_data=data;

	if(data<0){
		sign='-';
		tmp_data=-tmp_data;
	}

	do
	{
		s[i--]=tmp_data % 10 + '0';			// Fill in the end with an array of output characters
		tmp_data /= 10;
	} while (tmp_data>0);

	if(sign)
		s[i]=sign;
	else
		i++;
	do
		LCD_SendByte(s[i++], TRUE);			// Display the symbol on the screen
	while (i<LENLONG);
} // LCD_PrintDec

void LCD_PulseEN(void)
{
	HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_E, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_E, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_E, GPIO_PIN_RESET);
	HAL_Delay(1);
} // LCD_PulseEN

void LCD_SendByte(uint8_t ByteToSend, uint8_t IsData)
{
    LCD_Send4bits(ByteToSend >> 4);
    if (IsData == TRUE)
    {
		HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_RS, GPIO_PIN_SET);
    }
    else
    {
		HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_RS, GPIO_PIN_RESET);
    }
    LCD_PulseEN();
    LCD_Send4bits(ByteToSend);
    if (IsData == TRUE)
    {
		HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_RS, GPIO_PIN_SET);
    }
    else
    {
		HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_RS, GPIO_PIN_RESET);
    }
    LCD_PulseEN();
} // LCD_SendByte

void LCD_Send4bits(uint8_t value) {
	const uint16_t data_pin[4]={LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7};
	uint8_t tmp, i;
	HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_D4 | LCD_PIN_D5 | LCD_PIN_D6 | LCD_PIN_D7, GPIO_PIN_RESET);
	tmp = value;
	for (i = 0; i < 4; i++) {
		if (tmp & 01)
			HAL_GPIO_WritePin(PORT_LCD, data_pin[i], GPIO_PIN_SET);
		tmp = tmp >> 1;
	}
 } // LCD_Send4bits

#endif
