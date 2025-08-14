/*
  file: main.c
  author: pietro levo
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "state_machine.h"

int app_main(void) {
  FSM_init();
  vTaskDelay(pdMS_TO_TICKS(1000));
  xTaskCreate(FSM_event_task, "FSM Task", 2048, NULL, 5, NULL);

// Simulazione eventi
  FSM_receive_event(EVENT_START_COOKING);
  vTaskDelay(pdMS_TO_TICKS(1000));

  FSM_receive_event(EVENT_WATER_BOILING);
  vTaskDelay(pdMS_TO_TICKS(1000));

  FSM_receive_event(EVENT_COOKING_COMPLETE);
  vTaskDelay(pdMS_TO_TICKS(1000));

  FSM_receive_event(EVENT_ACK);

  return 0;
}