#pragma once

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// ------------------------------------------------------------------------------------------------
// Definition of macros
// ------------------------------------------------------------------------------------------------

#define NUM_SPRINKLE_HOURS 4
#define NUM_VALVES 4
#define NUM_WEEKDAYS 7
#define HTTP_PORT 80
#define RTC_DAT 4
#define RTC_CLK 5
#define RTC_RES 2

#define countof(a) (sizeof(a) / sizeof(a[0]))

// ------------------------------------------------------------------------------------------------
// Declaration of global constants
// ------------------------------------------------------------------------------------------------

enum class MemValue
{
    WEEKDAYS,
    SPRINKLERVALVES,
    SPRINKLERDURATION,
    SPRINKLERHOURS,
};

enum class ReceivedTopic
{
    WEEKDAYS,
    SPRINKLERVALVES,
    SPRINKLERDURATION,
    SPRINKLERHOURS,
    VALVES
};

// ------------------------------------------------------------------------------------------------
// Declaration of wifi constants
// ------------------------------------------------------------------------------------------------

extern const char *WIFI_SSID;
extern const char *WIFI_PASS;
extern const char *AP_SSID;
extern const char *AP_PASSWORD;

#endif