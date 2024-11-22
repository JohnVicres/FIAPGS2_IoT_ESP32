/**
   ESP32 + DHT22 + Thermistor
*/

#include "DHTesp.h"
#include <WiFi.h>
#include <PubSubClient.h>

const char *default_SSID = "Wokwi-GUEST";
const char *default_PASSWORD = "";
const char *default_BROKER_MQTT = "23.22.163.175";
const int default_BROKER_PORT = 1883;
const char *default_TOPICO_SUBSCRIBE = "/TEF/device072/cmd";
const char *default_TOPICO_PUBLISH_1 = "/TEF/device072/attrs";
const char *default_TOPICO_PUBLISH_2 = "/TEF/device072/attrs/t1";
const char *default_TOPICO_PUBLISH_3 = "/TEF/device072/attrs/h";
const char *default_TOPICO_PUBLISH_4 = "/TEF/device072/attrs/t2";
const char *default_ID_MQTT = "fiware_072";

const char *topicPrefix = "device072";

char *SSID = const_cast<char *>(default_SSID);
char *PASSWORD = const_cast<char *>(default_PASSWORD);
char *BROKER_MQTT = const_cast<char *>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char *TOPICO_SUBSCRIBE = const_cast<char *>(default_TOPICO_SUBSCRIBE);
char *TOPICO_PUBLISH_1 = const_cast<char *>(default_TOPICO_PUBLISH_1);
char *TOPICO_PUBLISH_2 = const_cast<char *>(default_TOPICO_PUBLISH_2);
char *TOPICO_PUBLISH_3 = const_cast<char *>(default_TOPICO_PUBLISH_3);
char *TOPICO_PUBLISH_4 = const_cast<char *>(default_TOPICO_PUBLISH_4);
char *ID_MQTT = const_cast<char *>(default_ID_MQTT);

const int default_D33 = 2;
int D33 = default_D33;
const int default_D15 = 2;
int D15 = default_D15;

WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';

const int DHT_PIN = 15;
const int THERM_PIN = 33;
const float BETA = 3950;

DHTesp dhtSensor;

void initSerial()
{
    Serial.begin(1082742890);
}

void initWiFi()
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

void initMQTT()
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void setup()
{
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}
void loop()
{
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    MQTT.loop();
}

void InitOutput()
{
    pinMode(D33, OUTPUT);
    pinMode(D15, OUTPUT);
    digitalWrite(D33, LOW);
    digitalWrite(D15, LOW);
}

void VerificaConexoesWiFIEMQTT()
{
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

void reconectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    // Garantir que os medidores iniciem desligados
    digitalWrite(D33, LOW);
    digitalWrite(D15, LOW);
}

void reconnectMQTT()
{
    while (!MQTT.connected())
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT))
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        }
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (int i = 0; i < length; i++)
    {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    // Compara com o tópico recebido
    if (msg.equals(onTopic))
    {
        digitalWrite(D33, HIGH);
        digitalWrite(D15, HIGH);
        EstadoSaida = '1';
    }

    if (msg.equals(offTopic))
    {
        digitalWrite(D33, LOW);
        digitalWrite(D15, LOW);
        EstadoSaida = '0';
    }
}

void EnviaEstadoOutputMQTT()
{
    if (EstadoSaida == '1')
    {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        // DHT22
        TempAndHumidity data = dhtSensor.getTempAndHumidity();

        String dht_temp = String(data.temperature, 2);
        String dht_hum = String(data.humidity, 1);

        Serial.println("Temperatura do ar: " + dht_temp + "°C");
        Serial.println("Umidade do ar: " + dht_hum + "%");
        Serial.println("---");

        MQTT.publish(TOPICO_PUBLISH_2, dht_temp.c_str());
        MQTT.publish(TOPICO_PUBLISH_3, dht_hum.c_str());

        // NTC Thermistor
        int analogValue = analogRead(33);

        String coolant_temp = String(1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15);

        Serial.println("Temperatura do líquido de arrefecimento: " + coolant_temp + "°C");

        MQTT.publish(TOPICO_PUBLISH_4, coolant_temp.c_str());
    }

    if (EstadoSaida == '0')
    {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Sensores Desligados");
    }

    Serial.println("- Estado dos sensores enviados ao broker!");

    delay(2000);
}