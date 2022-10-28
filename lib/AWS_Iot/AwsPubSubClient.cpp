//
// Created by Konstantinos Paliouras on 24/06/2022.
//

#define SSL_ERROR_MSG_SIZE 256

#include "AwsPubSubClient.h"


AwsPubSubClient::AwsPubSubClient(bool _ntp_auto_update, uint8_t _time_zone) :
        connected(false),
        time_zone(_time_zone),
        pub_sub_client(ssl_client),
        next_state(State::PhyConnect),
        mqtt_connect_loop_limiter(PeriodMs(2000)),
        phy_connect_loop_limiter(PeriodMs(500)),
        ntp_update_loop_limiter(PeriodMs(100)){
}




void AwsPubSubClient::begin(ConnectionHandler & _connection,
                            const char *_thing_name,
                            const char *_ca_cert, const char *_client_crt, const char *_private_key,
                            const char *_mqtt_host, uint16_t _mqtt_port) {
    network_connection = & _connection;
    p_ca_cert = new BearSSL::X509List(_ca_cert);
    p_client_crt = new BearSSL::X509List(_client_crt);
    p_private_key = new BearSSL::PrivateKey(_private_key);
    thing_name = _thing_name;

    // Setup SSL connection here
    ssl_client.setTrustAnchors(p_ca_cert);
    ssl_client.setClientRSACert(p_client_crt, p_private_key);

    pub_sub_client.setServer(_mqtt_host, _mqtt_port);
    // Register callback to the actual
    pub_sub_client.setCallback([](char *topic, byte *payload, unsigned int length){
        AwsPubSubClient::getInstance().process_received_message(topic, payload, length);
    });
}


bool AwsPubSubClient::_update_time() {
    time_t now;
    time_t nowish = 1510592825;

    configTime(time_zone * 3600, 0 * 3600,
               "pool.ntp.org", "time.nist.gov");
    now = time(nullptr);
    if (now < nowish) {
        // NTP was not updated yet
        return false;
    }
    Serial.println("NTP loop finished!");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
    return true;
}

void AwsPubSubClient::loop() {

    State new_state;
    switch(next_state) {
        case State::PhyConnect: new_state = handlePhyConnect();    break;
        case State::TimeSync: new_state = handleTimeSync();         break;
        case State::MqttConnect: new_state = handleMqttConnect();   break;
        case State::Connected: new_state = handleConnected();       break;
        case State::Disconnected: new_state = handleDisconnected(); break;
    }
    next_state = new_state;

    // Refresh MQTT client if it is still connected
    if (pub_sub_client.connected()) {
        pub_sub_client.loop();
    }
}

AwsPubSubClient::State AwsPubSubClient::handlePhyConnect() {
    if (!phy_connect_loop_limiter.process()) {
        return State::PhyConnect;   // Skipping this loop because it is too often
    }
    if (network_connection->check() == NetworkConnectionState::CONNECTED) {
        // The device got connected
        Serial.printf("AWS IoT: Physical network connectivity established. Moving to next state\n");
        return State::TimeSync;
    }
    return State::PhyConnect;
}

AwsPubSubClient::State AwsPubSubClient::handleTimeSync() {
    if  (!ntp_update_loop_limiter.process()) {
        return State::TimeSync;  // Skipping this loop because it is too often
    }
    if (_update_time()) {
        Serial.println("AWS IoT: Time was synchronised, moving to next state.");
        return State::MqttConnect;
    }

    return AwsPubSubClient::TimeSync;
}

AwsPubSubClient::State AwsPubSubClient::handleMqttConnect() {
    if (!mqtt_connect_loop_limiter.process()) {
        return State::MqttConnect;  // Skipping this loop because it is too often
    }
    if (pub_sub_client.connect(thing_name)) {
        Serial.println("AWS IoT: MQTT connection established. Subscribing to channels.");

        // Re-subscribe
        for (auto& it : subscriptions) {
            if (pub_sub_client.subscribe(it.c_str())) {
                Serial.printf("AWS IoT: Subscribed to topic %s successfully!\n", it.c_str());
            } else {
              Serial.printf("AWS IoT: Failed subscribing to topic %s!\n", it.c_str()    );
            }

        }
        connected = true;
        Serial.printf("AWS IoT: MQTT connection established!\n");
        return State::Connected;
    } else {
        char ssl_desc[SSL_ERROR_MSG_SIZE];
        int ssl_error_code = ssl_client.getLastSSLError(ssl_desc, SSL_ERROR_MSG_SIZE);


        Serial.printf("AWS IoT: Failed connecting to MQTT\n");
        Serial.printf("AWS IoT: SSL: connected: %d, error: [%d] %s\n", ssl_client.connected(), ssl_error_code, ssl_desc);

        return State::PhyConnect;
    }
}

AwsPubSubClient::State AwsPubSubClient::handleConnected() {
    if (!pub_sub_client.connected()) {
        Serial.println("AWS IoT: MQTT Client lost connection");
        return State::Disconnected;
    }

    return State::Connected;
}

AwsPubSubClient::State AwsPubSubClient::handleDisconnected() {
    Serial.println("AWS IoT: Client disconnected.");
    connected = false;
    pub_sub_client.disconnect();
    return State::PhyConnect;
}

void AwsPubSubClient::process_received_message(char *topic, byte *payload, unsigned int length) {
    Serial.printf("AWS IoT: New message arrived on topic %s\n", topic);
    if (message_callback_func) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
            Serial.printf("AWS IoT: deserializeMsgPack() failed for topic %s with message %s\n", topic, payload);
            Serial.println(error.f_str());
            return;
        } else {
            message_callback_func(topic, doc);
        }
    }
}
