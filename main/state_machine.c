/* 
  file: state_machine.c
  author: pietro levo
*/

#include "state_machine.h"

static cookState current_state = STATE_IDLE;


static cookState FSM_transition(cookState state, cookEvent event) {
  switch (state) {
  case STATE_IDLE:
    if (event == EVENT_START_COOKING) return STATE_WATER_HEATING;
    if (event == EVENT_CONNECTION_LOST) return STATE_OFFLINE;
    break;

  case STATE_WATER_HEATING:
    if (event == EVENT_WATER_BOILING) return STATE_COOKING;
    if (event == EVENT_ERROR) return STATE_ERROR;
    if (event == EVENT_STOP_CMD) return STATE_IDLE;
    if (event == EVENT_CONNECTION_LOST) return STATE_OFFLINE;
    break;

  case STATE_COOKING:
    if (event == EVENT_COOKING_COMPLETE) return STATE_FINISHED;
    if (event == EVENT_ERROR) return STATE_ERROR;
    if (event == EVENT_STOP_CMD) return STATE_IDLE;
    if (event == EVENT_CONNECTION_LOST) return STATE_OFFLINE;
    break;

  case STATE_FINISHED:
    if (event == EVENT_ACK) return STATE_IDLE;
    if (event == EVENT_CONNECTION_LOST) return STATE_OFFLINE;
    break;

  case STATE_ERROR:
    if (event == EVENT_RESET_CMD) return STATE_IDLE;
    if (event == EVENT_CONNECTION_LOST) return STATE_OFFLINE;
    break;

  case STATE_OFFLINE:
    if (event == EVENT_CONNECTION_RESTORED) return STATE_IDLE;
    break;
  
  default:
    return state;
    break;
  }
}


void FSM_init(void) {
  current_state = STATE_IDLE;
}


void FSM_set_state(cookState state) {
  current_state = state;
}


cookState FSM_get_state(void) {
  return current_state;
}


void FSM_handle_event(cookEvent event) {
  current_state = FSM_transition(current_state, event);
}