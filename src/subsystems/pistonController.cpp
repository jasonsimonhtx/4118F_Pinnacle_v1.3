#include "main.h"

PistonController::PistonController(std::initializer_list<char> ports, bool defaultState)
    : currentState(defaultState)
{
    for (char port : ports) {
        pistons.push_back(std::make_unique<pros::adi::DigitalOut>(port));
        pistons.back()->set_value(defaultState);
    }
}

void PistonController::updateTo(bool newState) {
    for (auto &piston : pistons) {
        piston->set_value(newState);
    }
    currentState = newState;
}
