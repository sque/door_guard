//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#include <ESP8266WiFi.h>
#include "DoorGuardUi.h"


DoorGuardUi::DoorGuardUi():
        display(OLED_RESET),
        wifi_widget(display, display.width() - 11, 0),
        yoda_animation_widget(display, 10, 10){

    // Start yoda animation
    yoda_animation_widget.set_loop_mode(true);
    yoda_animation_widget.start();
}

void DoorGuardUi::begin(uint8_t i2caddr) {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void DoorGuardUi::refresh() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Draw the Wifi connection
    wifi_widget.draw();

    if (_is_debug_info_displayed) {
        display.setCursor(0, 10);
        display.printf("WiFi %d", wifi_strength);

        display.setCursor(0, 19);
        display.printf("Door %d|%d", this->is_locked, this->lock_sensor_range);

        display.setCursor(0, 28);
        display.printf("Lamp %d|%d",this->is_lights_on, this->light_sensor_value);
        display.setCursor(0, 37);
        display.printf("State %d",this->current_state_index);
    } else {
        // Draw yoda
        yoda_animation_widget.draw();
    }

    display.display();
}

void DoorGuardUi::update_wifi_status(const NetworkConnectionState &new_state) {
    wifi_widget.update_status(new_state == NetworkConnectionState::CONNECTED, WiFi.RSSI());
    connection_state = new_state;
    wifi_strength = WiFi.RSSI();
}

void DoorGuardUi::display_fatal_error(const char *msg) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println("FATAL ERR");
    display.drawFastHLine(0,8, display.width(), WHITE);

    display.setCursor(2, 10);
    display.setTextSize(1);
    display.println(msg);
    display.display();
    while(1);
}

void DoorGuardUi::update_lock_state(bool is_locked, uint16_t lock_sensor_range) {
    this->is_locked = is_locked;
    this->lock_sensor_range = lock_sensor_range;
}

void DoorGuardUi::switch_debug_info(bool debug_flag) {
    _is_debug_info_displayed = debug_flag;
}

void DoorGuardUi::update_light_sensor(bool is_turned_on, int last_value) {
    this->is_lights_on = is_turned_on;
    this->light_sensor_value = last_value;
}

void DoorGuardUi::update_current_state(int state_index) {
    current_state_index = state_index;
}




