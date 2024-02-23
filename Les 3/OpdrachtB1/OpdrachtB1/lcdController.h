/*
 * lcdController.h
 *
 * Created: 09/02/2024 14:39:15
 *  Author: Luc
 */ 
#ifndef LCD_H
#define LCD_H

void lcd_init(void);
void lcd_display_text(char *str);
void lcd_set_cursor(int position);

#endif

