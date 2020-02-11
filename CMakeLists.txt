cmake_minimum_required(VERSION 3.8.2)

set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(BOARD simblee_thermocouple_board)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(cooker-node)

target_sources(app PRIVATE
  src/main.c
  #src/cts.c
  src/ess.c
  src/bas.c
  src/battery.c
)
