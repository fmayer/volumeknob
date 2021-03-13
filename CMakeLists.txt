# Copyright (C) 2021 Florian Mayer
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
project(pico_examples C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(volctrl
        volctrl.cc
        usb_descriptors.cc
        rotary.cc
        )

target_include_directories(volctrl PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(volctrl PRIVATE pico_stdlib tinyusb_device tinyusb_board)
pico_add_extra_outputs(volctrl)