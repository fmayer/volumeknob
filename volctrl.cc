/*
 * Copyright (C) 2021 Florian Mayer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board.h"
#include "class/hid/hid.h"
#include "hardware/gpio.h"
#include "rotary.h"
#include "tusb.h"

#include "usb_descriptors.h"

void hid_task();

int main() {
  board_init();
  tusb_init();

  gpio_init(19);
  gpio_init(21);
  gpio_init(22);
  gpio_set_dir(19, GPIO_IN);
  gpio_set_dir(21, GPIO_IN);
  gpio_set_dir(22, GPIO_IN);

  gpio_pull_up(19);
  gpio_pull_up(21);
  gpio_pull_up(22);

  gpio_set_irq_enabled_with_callback(
      19, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, interruptCallback);
  gpio_set_irq_enabled_with_callback(
      21, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, interruptCallback);
  gpio_set_irq_enabled_with_callback(
      22, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, interruptCallback);

  while (1) {
    tud_task();
    hid_task();
  }

  return 0;
}

void hid_task() {
  if (tud_suspended()) {
    tud_remote_wakeup();
  }

  if (tud_hid_ready()) {
    auto saved = save_and_disable_interrupts();
    int x = val / 2;
    if (val < 0) {
      val = -abs(val % 2);
    } else {
      val = val % 2;
    }
    restore_interrupts(saved);
    uint16_t key;
    if (x < 0) {
      key = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    } else if (x > 0) {
      key = HID_USAGE_CONSUMER_VOLUME_INCREMENT;
    }
    for (int i = 0; i < abs(x); ++i) {
      tud_hid_report(REPORT_ID_CONSUMER, &key, sizeof(key));
    }
  }
}

uint16_t tud_hid_get_report_cb(uint8_t, hid_report_type_t, uint8_t*, uint16_t) {
  return 0;
}

void tud_hid_set_report_cb(uint8_t,
                           hid_report_type_t,
                           uint8_t const*,
                           uint16_t) {}
