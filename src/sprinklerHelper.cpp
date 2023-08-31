// ------------------------------------------------------------------------------------------------
// Include section
// ------------------------------------------------------------------------------------------------

#include "sprinklerHelper.h"
#include "constants.h"
#include "rtcHelper.h"
#include <LibPrintf.h>

namespace sprinkler
{
    // ------------------------------------------------------------------------------------------------
    // Definition of sprinkler functions
    // ------------------------------------------------------------------------------------------------

    void initValves()
    {
        printf("Initializing valves\n");
        for (uint i = 0; i < 3; i++)
        {
            pinMode(VALVE_GPIO_OUTPUTS[i], OUTPUT);
        }
    }

    void changeValveState(ValveNumber valve, bool state)
    {
        printf("Changin valve: %i state\n", (uint8_t)valve + 1);
        digitalWrite(VALVE_GPIO_OUTPUTS[(uint8_t)valve], state);
    }

    void sprinkle()
    {
        static unsigned long sprinklerMillis = millis();
        static SprinklerState currentState = SprinklerState::IDLE;
        static uint8_t currentValve = 0;
        static bool valveActivated = false;

        switch (currentState)
        {
        case SprinklerState::IDLE:
        {
            // printf("Is sprinkle day: %i and is time to sprinkle: %i \n", isSprinkleDay(), isSprinkleTime());
            if (isSprinkleDay() && isSprinkleTime())
            {
                currentValve = 0;
                sprinklerMillis = millis();
                printf("Changing sprinkle state to activating valves\n");
                currentState = SprinklerState::ACTIVATINGVALVES;
            }
            break;
        }
        case SprinklerState::ACTIVATINGVALVES:
        {
            if (currentValve >= NUM_VALVES)
            {
                sprinklerMillis = millis();
                printf("Changing sprinkle state to waiting\n");
                currentState = SprinklerState::WAITING;
                break;
            }

            ValveNumber valve = static_cast<ValveNumber>(currentValve);
            if (!isValveEnabled(valve))
            {
                currentValve++;
                break;
            }

            if (valveActivated && millis() - sprinklerMillis >= rtc::readValueFromRTCMem(MemValue::SPRINKLERDURATION) * 60 * 1000)
            {
                changeValveState(valve, false);
                valveActivated = false;
                break;
            }

            changeValveState(valve, true);
            sprinklerMillis = millis();
            break;
        }
        case SprinklerState::WAITING:
        default:
            printf("Changing sprinkle state to idle\n");
            currentState = SprinklerState::IDLE;
            break;
        }
    }

    bool isSprinkleDay()
    {
        uint8_t dayStatus = rtc::readValueFromRTCMem(MemValue::WEEKDAYS);
        uint8_t mask = 1 << rtc::dayOfWeek(); // Create a mask with the bit corresponding to the target day set to 1
        return (dayStatus & mask) != 0;
    }

    bool isSprinkleTime()
    {
        uint8_t currentHour = rtc::RTC.GetDateTime().Hour();
        uint8_t currentMinute = rtc::RTC.GetDateTime().Minute();

        for (uint i = 0; i < 4; i++)
        {
            uint16_t timeInfo = (rtc::readValueFromRTCMem(MemValue::SPRINKLERHOURS) >> (16 * i)) & 0xFFFF;
            bool isEnabled = (timeInfo & 0x8000) != 0;
            uint8_t enabledHour = (timeInfo >> 10) & 0x1F;
            uint8_t enabledMinute = timeInfo & 0x3FF;

            if (isEnabled && currentHour == enabledHour && currentMinute == enabledMinute)
            {
                return true;
            }
        }
        return false;
    }

    bool isValveEnabled(ValveNumber currentValve)
    {
        uint8_t valveStatus = rtc::readValueFromRTCMem(MemValue::SPRINKLERVALVES);
        uint8_t mask = 1 << (uint8_t)currentValve; // Create a mask with the bit corresponding to the target valve set to 1
        return (valveStatus & mask) != 0;          // Perform bitwise AND to check if bit is set
    }
}