#pragma once

#ifndef SPRINKLERHELPER_H_
#define SPRINKLERHELPER_H_

// ------------------------------------------------------------------------------------------------
// Include section
// ------------------------------------------------------------------------------------------------

#include <Arduino.h>

// ------------------------------------------------------------------------------------------------
// Sprinkler section
// ------------------------------------------------------------------------------------------------

namespace sprinkler
{
    // ------------------------------------------------------------------------------------------------
    // Declaration of sprinkler constants
    // ------------------------------------------------------------------------------------------------

    const unsigned int VALVE_GPIO_OUTPUTS[] = {15, 14, 12, 13}; // Should be equal to NUM_VALVES
    enum class ValveNumber : uint8_t
    {
        Valve1,
        Valve2,
        Valve3,
        Valve4
    };
    enum class SprinklerState : uint8_t
    {
        IDLE,
        ACTIVATINGVALVES,
        WAITING
    };

    // ------------------------------------------------------------------------------------------------
    // Declaration of sprinkler functions
    // ------------------------------------------------------------------------------------------------

    extern void initValves();
    extern void changeValveState(ValveNumber valve, bool state);
    extern void sprinkle();
    extern bool isSprinkleDay();
    extern bool isSprinkleTime();
    extern bool isValveEnabled(ValveNumber currentValve);
}

#endif