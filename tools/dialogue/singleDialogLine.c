
#include "common.h"

char *dialogBuf = "1EdI’m Ed! I’m one line of dialog!";

Action *actionBuf = [
  // {offset, length}
  {ActionEnd, {}},
  {ActionDialogLine, {{dialogBuf[3], 31}, {dialogBuf[0], 3}, 0}}
];

