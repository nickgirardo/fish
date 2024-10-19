#ifndef TEXT_H
#define TEXT_H

#define TEXT_COLOR_BLACK 0
#define TEXT_COLOR_WHITE 128

void init_text();

void load_font(char slot);

void set_cursor(char x, char y);

void print_text(char* text);
void print_char(char c);

void print_bcd(unsigned short n);
void print_bcd_zp(unsigned short n);

extern char text_cursor_x, text_cursor_y, text_print_width, text_print_line_start;
extern char text_color;

#endif
