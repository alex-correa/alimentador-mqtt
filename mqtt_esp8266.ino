/*
Projeto: Alimentador automático de pets
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(D8, D7);

// Valores contendo os dados da rede wireless

const char* ssid = "...";
const char* password = "...";
const char* mqtt_server = "mqtt.eclipseprojects.io";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // Faz a conexao com a rede wireless
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Liga o LED se receber 1 como payload
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);
  } else if ((char)payload[0] == '0') {
    digitalWrite(BUILTIN_LED, HIGH);
  } else if ((char)payload[0] == '4') { // Teste envolvendo o rele conectado ao motor
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
  } else if ((char)payload[0] == '5') {
    digitalWrite(4, LOW);
  }
}

void reconnect() {
  // Repete ate reconectar
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Cria um client ID aleatorio
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Tentando conexao
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Uma vez conectado faz uma publicacao
      client.publish("publicacao_esp8266_271120211330", "hello world");
      // ... e subscreve novamente
      client.subscribe("subscricao_esp8266_271120211330");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Espera 5 segundos antes de tentar novamente
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    
    // Teste sensor de distância
    Serial.print("Distancia: ");
    Serial.print(ultrasonic.distanceRead());
    Serial.println(" cm");
    long distancia = ultrasonic.distanceRead();
    if (distancia <= 7 && distancia != 0) {
      Serial.println("Distancia menor ou igual 7 cm");
      digitalWrite(4, HIGH);
      delay(1000);
      digitalWrite(4, LOW);
    } else {
      Serial.println("Distancia maior que 7 cm");
    }
    delay(1000);
    
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("publicacao_esp8266_271120211330", msg);
  }
}
