#ifndef _LCD_HD44780
#define _LCD_HD44780

#include "stm32f1xx_hal.h"

#include "lcd.h"

#define LENLONG 12												// максимальное количество (включая знак -) десятичных цифр в переменной типа long


void LCD_PulseEN(void);
void LCD_SendByte(uint8_t, uint8_t);
void LCD_Send4bits(uint8_t);


void LCD_Init(void);											// Инициализация дисплея
void LCD_Clear(void);											// Очистка экрана дисплея
void LCD_GoTo(uint8_t Row, uint8_t Col);						// Установка курсора в заданные координаты
void LCD_PrintStr(uint8_t *Text);								// Печать строки
void LCD_PrintDec(int32_t data);								// Печать десятичного числа

void LCD_GoTo(uint8_t Row, uint8_t Col)    						// Установка курсора в заданные координаты
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
    address |= Col;//     Col - zero based col number
    LCD_SendByte(0x80 | address, FALSE);
}

void LCD_Clear()												// Очистка экрана дисплея
{
    LCD_SendByte(0x01, FALSE);
    LCD_SendByte(0x02, FALSE);
		HAL_Delay(2);
}

void LCD_Init(void)										// Инициализация дисплея (16*2, 5*7, Display on, cursor off, blink cursor off)
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
}

void LCD_PrintStr(uint8_t *Text)							// Печать строки
{
    uint8_t *c;

    c = Text;

    while ((c != 0) && (*c != 0))
    {
        LCD_SendByte(*c, TRUE);
        c++;
    }
}

void LCD_PrintDec(int32_t data){	// Печать десятичного числа

	uint8_t s[LENLONG],	// Массив символов для вывода
	sign=0,							// Флаг знака выводимого числа
	i=LENLONG-1;				// Индекс, указывает на последний элемент массива
	int32_t tmp_data;		// Временная переменая для хранения выводимого числа

	tmp_data=data;

	if(data<0){
		sign='-';
		tmp_data=-tmp_data;
	}

	do
	{
		s[i--]=tmp_data % 10 + '0';			// Заполняем с конца массив выводимых символов
		tmp_data /= 10;
	} while (tmp_data>0);

	if(sign)
		s[i]=sign;
	else
		i++;
	do
		LCD_SendByte(s[i++], TRUE);			// Выводим символ на экран
	while (i<LENLONG);
}

void LCD_PulseEN(void)
{
	HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_E, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_E, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(PORT_LCD, LCD_PIN_E, GPIO_PIN_RESET);
	HAL_Delay(1);
}

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
}

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
 }

// void delay_us(uint32_t value)
//{
// #include "stm32f1xx_ll_tim.h" !!!
//	LL_TIM_SetAutoReload(htim2.Instance, value - 1);
//	LL_TIM_SetCounter(htim2.Instance,0);
//	HAL_TIM_Base_Start(&htim2);
//	while(LL_TIM_IsActiveFlag_UPDATE(htim2.Instance) == 0);
//	LL_TIM_ClearFlag_UPDATE(htim2.Instance);
//}

#endif
