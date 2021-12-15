#include <WiFi.h>
#include <PubSubClient.h>

char* ssid = "SEU WIFI";
char* password = "5UA SENHA";

const char* mqttServer = "pet.eletrica.ufpr.br";
const int mqttPort = 2283;
const char* mqttTopic = "lamp";
const char* mqttUser = "iopet";
const char* mqttPassword = "pet";

const int RELE = 13;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  String messageTemp;
 
  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }
  Serial.println("Mensagem:" + messageTemp);
  
  if (messageTemp == "ligar"){
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(RELE, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(1000); 
  }
  if (messageTemp == "desligar"){
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELE, HIGH);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  }
}


void setup() {
  Serial.begin(115200);
   
  WiFi.begin(ssid, password);

  pinMode(RELE, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(RELE, LOW);
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe(mqttTopic);
}
 
void loop() {
  client.loop();
 }
