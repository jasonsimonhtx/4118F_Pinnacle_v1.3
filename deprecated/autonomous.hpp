#pragma once
#include "main.h"

const uint32_t RED = 0xFF2222;
const uint32_t BLUE = 0x2255FF;
const uint32_t GRAY = 0xAAAAAA;

using RouteID = uint8_t;
using RouteFunc = void(*)();
struct Route {
    RouteID id;
    std::string title;
    std::string description;
    uint32_t display_color;
    RouteFunc func;
};

inline std::atomic<RouteID> current_route_id{0};

// aut_(type)_(strat)_(side)
void aut_match_stack_right();
void aut_match_stack_left();
void aut_match_toggle_right();
void aut_match_toggle_left();
void aut_match_idle();

inline std::vector<Route> routes = {
    {0, "15-Sec Red-Right Stack",   "3x red pins in red goal",  RED, &aut_match_stack_right},
    {1, "15-Sec Red-Left Stack",    "3x red pins in red goal",  RED, &aut_match_stack_left},
    {2, "15-Sec Blue-Right Stack",  "3x blue pins in blue goal", BLUE, &aut_match_stack_right},
    {3, "15-Sec Blue-Left Stack",   "3x blue pins in blue goal", BLUE, &aut_match_stack_left},
    {4, "15-Sec Red-Right Toggle",  "2x red pins in red goal + 1 toggle", RED, &aut_match_toggle_right},
    {5, "15-Sec Red-Left Toggle",   "2x red pins in red goal + 1 toggle", RED, &aut_match_toggle_left},
    {6, "15-Sec Blue-Right Toggle", "2x blue pins in blue goal + 1 toggle", BLUE, &aut_match_toggle_right},
    {7, "15-Sec Blue-left Toggle",  "2x blue pins in blue goal + 1 toggle", BLUE, &aut_match_toggle_left},
    {8, "15-Sec BACKUP", "Drives 6\" to not touch the loader.", GRAY, &aut_match_idle},
};