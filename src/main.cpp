#include <Arduino.h>
#include <ArduinoCloudConfiguration.h>
#include <DoorGuardUi.h>
#include<DoorLockSensor.h>
#include <LightsDetector.h>
#include <SongPlayer.h>
#include <DefaultSongsLibrary.h>
#include <PinLayout.h>
#include <BlinkControl.h>
#include <StateMachine.h>
#include <OneButton.h>
#include "FixedFrequency.h"

DoorGuardUi ui;
DoorLockSensor lockSensor(40);
LightsDetector lightsDetector(100);

SongPlayer internal_player(INTERNAL_BUZZER_PIN);
SongPlayer external_player(EXTERNAL_BUZZER_PIN);
DefaultSongsLibrary song_library;
OneButton debugButton(DEBUG_BUTTON_PIN, false, true);

BlinkControl happyFaceLed(HAPPY_FACE_LED_PIN);
BlinkControl lockerLed(LOCKER_LED_PIN);

StateMachine machine = StateMachine();

// Prototypes
void locked_state();

void unlocked_unattended_state();

void unlocked_occupied_state();

// Configure State machine
State *lockedState = machine.addState(&locked_state);
State *unlockedUnattendedState = machine.addState(&unlocked_unattended_state);
State *unlockedOccupiedState = machine.addState(&unlocked_occupied_state);


void setup() {
    Serial.begin(9600);

    // Setup UI
    Serial.println("Starting UI...");
    ui.begin();
//    ui.switch_debug_info(true);
    Serial.println("UI Started.");

    // Setup door sensor
    Serial.println("Connecting to sensor...");
    if (!lockSensor.begin()) {
        ui.display_fatal_error("Cannot initialise lock sensor.");
    }
    Serial.println("Connected.");

    // Setup music song_player
    Serial.println("Initialising music song_player...");
    internal_player.set_auto_restart(false);
    external_player.set_auto_restart(true);
    Serial.println("Music song_player initialised.");

    // Setup IOT connection
    initProperties();

    // Connect to Arduino IoT Cloud
    Serial.println("Initialising arduino cloud.");
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    Serial.println("Arduino cloud initialised.");

    // State machine
    Serial.println("Configure state machine.");
    lockedState->addTransition([]() {
        return ((!lockSensor.is_locked()) && (lightsDetector.is_turned_on()));
    }, unlockedOccupiedState);
    lockedState->addTransition([]() {
        return ((!lockSensor.is_locked()) && (!lightsDetector.is_turned_on()));
    }, unlockedUnattendedState);
    unlockedOccupiedState->addTransition([]() {
        return ((!lockSensor.is_locked()) && (!lightsDetector.is_turned_on()));
    }, unlockedUnattendedState);
    unlockedOccupiedState->addTransition([]() {
        return (lockSensor.is_locked());
    }, lockedState);
    unlockedUnattendedState->addTransition([]() {
        return ((!lockSensor.is_locked()) && (lightsDetector.is_turned_on()));
    }, unlockedOccupiedState);
    unlockedUnattendedState->addTransition([]() {
        return (lockSensor.is_locked());
    }, lockedState);

    // Setup debug button
    debugButton.attachClick([](){
        Serial.println("Debug button was pressed!");
        ui.switch_debug_info(!ui.is_debug_info_displayed());
    });
}


void locked_state() {
    if(machine.executeOnce){
        Serial.println("We are in locked state.");
        lockerLed.on();
        happyFaceLed.off();
        internal_player.stop();
        external_player.stop();
    }
}

void unlocked_unattended_state() {
    if(machine.executeOnce){
        Serial.println("We are in unattended state.");
        lockerLed.blink2();
        happyFaceLed.off();

        // Setup sound
        internal_player.set_auto_restart(true);
        internal_player.stop();
        external_player.set_auto_restart(true);
        external_player.play(song_library.get_song(DefaultSongsLibrary::Songs::UGLY_ALARM));
    }
}

void unlocked_occupied_state() {
    if(machine.executeOnce){
        Serial.println("We are in occupied state.");
        lockerLed.off();
        happyFaceLed.on();

        // Sound
        internal_player.set_auto_restart(false);
        internal_player.play(song_library.get_song(DefaultSongsLibrary::Songs::IMPERIAL_MARCH));
        external_player.stop();
    }
}

FixedFrequency sensorsFrequency(2);
FixedFrequency cloudFrequency = FixedFrequency::from_period(2000);

unsigned long last_time = millis();

void loop() {

    if (sensorsFrequency.process()) {
        Serial.println("Reading sensors.");
        lockSensor.update();
        lightsDetector.update();
    }
    machine.run();
    lockerLed.loop();
    happyFaceLed.loop();
    debugButton.tick();

    // Update arduino
    lockDistance = lockSensor.get_raw_distance();
    lockedDoor = lockSensor.is_locked();
    kellerLights = lightsDetector.is_turned_on();
    if(cloudFrequency.process()){
        Serial.println("Updating arduino cloud.");
        ArduinoCloud.update();
    }

    // Update UI
    ui.update_wifi_status(ArduinoCloud.getConnection()->check());
    ui.update_lock_state(lockSensor.is_locked(), lockSensor.get_raw_distance());
    ui.update_light_sensor(lightsDetector.is_turned_on(), lightsDetector.get_raw_value());
    ui.update_current_state(machine.currentState);
    ui.refresh();

    // Update Music
    internal_player.refresh();
    external_player.refresh();

    // Calculate fps
    unsigned long current_time = millis();
    float fps = 1000.0 / (float)(current_time - last_time);
    last_time = current_time;
    Serial.printf("FPS: %f\n", fps);
}

void onLockDistanceChange() {
    // This is an event when this is triggered by iot
    // NOOP
}
