// ------------------------------------------------------------------------------------------------
// Include section
// ------------------------------------------------------------------------------------------------

#include "webHelper.h"
#include <LibPrintf.h>
#include "rtcHelper.h"
#include "sprinklerHelper.h"
#include "constants.h"

namespace webserver
{

  // ------------------------------------------------------------------------------------------------
  // Definition of webserver constants
  // ------------------------------------------------------------------------------------------------

  AsyncWebServer server(HTTP_PORT);

  // ------------------------------------------------------------------------------------------------
  // Definition of webserver functions
  // ------------------------------------------------------------------------------------------------

}

namespace websocket
{
  // ------------------------------------------------------------------------------------------------
  // Definition of websocket constants
  // ------------------------------------------------------------------------------------------------

  AsyncWebSocket ws("/ws");
  String receivedMsg = "";

  // ------------------------------------------------------------------------------------------------
  // Definition of websocket functions
  // ------------------------------------------------------------------------------------------------

  void initWebSocket()
  {
    printf("Initializing websocket\n");
    ws.onEvent(websocket::onEvent);
    webserver::server.addHandler(&ws);
  }

  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len)
  {
    printf("Received websocket event\n");
    if (type == WS_EVT_CONNECT)
    {
      websocket::notifyClients();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
    }
    else if (type == WS_EVT_ERROR)
    {
    }
    else if (type == WS_EVT_PONG)
    {
    }
    else if (type == WS_EVT_DATA)
    {
      handleWebSocketMessage(server, client, arg, data, len);
    }
  }

  void notifyClients()
  {
    /*DynamicJsonDocument dataForClient(1536);
    dataForClient["sprinkleDuration"] = sprinkleDuration.duration;

    dataForClient["serverTime"]["day"] = RTC.GetDateTime().Day();
    dataForClient["serverTime"]["month"] = RTC.GetDateTime().Month();
    dataForClient["serverTime"]["year"] = RTC.GetDateTime().Year();
    dataForClient["serverTime"]["hour"] = RTC.GetDateTime().Hour();
    dataForClient["serverTime"]["minute"] = RTC.GetDateTime().Minute();
    dataForClient["serverTime"]["second"] = RTC.GetDateTime().Second();

    JsonArray sprinkleHours = dataForClient.createNestedArray("sprinkleHours");
    for (int i = 0; i < NUM_SPRINKLE_HOURS; i++)
    {
      JsonObject sprinkleHour = sprinkleHours.createNestedObject();
      sprinkleHour["hour"] = SprinklerHours[i].hour;
      sprinkleHour["minute"] = SprinklerHours[i].minute;
      sprinkleHour["enabled"] = SprinklerHours[i].enabled;
    }

    JsonArray sprinkleDays = dataForClient.createNestedArray("sprinkleDays");
    for (int i = 0; i < NUM_WEEKDAYS; i++)
    {
      JsonObject sprinkleDay = sprinkleDays.createNestedObject();
      sprinkleDay["name"] = SprinklerDays[i].name;
      sprinkleDay["state"] = SprinklerDays[i].state;
    }

    JsonArray sprinkleValves = dataForClient.createNestedArray("sprinkleValves");
    for (int i = 0; i < NUM_VALVES; i++)
    {
      JsonObject sprinkleValve = sprinkleValves.createNestedObject();
      sprinkleValve["output"] = SprinklerValves[i].pin;
      sprinkleValve["state"] = SprinklerValves[i].on;
    }

    JsonArray valvesState = dataForClient.createNestedArray("valveState");
    for (int i = 0; i < NUM_VALVES; i++)
    {
      JsonObject valveState = valvesState.createNestedObject();
      valveState["output"] = Valves[i].pin;
      valveState["state"] = Valves[i].on;
    }

    char data[1536];
    size_t len = serializeJson(dataForClient, data);
    ws.textAll(data, len);*/
  }

  void handleWebSocketMessage(AsyncWebSocket *server, AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len)
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len)
    {
      // the whole message is in a single frame and we got all of it's data
      if (info->opcode == WS_TEXT)
      {
        for (size_t i = 0; i < info->len; i++)
        {
          receivedMsg += (char)data[i];
        }
      }
      handleClientMessage(receivedMsg);
      receivedMsg = "";
    }
    else
    {
      // message is comprised of multiple frames or the frame is split into multiple packets
      if (info->opcode == WS_TEXT)
      {
        for (size_t i = 0; i < len; i++)
        {
          receivedMsg += (char)data[i];
        }
      }

      // On message end
      if ((info->index + len) == info->len)
      {
        if (info->final)
        {
          handleClientMessage(receivedMsg);
          receivedMsg = "";
        }
      }
    }
  }

  void handleClientMessage(String clientMessage)
  {
    // The client message will be something like:
    // SPRINKLERHOUS::1111....(maximum 64bits payload)
    // TOPIC        ::PAYLOAD
    // the message will need to be split on "::" (removing the symbol) and the payload saved to the
    // RTC memory address for that topic

    // Find the index of the "::" delimiter
    int delimiter_index = clientMessage.indexOf("::");
    if (delimiter_index == -1)
    {
      return;
    }

    // Extract the topic and payload
    String topic = clientMessage.substring(0, delimiter_index);
    String payload = clientMessage.substring(delimiter_index + 2);

    printf("Received topic: %c and received payload: %c\n", topic, payload);

    std::map<String, ReceivedTopic> topicMap = {
        {"WEEKDAYS", ReceivedTopic::WEEKDAYS},
        {"SPRINKLERVALVES", ReceivedTopic::SPRINKLERVALVES},
        {"SPRINKLERDURATION", ReceivedTopic::SPRINKLERDURATION},
        {"SPRINKLERHOURS", ReceivedTopic::SPRINKLERHOURS},
        {"VALVES", ReceivedTopic::VALVES}};

    // Find the corresponding enum value for the topic
    if (topicMap.find(topic) != topicMap.end())
    {
      ReceivedTopic receivedTopic = topicMap[topic];

      // Use a switch case to handle different enum values
      switch (receivedTopic)
      {
      case ReceivedTopic::WEEKDAYS:
        rtc::saveValueInRTCMem(MemValue::WEEKDAYS, (uint64_t)payload.toInt());
        break;
      case ReceivedTopic::SPRINKLERVALVES:
        rtc::saveValueInRTCMem(MemValue::SPRINKLERVALVES, (uint64_t)payload.toInt());
        break;
      case ReceivedTopic::SPRINKLERDURATION:
        rtc::saveValueInRTCMem(MemValue::SPRINKLERDURATION, (uint64_t)payload.toInt());
        break;
      case ReceivedTopic::SPRINKLERHOURS:
        rtc::saveValueInRTCMem(MemValue::SPRINKLERHOURS, (uint64_t)payload.toInt());
        break;
      case ReceivedTopic::VALVES:
        uint8_t valve1Mask = 0x01; // Bit 0
        uint8_t valve2Mask = 0x02; // Bit 1
        uint8_t valve3Mask = 0x04; // Bit 2
        uint8_t valve4Mask = 0x08; // Bit 3

        if ((uint64_t)payload.toInt() & valve1Mask)
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve1, true);
        }
        else
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve1, false);
        }

        if ((uint64_t)payload.toInt() & valve2Mask)
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve2, true);
        }
        else
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve2, false);
        }

        if ((uint64_t)payload.toInt() & valve3Mask)
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve3, true);
        }
        else
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve3, false);
        }

        if ((uint64_t)payload.toInt() & valve4Mask)
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve4, true);
        }
        else
        {
          sprinkler::changeValveState(sprinkler::ValveNumber::Valve4, false);
        }
        break;
      }
    }
  }
}