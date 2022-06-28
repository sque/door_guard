## AWS Iot Pub/Sub client

This is a small library to implement a robust interface to AWS IoT similar to Arduino Cloud IoT

The library supports:
 * Certificate based authentication
 * NTP updates
 * Connection management based on Arduino native Connection Management

## Example

```c++
#include <Arduino_WiFiConnectionHandler.h>
#include <AwsPubSubClient.h>


const char WIFI_SSID[] = "";
const char WIFI_PASSWORD[] = "";

#define THINGNAME "" // The name of the thing in AWS

int8_t TIME_ZONE = 0; 

const char MQTT_HOST[] = "XXXXXXX.iot.eu-central-1.amazonaws.com";


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


#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"

AwsPubSubClient & aws_client = AwsPubSubClient::getInstance();
WiFiConnectionHandler wifi_connection(WIFI_SSID, WIFI_PASSWORD);


void document_received(const char * topic, const JsonDocument & doc) {
    Serial.printf("AWS IoT: Document received %d", doc.size());
}

void setup() {
    aws_client.begin(wifi_connection,THINGNAME, ca_cert_str, client_cert_str,
                     priv_key_str, MQTT_HOST);
    aws_client.set_message_callback(document_received);
    aws_client.add_subscription(AWS_IOT_SUBSCRIBE_TOPIC);
}

void loop()
{

    aws_client.loop();
}
```