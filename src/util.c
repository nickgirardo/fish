#include "util.h"

char dec_bcd_char(char n) {
  if (n == 0) return 0;
  if ((n & 0x0F) == 0x00) return (n - 0x10) + 0x09;

  return n - 1;
}

char inc_bcd_char(char n) {
  if ((n & 0x0F) != 0x09) return n + 0x01;
  if ((n & 0xF0) != 0x90) return (n + 0x10) & 0xF0;

  return 0x99;
}

unsigned short inc_bcd_short(unsigned short n) {
  if ((n & 0x000F) != 0x0009) return n + 0x0001;
  if ((n & 0x00F0) != 0x0090) return (n + 0x0010) & 0xFFF0;
  if ((n & 0x0F00) != 0x0900) return (n + 0x0100) & 0xFF00;
  if ((n & 0xF000) != 0x9000) return n + 0x1000 & 0xF000;

  return 0x9999;
}
