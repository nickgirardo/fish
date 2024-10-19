#include <stdbool.h>
#include "text.h"

#include "../../gametank.h"
#include "../../drawing_funcs.h"
#include "../../../gen/assets/font.h"


#define SPACE_WIDTH 6
#define TEXT_CHAR_WIDTH 8
#define TEXT_CHAR_HEIGHT 8
#define TEXT_LINE_HEIGHT 8

char font_slot;
char text_cursor_x, text_cursor_y;
char text_print_width, text_print_line_start;
char text_color;

char internal_cursor_x;

void init_text() {
    internal_cursor_x = 0;
    text_cursor_x = 0;
    text_cursor_y = 0;
    text_print_width = 128;
    text_print_line_start = 0;
    text_color = TEXT_COLOR_BLACK;
}

void load_font(char slot) {
    font_slot = slot;
    load_spritesheet(&ASSET__font__bios8_bmp, slot);
}

void set_cursor(char x, char y) {
    internal_cursor_x = x;
    text_cursor_x = x;
    text_cursor_y = y;

    text_print_width = 128 - x;
}

char text_tmp;
void print_text(char* text) {
    *dma_flags = (flagsMirror | DMA_GCARRY) & ~(DMA_COLORFILL_ENABLE | DMA_OPAQUE);
    banksMirror = bankflip | GRAM_PAGE(font_slot);
    *bank_reg = banksMirror;
    vram[WIDTH] = TEXT_CHAR_WIDTH;
    vram[HEIGHT] = TEXT_CHAR_HEIGHT;
    vram[VY] = text_cursor_y;
    while(*text != 0) {
        switch(*text) {
            case ' ':
                text_cursor_x += SPACE_WIDTH;
                break;
            case '\n':
                text_cursor_y += TEXT_CHAR_HEIGHT;
                vram[VY] = text_cursor_y;
                break;
            case '\r':
                text_cursor_x = text_print_line_start;
                break;
            default:
                text_tmp = *text + text_color;
                if((text_cursor_x + TEXT_CHAR_WIDTH) >= (text_print_width + text_print_line_start)) {
                    text_cursor_x = internal_cursor_x;
                    text_cursor_y += TEXT_CHAR_HEIGHT;
                    vram[VY] = text_cursor_y;
                }
                vram[VX] = text_cursor_x;
                vram[GX] = (text_tmp & 0x0F) << 3;
                vram[GY] = ((text_tmp & 0xF0) >> 1);
                vram[START] = 1;
                text_cursor_x += TEXT_CHAR_WIDTH;
                wait();
        }
        
        ++text;
    }
}

void print_char(char c) {
    c += text_color;

    *dma_flags = (flagsMirror | DMA_GCARRY) & ~(DMA_COLORFILL_ENABLE | DMA_OPAQUE);
    banksMirror = bankflip | GRAM_PAGE(font_slot);
    *bank_reg = banksMirror;
    vram[WIDTH] = TEXT_CHAR_WIDTH;
    vram[HEIGHT] = TEXT_CHAR_HEIGHT;
    vram[VY] = text_cursor_y;

    if(text_cursor_x >= (text_print_width + text_print_line_start)) {
        text_cursor_x -= text_print_width;
        text_cursor_y += TEXT_CHAR_HEIGHT;
        vram[VY] = text_cursor_y;
    }
    vram[VX] = text_cursor_x;
    vram[GX] = (c & 0x0F) << 3;
    vram[GY] = ((c & 0xF0) >> 1);
    vram[START] = 1;
    text_cursor_x += TEXT_CHAR_WIDTH;
    wait();
}

void print_bcd(unsigned short n) {
  char i;
  bool has_printed = false;

  if (n == 0) {
    print_char('0');
    return;
  }

  i = n >> 12;
  if (i != 0) {
    print_char('0' + i);
    has_printed = true;
  }
  i = (n >> 8) & 15;
  if (i != 0 || has_printed) {
    print_char('0' + i);
    has_printed = true;
  }
  i = (n >> 4) & 15;
  if (i != 0 || has_printed) {
    print_char('0' + i);
    has_printed = true;
  }
  i = n & 15;
  if (i != 0 || has_printed) {
    print_char('0' + i);
    has_printed = true;
  }
}

void print_bcd_zp(unsigned short n) {
  char i;

  i = n >> 12;
  print_char('0' + i);
  i = (n >> 8) & 15;
  print_char('0' + i);
  i = (n >> 4) & 15;
  print_char('0' + i);
  i = n & 15;
  print_char('0' + i);
}
