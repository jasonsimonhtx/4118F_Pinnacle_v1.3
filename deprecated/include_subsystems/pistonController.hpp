#pragma once
#include "main.h"

class PistonController {
public:
    PistonController(std::initializer_list<char> ports, bool defaultState);
    
    void set(bool newState) { updateTo(newState); }
    void toggle() { updateTo(!currentState); }
    bool get() const { return currentState; }

private:
    std::vector<std::unique_ptr<pros::adi::DigitalOut>> pistons;
    bool currentState = false;

    void updateTo(bool newState);
};