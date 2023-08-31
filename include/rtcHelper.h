#ifndef RTCHELPER_H_
#define RTCHELPER_H_

// ------------------------------------------------------------------------------------------------
// Include section
// ------------------------------------------------------------------------------------------------

#include <RtcDS1302.h>
#include "constants.h"

// ------------------------------------------------------------------------------------------------
// RTC section
// ------------------------------------------------------------------------------------------------
namespace rtc
{
    // ------------------------------------------------------------------------------------------------
    // Declaration of rtc constants
    // ------------------------------------------------------------------------------------------------

    extern ThreeWire rtcWire;
    extern RtcDS1302<ThreeWire> RTC;

    // ------------------------------------------------------------------------------------------------
    // Declaration of rtc functions
    // ------------------------------------------------------------------------------------------------

    void initRTC(void);
    void saveValueInRTCMem(MemValue type, uint64_t value);
    uint64_t readValueFromRTCMem(MemValue type);
    uint8_t dayOfWeek(void);
}

#endif