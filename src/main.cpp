#include <Arduino.h>
#include <DoorGuardUi.h>
#include<DoorLockSensor.h>
#include <LightsDetector.h>
#include <SongPlayer.h>
#include <DefaultSongsLibrary.h>
#include <PinLayout.h>
#include <BlinkControl.h>
#include <StateMachine.h>
#include <OneButton.h>
#include <FrequencyLimiter.h>
#include <AwsPubSubClient.h>
#include <ArduinoJson.h>
#include "Credentials.h"

// 5 Seconds for a person to unlock the door and then enter should be safe
#define UNLOCK_GRACE_TIME_MS 15000

// The distance measurement is by default around 80, and it seems that at close
// distances the sensor flirts around 40. So 50 should be safe.
#define LOCK_SENSOR_DISTANCE_THRESHOLD 50

// Luminance in dark is around 4, 50 should be safe.
#define LIGHT_SENSOR_LUMINANCE_THRESHOLD 80

DoorGuardUi ui;
DoorLockSensor lockSensor(LOCK_SENSOR_DISTANCE_THRESHOLD);
LightsDetector lightsDetector(LIGHT_SENSOR_LUMINANCE_THRESHOLD);

SongPlayer internal_player(INTERNAL_BUZZER_PIN);
SongPlayer external_player(EXTERNAL_BUZZER_PIN);
DefaultSongsLibrary song_library;
OneButton debugButton(DEBUG_BUTTON_PIN, false, true);

BlinkControl happyFaceLed(HAPPY_FACE_LED_PIN);
BlinkControl lockerLed(LOCKER_LED_PIN);

StateMachine machine = StateMachine();

// Setup WiF and cloud connection handler
WiFiConnectionHandler wifi_connection(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
AwsPubSubClient &aws_client = AwsPubSubClient::getInstance();


// Prototypes
void handle_locked_state();

void handle_unlocked_unattended_state();

void handle_unlocked_occupied_state();

// Configure State machine
State *lockedState = machine.addState(&handle_locked_state);
State *unlockedUnattendedState = machine.addState(&handle_unlocked_unattended_state);
State *unlockedOccupiedState = machine.addState(&handle_unlocked_occupied_state);


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
//    initProperties();

    // Connect to IoT Cloud
    Serial.println("Initialising arduino cloud.");
//    ArduinoCloud.begin(wifi_connection);
    aws_client.begin(wifi_connection,
                     THINGNAME, ca_cert_str, client_cert_str,
                     priv_key_str, MQTT_HOST);
    Serial.println("Arduino cloud initialised.");

    // State machine
    Serial.println("Configure state machine.");
    lockedState->addTransition([]() {
        return ((!lockSensor.is_locked()) && (lightsDetector.is_turned_on()));
    }, unlockedOccupiedState);
    lockedState->addTransition([]() {
        return ((!lockSensor.is_locked())
        && (lockSensor.state_unchanged_time_ms() > UNLOCK_GRACE_TIME_MS)
        && (!lightsDetector.is_turned_on()));
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

/**
 * Publish status on the cloud
 * @param state
 */
void publish_status() {
    StaticJsonDocument<200> json_doc;
    DynamicJsonDocument shadow_doc(512);
    char jsonBuffer[512];

    if (machine.stateList->get(machine.currentState) == lockedState){
        json_doc["state"] = "locked";
    } else if (machine.stateList->get(machine.currentState) == unlockedOccupiedState) {
        json_doc["state"] = "occupied";
    } else if (machine.stateList->get(machine.currentState) == unlockedUnattendedState) {
        json_doc["state"] = "unattended";
    } else {
        json_doc["state"] = "unknown";
    }

    json_doc["lights_sensor"] = lightsDetector.get_raw_value();
    json_doc["lights_on"] = lightsDetector.is_turned_on();
    json_doc["door_sensor"] = lockSensor.get_raw_distance();
    json_doc["door_locked"] = lockSensor.is_locked();

    serializeJson(json_doc, jsonBuffer);
    aws_client.publish(MQTT_PUB_STATUS_TOPIC, jsonBuffer);

    shadow_doc.createNestedObject("state");
    shadow_doc["state"]["reported"] = json_doc;

    serializeJson(shadow_doc, jsonBuffer);
    aws_client.publish(MQTT_SHADOW_STATUS_UPDATE_TOPIC, jsonBuffer);
}

void handle_locked_state() {
    if(machine.executeOnce){
        Serial.println("We are in locked state.");
        lockerLed.on();
        happyFaceLed.off();
        internal_player.stop();
        external_player.set_auto_restart(false);
        external_player.play(song_library.get_song(DefaultSongsLibrary::Songs::LOCKED_BEEP));

        // Update cloud status
        publish_status();
    }
}

void handle_unlocked_unattended_state() {
    if(machine.executeOnce){
        Serial.println("We are in unattended state.");
        lockerLed.blink2();
        happyFaceLed.off();

        // Setup sound
        internal_player.set_auto_restart(true);
        internal_player.stop();
        external_player.set_auto_restart(true);
        external_player.play(song_library.get_song(DefaultSongsLibrary::Songs::PERSISTENT_BEEP));

        // Update cloud status
        publish_status();
    }
}

void handle_unlocked_occupied_state() {
    if(machine.executeOnce){
        Serial.println("We are in occupied state.");
        lockerLed.off();
        happyFaceLed.on();

        // Sound
        internal_player.set_auto_restart(false);
        internal_player.play(song_library.get_song(DefaultSongsLibrary::Songs::IMPERIAL_MARCH));

        external_player.stop();

        //  Update cloud status
        publish_status();
    }
}

FrequencyLimiter readSensorsLimiter(PeriodMs(500));
FrequencyLimiter updateCloudLimiter(PeriodMs(2000));
FrequencyLimiter cloudPublishStatusLimiter(PeriodMs(30 * 60 * 1000)); // Log status anyway every 10 minutes
FrequencyLimiter fpsPrinterLimiter(PeriodMs(500));
unsigned long last_time = millis();

void loop() {

    if (readSensorsLimiter.process()) {
        Serial.println("Reading sensors.");
        lockSensor.update();
        lightsDetector.update();
    }
    machine.run();
    lockerLed.loop();
    happyFaceLed.loop();
    debugButton.tick();


    if(updateCloudLimiter.process()){
        Serial.println("Updating cloud.");
        aws_client.loop();
    }

    if (cloudPublishStatusLimiter.process()) {
        Serial.println("Updating status in the cloud");
        publish_status();
    }

    // Update UI
    ui.update_wifi_status(wifi_connection.check());
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
    if (fpsPrinterLimiter.process()) {
        Serial.printf("FPS: %f\n", fps);
    }
}

void onLockDistanceChange() {
    // This is an event when this is triggered by iot
    // NOOP
}
