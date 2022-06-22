//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_DOORGUARDUI_H
#define DOOR_GUARD_DOORGUARDUI_H

#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <Arduino_ConnectionHandler.h>
#include <DoorLockSensor.h>
#include <WifiIndicatorWidget.h>
#include <YodaAnimation.h>

#define OLED_RESET 0


class   DoorGuardUi {
private:
    Adafruit_SSD1306 display;

    NetworkConnectionState connection_state = NetworkConnectionState::INIT;
    int8_t wifi_strength;
    bool is_locked;
    bool _is_debug_info_displayed;
    uint16_t lock_sensor_range;
    WifiIndicatorWidget wifi_widget;
    YodaAnimation yoda_animation_widget;
    bool is_lights_on;
    int light_sensor_value;
    int current_state_index;

public:

    DoorGuardUi();

    void begin(uint8_t i2caddr = 0x3C);

    /**
     * Function to be called in the loop
     */
    void refresh();

    /**
     * Set connection status
     */
     void update_wifi_status(NetworkConnectionState const &new_state);

     /**
      * Update data for lock state.
      * @param is_locked
      * @param lock_sensor_range
      */
     void update_lock_state(bool is_locked, uint16_t lock_sensor_range);

     /**
      * Update data from light sensor.
      * @param is_turned_on
      * @param last_value
      */
     void update_light_sensor(bool is_turned_on, int last_value);

     /**
      * Update the index of the current state
      * @param state_index The index of the selected state
      */
      void update_current_state(int state_index);

     /**
      * Display a fatal error and block forever.
      * @param msg The message to display.
      */
     void display_fatal_error(const char * msg);


     /**
      * Switch the display of debug information
      * @param debug_flag If true debug info will be displayed
      */
     void switch_debug_info(bool debug_flag);

     /**
      * Get the current state of debug info
      */
      bool is_debug_info_displayed() const{
          return _is_debug_info_displayed;
      }


};


#endif //DOOR_GUARD_DOORGUARDUI_H
