//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#include "WifiIndicatorWidget.h"
#define WHITE 1

void WifiIndicatorWidget::on_draw() {
    uint8_t r = width/2;
    display.drawCircle(x + r, y + r, r, WHITE);

    if (!is_connected) {
        display.drawLine(x, y, x+width, y+width, WHITE);
    } else {
        // The level is 5 scale between -80dbm and -50dbm
        const int16_t best_rssi = -55;
        const int16_t worst_rssi = -85;
        int16_t signal_level = max(min(rssi_strength, (int8_t)best_rssi), (int8_t)worst_rssi);
        signal_level = (signal_level - worst_rssi) * 5 / (best_rssi - worst_rssi);
//        display.setCursor(0,40);
//        display.setTextSize(1);
//        display.setTextColor(WHITE);
//        display.printf("%d %d",rssi_strength, signal_level);

        for(int8_t i=1; i<= signal_level;i++){
            display.drawCircle(x + r, y + r, r-i, WHITE);
        }
    }

}

WifiIndicatorWidget::WifiIndicatorWidget(Adafruit_GFX &display, uint8_t x, uint8_t y):
        Widget(display, x, y, 10, 10),
        is_connected(false),
        rssi_strength(0){

}
