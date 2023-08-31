#ifndef WEBHELPER_H_
#define WEBHELPER_H_

// ------------------------------------------------------------------------------------------------
// Include section
// ------------------------------------------------------------------------------------------------

#include <ESPAsyncWebServer.h>
#include "constants.h"

// ------------------------------------------------------------------------------------------------
// Webserver section
// ------------------------------------------------------------------------------------------------

namespace webserver
{
    // ------------------------------------------------------------------------------------------------
    // Declaration of webserver constants
    // ------------------------------------------------------------------------------------------------

    extern AsyncWebServer server;

    // ------------------------------------------------------------------------------------------------
    // Declaration of webserver functions
    // ------------------------------------------------------------------------------------------------

}

// ------------------------------------------------------------------------------------------------
// Websocket section
// ------------------------------------------------------------------------------------------------

namespace websocket
{
    // ------------------------------------------------------------------------------------------------
    // Declaration of websocket constants
    // ------------------------------------------------------------------------------------------------

    extern AsyncWebSocket ws;
    extern String receivedMsg;

    // ------------------------------------------------------------------------------------------------
    // Declaration of websocket functions
    // ------------------------------------------------------------------------------------------------

    extern void initWebSocket();
    extern void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                        void *arg, uint8_t *data, size_t len);
    extern void notifyClients();
    extern void handleWebSocketMessage(AsyncWebSocket *server, AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len);
    extern void handleClientMessage(String clientMessage);
}

#endif