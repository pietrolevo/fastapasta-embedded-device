/* 
  file: state_machine.c
  author: pietro levo
*/

#include "state_machine.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static cookState current_state = STATE_IDLE;
static SemaphoreHandle_t state_mutex = NULL;
static QueueHandle_t event_queue = NULL;

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
    return STATE_ERROR;
    break;
  }
  return STATE_ERROR;
}


void FSM_init(void) {
  current_state = STATE_IDLE;
  state_mutex = xSemaphoreCreateMutex();
  event_queue = xQueueCreate(10, sizeof(cookEvent));
}


void FSM_set_state(cookState state) {
  if (xSemaphoreTake(state_mutex, portMAX_DELAY)) {
    current_state = state;
    xSemaphoreGive(state_mutex);
  }
}


cookState FSM_get_state(void) {
  cookState state;
  if (xSemaphoreTake(state_mutex, portMAX_DELAY)) {
    state = current_state;
    xSemaphoreGive(state_mutex);
  }
  return state;
}


void FSM_handle_event(cookEvent event) {
  if (xSemaphoreTake(state_mutex, portMAX_DELAY)) {
    cookState new_state = FSM_transition(current_state, event);
    current_state = new_state;
    xSemaphoreGive(state_mutex);
  }
}


void FSM_receive_event(cookEvent event) {
  xQueueSend(event_queue, &event, portMAX_DELAY);
}


void FSM_event_task(void *pvParameters) {
  cookEvent event;
  while(1) {
    if (xQueueReceive(event_queue, &event, portMAX_DELAY)) {
      FSM_handle_event(event);
    }
  }
}