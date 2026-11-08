#pragma once
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

void brain_init();
void controller_init();
void brain_periodic();
void controller_periodic();