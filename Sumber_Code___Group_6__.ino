#include <ESP8266WiFi.h>         // Tensilica 32-bit RISC CPU Xtensa LX106
#include <FirebaseArduino.h>      // IC CP2102.

#define FIREBASE_HOST "tubes-iotxmicro.firebaseio.com"
#define FIREBASE_AUTH "oO3HeiVUxdKZsu6JAVD0tXgxi79EL2XHKg8pRkhZ"
#define WIFI_SSID "TELEKOMUNIKASI"
#define WIFI_PASSWORD "d3tt4203"

int vibrationSensorPin = D5;
int indikatorHijau = D6;
int indikatorMerah = D4; 
int indikatorBuzzer = D2;
int indikatorKuning = D3;

long earthquake(){
  long measurement = pulseIn (vibrationSensorPin, HIGH); 
  return measurement;
}

void setup() {
  Serial.begin(9600);
  pinMode(vibrationSensorPin, INPUT); 
  pinMode(indikatorHijau, OUTPUT); 
  pinMode(indikatorMerah, OUTPUT); 
  pinMode(indikatorBuzzer, OUTPUT); 
  pinMode(indikatorKuning, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void aman(){
  tone(indikatorBuzzer, 3);
  delay(1000);
  noTone(indikatorBuzzer);
  delay(200);
}

void siaga(){
  tone(indikatorBuzzer, 5);
  delay(1000);
  noTone(indikatorBuzzer);
  delay(200);
}

void awas(){
  tone(indikatorBuzzer, 7);
  delay(1000);
  noTone(indikatorBuzzer);
  delay(200);
}



void loop() { 
  long g = earthquake();
  Firebase.setInt ("/Monitoring/gempa",g);
  if (g > 25000) {
    digitalWrite(indikatorHijau, LOW);
    digitalWrite(indikatorMerah, LOW);
    digitalWrite(indikatorBuzzer, HIGH);
    digitalWrite(indikatorKuning, LOW);
    Serial.print("Terdapat Getaran = ");
    Serial.println(g);
    Serial.println("Status : Besar (AWAS)");
    Serial.println();
    return awas();
    delay(500); 
  }
  else if  (g > 15000) {
    digitalWrite(indikatorHijau, LOW);
    digitalWrite(indikatorMerah, LOW);
    digitalWrite(indikatorBuzzer, HIGH);
    digitalWrite(indikatorKuning, HIGH);
    Serial.print("Terdapat Getaran = ");
    Serial.println(g);
    Serial.println("Status : Sedang (SIAGA)");
    Serial.println();
    return siaga();
    delay(500);
  }
  else if  (g > 5000){
    digitalWrite(indikatorHijau, HIGH);
    digitalWrite(indikatorMerah, LOW);
    digitalWrite(indikatorBuzzer, HIGH);
    digitalWrite(indikatorKuning, LOW);
    Serial.print("Terdapat Getaran = ");
    Serial.println(g);
    Serial.println("Status : Kecil (AMAN)");
    Serial.println();
    return aman();
    delay(500);
  }
  else if  (g < 5000){
    digitalWrite(indikatorBuzzer, LOW);
    digitalWrite(indikatorHijau, LOW);
    digitalWrite(indikatorMerah, LOW);
    digitalWrite(indikatorKuning, LOW);
    Serial.print("Terdapat Getaran = ");
    Serial.println(g);
    Serial.println("Status : Tidak Terdapat Getaran ! ( belum memenuhi )");
    Serial.println();
    delay(500);
  }
}
