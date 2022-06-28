//
// Created by Konstantinos Paliouras on 15/06/2022.
//

#ifndef DOOR_GUARD_CREDENTIALS_H
#define DOOR_GUARD_CREDENTIALS_H

#define SECRET_WIFI_SSID ""
#define SECRET_WIFI_PASS ""
#define SECRET_IOT_CLOUD_DEVICE_NAME ""
#define SECRET_IOT_CLOUD_DEVICE_KEY ""

// -------------------------------------------------
// AWS IoT Configuration

#define THINGNAME ""

const char MQTT_HOST[] = "";


static const char ca_cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";


// Copy contents from XXXXXXXX-certificate.pem.crt here ▼
static const char client_cert_str[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";


// Copy contents from  XXXXXXXX-private.pem.key here ▼
static const char priv_key_str[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";

#endif //DOOR_GUARD_CREDENTIALS_H
