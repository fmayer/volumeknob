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

#include "rotary.h"

#include <stdint.h>

#include "hardware/gpio.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/types.h"

namespace {
uint32_t prev_vals;
constexpr auto buttonGpio = 19;
constexpr auto rotaryAGpio = 22;
constexpr auto rotaryBGpio = 21;
}  // namespace

volatile int val;

void interruptCallback(uint gpio, uint32_t events) {
  if (gpio == buttonGpio) {
    reset_usb_boot(0, 0);
    return;
  }
  uint32_t vals = gpio_get_all();
  uint32_t changed_vals = vals ^ prev_vals;
  // We only consider when our chosen phase changes.
  if (~changed_vals & (1 << rotaryAGpio)) {
    return;
  }
  // Only take one of the up-down.
  if ((changed_vals & vals) & (1 << rotaryBGpio)) {
    if (vals & (1 << rotaryAGpio)) {
      val--;
    } else {
      val++;
    }
  }
  prev_vals = vals;
}
