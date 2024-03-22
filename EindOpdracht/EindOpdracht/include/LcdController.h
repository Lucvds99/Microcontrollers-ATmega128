/*
 * lcdController.h
 *
 * Created: 15/03/2024 11:12:59
 *  Author: teunl
 */ 


#ifndef LCDCONTROLLER_H
#define LCDCONTROLLER_H

void lcd_init(void);
void lcd_display_text(char *str);
void lcd_set_cursor(int position);
void lcd_clear_display(void);




#endif /* LCDCONTROLLER_H_ */