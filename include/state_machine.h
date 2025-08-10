/* 
  file: state_machine.h
  author: pietro levo
*/

#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

typedef enum {
  STATE_IDLE,
  STATE_WATER_HEATING,
  STATE_COOKING,
  STATE_FINISHED,
  STATE_ERROR,
  STATE_OFFLINE
} cookState;

typedef enum {
  EVENT_RESET_CMD,
  EVENT_START_COOKING,
  EVENT_WATER_BOILING,
  EVENT_COOKING_COMPLETE,
  EVENT_ERROR,
  EVENT_STOP_CMD,
  EVENT_ACK,
  EVENT_CONNECTION_LOST,
  EVENT_CONNECTION_RESTORED
} cookEvent;

void FSM_init(void);
void FSM_set_state(cookState state);
cookState FSM_get_state(void);
void FSM_handle_event(cookEvent event);

#endif