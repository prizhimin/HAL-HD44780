#ifndef _LCD_HD44780_H
#define _LCD_HD44780_H

#include "stm32f1xx_hal.h"

#define TRUE				1
#define FALSE				0

#define	PORT_LCD		GPIOB
#define	LCD_PIN_D4		GPIO_PIN_12
#define	LCD_PIN_D5		GPIO_PIN_15
#define	LCD_PIN_D6		GPIO_PIN_13
#define	LCD_PIN_D7		GPIO_PIN_14
#define LCD_PIN_E		GPIO_PIN_9
#define LCD_PIN_RS		GPIO_PIN_8

void LCD_Init(void);				// Initializing the display
void LCD_Clear(void);				// Cleaning the display
void LCD_GoTo(uint8_t Row, uint8_t Col);	// Set the cursor to the specified coordinates
void LCD_PrintStr(uint8_t *Text);		// Print a string
void LCD_PrintDec(int32_t data);		// Print a decimal number


#endif
