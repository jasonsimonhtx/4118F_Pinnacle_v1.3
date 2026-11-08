#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#if defined(__cplusplus) && __cplusplus < 201703L
    #error "This stack requires at least C++17"
#endif  // __cplusplus && __cplusplus < 201703L

#ifdef __arm__
    //#pragma GCC target("cpu=cortex-a9,fpu=neon-vfpv3,float-abi=hard")
#endif

#ifdef __GNUC__
    #pragma GCC diagnostic error "-Wuninitialized"
    #pragma GCC diagnostic error "-Wreturn-type"
    #pragma GCC diagnostic error "-Wfloat-equal"
    //#pragma GCC poison strcpy strcat sprintf
#endif  // __GNUC__

#include "api.h"
#include "lemlib/api.hpp"
#include "liblvgl/lvgl.h"

#include "utility/arithmetic.hpp"
#include "utility/constants.hpp"
#include "utility/static_string.hpp"

#include "robot/parameters.hpp"  // [!] Must be placed first
#include "robot/hardware.hpp"

#include "managers/autonomous_manager.hpp"
#include "managers/lifecycle_manager.hpp"  // [!] Must be placed first
#include "managers/subsystem_manager.hpp"
#include "managers/ui_manager.hpp"

#include "subsystems/subsystem.hpp"  // [!] Must be placed first
#include "subsystems/claw.hpp"
#include "subsystems/inertial.hpp"
#include "subsystems/lift.hpp"

#include "autonomous/autonomous_utility.hpp"
#include "autonomous/routine.hpp"
#include "autonomous/routines/15_sec_stack_right.hpp"

#include "operator/input_map.hpp"
#include "operator/operator_control.hpp"

#include "ui/brain_screen.hpp"
#include "ui/controller_screen.hpp"
#include "ui/ui_utility.hpp"

#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus

void autonomous(void);
void competition_initialize(void);
void disabled(void);
void initialize(void);
void opcontrol(void);

#ifdef __cplusplus
}  // extern "C"
#endif

/**
 * @brief Forces inline code expansion to eliminate function-call overhead 
 * on tight, high-frequency execution streams (e.g., vector math, filtering).
 */
#define HOT_PATH __attribute__((always_inline)) inline

/**
 * @brief Forces individual functions to compile at extreme speed optimization loops (-O3)
 * regardless of the global project toolchain configuration flags (-O2 or -Os).
 */
#define FORCE_SPEED_OPTIMIZE __attribute__((optimize("O3")))

#endif  // _PROS_MAIN_H_