// ------------------------------------------------------------------------------------------------
// Include section
// ------------------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <LibPrintf.h>
#include "wifiHelper.h"
#include "constants.h"

namespace wifi
{

    // ------------------------------------------------------------------------------------------------
    // Definition of constants for wifi access point
    // ------------------------------------------------------------------------------------------------

    IPAddress local_IP(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    // ------------------------------------------------------------------------------------------------
    // Definition of wifi functions
    // ------------------------------------------------------------------------------------------------

    void startApMode()
    {
        WiFi.softAPConfig(local_IP, gateway, subnet);
        WiFi.softAP(AP_SSID, AP_PASSWORD);
    }

    void initWifi()
    {
        printf("Initializing WiFi...");
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASS);

        printf("Connecting to WiFi ..");

        /*int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10)
        {
            printf(".");
            delay(500);
            attempts++;
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            printf("\nFailed to connect to WIFI. Enabling Access Point.\n");
            startApMode();
        }
        else
        {
            printf("\nSuccessfully connected to WIFI\n");
        }*/
        while (WiFi.status() != WL_CONNECTED)
        {
            printf(".");
            delay(500);
        }

        printf("\nSuccessfully connected to WiFi\n");
    }
}