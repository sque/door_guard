//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_ARDUINOCLOUDCONFIGURATION_H
#define DOOR_GUARD_ARDUINOCLOUDCONFIGURATION_H

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <Credentials.h>

const char DEVICE_LOGIN_NAME[]  = SECRET_IOT_CLOUD_DEVICE_NAME;

const char SSID[]               = SECRET_WIFI_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_WIFI_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_IOT_CLOUD_DEVICE_KEY;    // Secret device password

void onLockDistanceChange();

float lockDistance;
CloudContactSensor lockedDoor;
CloudLight kellerLights;

void initProperties(){

    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
    ArduinoCloud.addProperty(lockDistance, READWRITE, ON_CHANGE, onLockDistanceChange);
    ArduinoCloud.addProperty(lockedDoor, READ, ON_CHANGE, NULL);
    ArduinoCloud.addProperty(kellerLights, READ, ON_CHANGE, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
#endif //DOOR_GUARD_ARDUINOCLOUDCONFIGURATION_H
