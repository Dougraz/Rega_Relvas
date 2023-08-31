// ------------------------------------------------------------------------------------------------
// Include section
// ------------------------------------------------------------------------------------------------

#include "rtcHelper.h"
#include <LibPrintf.h>

namespace rtc
{

  // ------------------------------------------------------------------------------------------------
  // Definition of rtc constants
  // ------------------------------------------------------------------------------------------------

  ThreeWire rtcWire(RTC_DAT, RTC_CLK, RTC_RES);
  RtcDS1302<ThreeWire> RTC(rtcWire);

  /*ThreeWire rtcWire(RTC_DAT, RTC_CLK, RTC_RES);
  RtcDS1302<ThreeWire> RTC(rtcWire);*/

  // ------------------------------------------------------------------------------------------------
  // Definition of rtc functions
  // ------------------------------------------------------------------------------------------------

  void initRTC()
  {
    printf("Initializing RTC...");
    RTC.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (!RTC.IsDateTimeValid())
    {
      RTC.SetDateTime(compiled);
    }

    if (RTC.GetIsWriteProtected())
    {
      RTC.SetIsWriteProtected(false);
    }

    if (!RTC.GetIsRunning())
    {
      RTC.SetIsRunning(true);
    }
  }

  void saveValueInRTCMem(MemValue type, uint64_t value)
  {
    switch (type)
    {
    case MemValue::WEEKDAYS:
    {
      const uint8_t address = 0;
      RTC.SetMemory(address, (uint8_t)value);
      break;
    }
    case MemValue::SPRINKLERVALVES:
    {
      const uint8_t address = 1;
      RTC.SetMemory(address, (uint8_t)value);
      break;
    }
    case MemValue::SPRINKLERDURATION:
    {
      const uint8_t address = 2;
      RTC.SetMemory(address, (uint8_t)value);
      break;
    }
    case MemValue::SPRINKLERHOURS:
    {
      // Function for handling the save of all 4 sprinkler hours at the same time
      // each sprinkler hour curresponds to a uint16_t value with:
      //    Bit15: Enable/Disable (1bit)
      //    Bit14-10: Hours (5bits)
      //    Bit9-0: Minutes (10bits)
      // As the ds1302 rtc only accepts uint8_t values each uint16_t sprinklerHour gets converted to
      // uint8_t sprinklerHourHigh and uint8_t sprinklerHourLow.

      const uint8_t baseHourAddress = 4;
      for (uint i = 8; i > 1; i--)
      {
        const uint8_t SprinklerHour = (value >> (8 * (i - 1))) & 0xFFFF;
        RTC.SetMemory(baseHourAddress + i - 1, SprinklerHour);
      }
      break;
    }
    }
  }

  uint64_t readValueFromRTCMem(MemValue type)
  {
    switch (type)
    {
    case MemValue::WEEKDAYS:
    {
      const uint8_t address = 0;
      return (uint64_t)RTC.GetMemory(address);
      break;
    }
    case MemValue::SPRINKLERVALVES:
    {
      const uint8_t address = 1;
      return (uint64_t)RTC.GetMemory(address);
      break;
    }
    case MemValue::SPRINKLERDURATION:
    {
      const uint8_t address = 2;
      return (uint64_t)RTC.GetMemory(address);
      break;
    }
    case MemValue::SPRINKLERHOURS:
    {
      // Function for handling the recall of all 4 sprinkler hours at the same time
      // each sprinkler hour curresponds to a uint16_t value with:
      //    Bit15: Enable/Disable (1bit)
      //    Bit14-10: Hours (5bits)
      //    Bit9-0: Minutes (10bits)
      // As the ds1302 rtc only accepts uint8_t values each uint16_t sprinklerHour is converted from
      // uint8_t sprinklerHourHigh and uint8_t sprinklerHourLow.

      const uint8_t baseHourAddress = 4;
      uint64_t SprinkleHours = 0;
      for (uint i = 8; i > 1; i--)
      {
        SprinkleHours = RTC.GetMemory(baseHourAddress + i - 1) << (8 * (i - 1)) & 0xFFFF;
      }

      return SprinkleHours;
      break;
    }
    default:
      return -1;
    }
  }

  uint8_t dayOfWeek()
  {
    return RTC.GetDateTime().DayOfWeek();
  }
}