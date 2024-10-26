#include "action.h"

Action *queued_action;

extern Action action_buf[];

// TODO this is for testing
Action *entity_actions[] = {
  &action_buf[2],
  &action_buf[2],
};


void process_action() {
  Action action;

  if (!queued_action) return;


  while (queued_action) {
    action = *queued_action;

    switch (action.kind) {
    case ActionEnd:
	queued_action = 0;
	// TODO return the game to a "normal" state
	return;
    case ActionDialogLine:
	// TODO render dialog
	return;
    case ActionPortal:
	set_portal_target(
	    action.data.apd.scroll,
	    action.data.apd.scrollstop_left,
	    action.data.apd.scrollstop_right,
	    action.data.apd.player_x,
	    action.data.apd.player_y
	);

	queued_action = action.data.apd.next;
	return;
    }
  }
}
