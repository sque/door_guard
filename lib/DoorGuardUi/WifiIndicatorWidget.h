//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_WIFIINDICATORWIDGET_H
#define DOOR_GUARD_WIFIINDICATORWIDGET_H
#include <Widget.h>

class WifiIndicatorWidget: public Widget{
public:
    WifiIndicatorWidget(Adafruit_GFX &display, uint8_t x, uint8_t y);

    void update_status(bool is_connected, int8_t rssi_strength) {
        this->is_connected = is_connected;
        this->rssi_strength = rssi_strength;
    }

protected:
    bool is_connected;
    int8_t rssi_strength;

    void on_draw() override;

};


#endif //DOOR_GUARD_WIFIINDICATORWIDGET_H
