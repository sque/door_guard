//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_DOORGUARDUI_H
#define DOOR_GUARD_DOORGUARDUI_H

#include <Adafruit_SSD1306_Wemos.h>
#include <ESP8266WiFi.h>
#include <Arduino_ConnectionHandler.h>
#include <DoorLockSensor.h>
#include <WifiIndicatorWidget.h>
#include <GhostAnimation.h>

#define OLED_RESET_PIN 0


class DoorGuardUi {
private:
    Adafruit_SSD1306_Wemos display;

    NetworkConnectionState connection_state = NetworkConnectionState::INIT;
    int8_t wifi_strength;
    bool is_locked;
    bool _is_debug_info_displayed;
    uint16_t lock_sensor_range;
    WifiIndicatorWidget wifi_widget;
    GhostAnimation front_animation_widget;
    bool is_lights_on;
    int light_sensor_value;
    int current_state_index;

public:

    DoorGuardUi(uint8_t w, uint8_t h, bool wemos_oled = false) :
            display(w, h, wemos_oled, &Wire, OLED_RESET_PIN),
            wifi_widget(display, display.width() - 11, 0),
            front_animation_widget(display, 10, 10) {

        // Start yoda animation
        front_animation_widget.set_loop_mode(true);
        front_animation_widget.start();
    }

    void begin(uint8_t i2caddr = 0x3C) {
        display.begin(SSD1306_SWITCHCAPVCC, i2caddr, true);
        display.display();
        Serial.printf("UI: Initialised display [%dx%d] Wemos: %d\n",display.width(), display.height(), display.is_wemos_oled());

    }

    /**
     * Function to be called in the loop
     */
    void refresh() {
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
            display.printf("Lamp %d|%d", this->is_lights_on, this->light_sensor_value);
            display.setCursor(0, 37);
            display.printf("State %d", this->current_state_index);
        } else {
            // Draw yoda
            front_animation_widget.draw();
        }

        display.display();
    }

    /**
     * Set connection status
     */
    void update_wifi_status(const NetworkConnectionState &new_state) {
        wifi_widget.update_status(new_state == NetworkConnectionState::CONNECTED, WiFi.RSSI());
        connection_state = new_state;
        wifi_strength = WiFi.RSSI();
    }

    /**
     * Update data for lock state.
     * @param is_locked
     * @param lock_sensor_range
     */
    void update_lock_state(bool is_locked, uint16_t lock_sensor_range) {
        this->is_locked = is_locked;
        this->lock_sensor_range = lock_sensor_range;
    }

    /**
     * Update data from light sensor.
     * @param is_turned_on
     * @param last_value
     */
    void update_light_sensor(bool is_turned_on, int last_value) {
        this->is_lights_on = is_turned_on;
        this->light_sensor_value = last_value;
    }

    /**
     * Update the index of the current state
     * @param state_index The index of the selected state
     */
    void update_current_state(int state_index) {
        current_state_index = state_index;
    }

    /**
     * Display a fatal error and block forever.
     * @param msg The message to display.
     */
    void display_fatal_error(const char *msg) {

        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.println("FATAL ERR");
        display.drawFastHLine(0, 8, display.width(), WHITE);

        display.setCursor(2, 10);
        display.setTextSize(1);
        display.println(msg);
        display.display();
        while (1);
    }


    /**
     * Switch the display of debug information
     * @param debug_flag If true debug info will be displayed
     */
    void switch_debug_info(bool debug_flag) {
        _is_debug_info_displayed = debug_flag;
    }

    /**
     * Get the current state of debug info
     */
    bool is_debug_info_displayed() const {
        return _is_debug_info_displayed;
    }


};


#endif //DOOR_GUARD_DOORGUARDUI_H
