//
// Created by Konstantinos Paliouras on 24/06/2022.
//

#ifndef TEST_AWS_IOT_AWSPUBSUBCLIENT_H
#define TEST_AWS_IOT_AWSPUBSUBCLIENT_H

#include <BearSSLHelpers.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "Arduino_ConnectionHandler.h"
#include "ArduinoJson.h"
#include <vector>
#include <string>
#include <FrequencyLimiter.h>


#define MQTT_DEFAULT_PORT 8883

/**
 * Client to interact with AWS Iot MQTT Service
 */
class AwsPubSubClient {
public:
    // Type definition for callback
    typedef std::function<void(const char *, const JsonDocument &)> callback_t;

protected:
    typedef std::vector<std::string> subscriptions_t;

    BearSSL::X509List *p_ca_cert;
    BearSSL::X509List *p_client_crt;
    BearSSL::PrivateKey *p_private_key;

    std::function<void(char *, uint8_t *, unsigned int)> message_received_callback;

    bool connected;
    uint8_t time_zone;
    const char *thing_name;

    WiFiClientSecure ssl_client;
    PubSubClient pub_sub_client;
    ConnectionHandler *network_connection;

    callback_t message_callback_func;

    subscriptions_t subscriptions;

    // Frequency limiters for some operations
    FrequencyLimiter mqtt_connect_loop_limiter;
    FrequencyLimiter phy_connect_loop_limiter;
    FrequencyLimiter ntp_update_loop_limiter;



    /**
     * Update time from NTP Service
     */
    bool _update_time();

    /**
     * Process incoming messages
     * @param topic The name of the topic
     * @param payload The payload
     * @param length The length
     */
    void process_received_message(char *topic, byte *payload, unsigned int length);

    /**
     *  Different states of the internal state machine
     */
    enum State {
        PhyConnect,
        TimeSync,
        MqttConnect,
        Connected,
        Disconnected,
    };

    // Internal state handlers
    State next_state;

    State handlePhyConnect();

    State handleTimeSync();

    State handleMqttConnect();

    State handleConnected();

    State handleDisconnected();

    /**
      * Initialise AWS PubSub client
      * @param _ntp_auto_update If true, the AWS pub sub client will try to perform an NTP update before connecting
      * @param _time_zone The timezone of this device
      */
    AwsPubSubClient(bool _ntp_auto_update = false, uint8_t _time_zone = 0);


public:

    /**
     * Access to singleton member
     * @return The instantiated member
     */
    static AwsPubSubClient &getInstance() {
        static AwsPubSubClient client;
        return client;
    }

    // Non-copyable
    AwsPubSubClient(AwsPubSubClient const &) = delete;
    void operator=(AwsPubSubClient const &) = delete;

    /**
     * Initialise client
     * @param _connection Handler to network connection
     * @param _thing_name The name of this thing
     * @param _ca_cert The CA Root certificate that issued all certificates
     * @param _client_crt The public part of the client certificate
     * @param _private_key The private key of the client certificate
     * @param mqtt_host The mqtt_host to connect at.
     * @param _mqtt_port The mqtt_port to connect at.
     */
    void begin(ConnectionHandler &_connection, const char *_thing_name, const char *_ca_cert, const char *_client_crt,
               const char *_private_key, const char *mqtt_host, uint16_t _mqtt_port = MQTT_DEFAULT_PORT);


    /**
     * set the callback function that will process message documents
     * @param callback_func
     */
    void set_message_callback(const callback_t &callback_func) {
        message_callback_func = callback_func;
    }


    /**
     * Check if connection is still alive
     * @return
     */
    bool is_connected() {
        if (connected) {
            if (!pub_sub_client.connected())
                connected = false; // We got disconnected
        }
        return connected;
    }

    /**
     * Publish message
     */
    bool publish(const char *topic, const char *payload) {
        return get_pub_sub_client().publish(topic, payload);
    }

    /**
     * Subscribe to a topic and subscription will be preserved across reconnections
     * @param The name of the topic
     */
    void add_subscription(const char *topic) {
        subscriptions.push_back(std::string(topic));
    }

    /**
     * Get access to native pub sub client.
     * @return
     */
    PubSubClient &get_pub_sub_client() {
        return pub_sub_client;
    }

    /**
     * Perform in loop process
     */
    void loop();

};


#endif //TEST_AWS_IOT_AWSPUBSUBCLIENT_H
