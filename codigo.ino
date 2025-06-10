// Bibiliotecas
#include<ArduinoJson.h>
#include<DHT.h>
#include<LiquidCrystal_I2C.h>
// Pinos
#define DHTPIN 4
#define DHTTYPE 11
#define ledR 3
#define ledG 2
#define buzzerPin 5
// Variáveis Globais
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int MAX_TEMP = 25;
const int MIN_TEMP = 20;
const int MAX_UMID = 65;
const int MIN_UMID = 40;
int temp = 0;
int umid = 0;
bool tempAlert = false;
bool umidAlert = false;
 
 
void checkSensor() {
  temp = dht.readTemperature();
  umid = dht.readHumidity();
  StaticJsonDocument<100>json;
  json["Temperatura"] = temp;
  json["Umidade"] = umid;
  serializeJson(json, Serial);
  Serial.println();
}
 
void setup() {
  Serial.begin(9600);
  Serial.println("Serial aberta!\n");
  dht.begin();
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  lcd.backlight();
}
 
void loop() {
  lcd.clear();
 
  //Temperatura
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223);  // Símbolo de grau
  lcd.print("C");
  if (temp > MAX_TEMP) {
    lcd.print(" ALTO!");
    tempAlert = true;
  } else if (temp < MIN_TEMP) {
    lcd.print(" BAIXO!");
    tempAlert = true;
  } else {
    lcd.print(" OK!");
    tempAlert = false;
  }
 
  //Umidade
  lcd.setCursor(0, 1);
  lcd.print("Umid: ");
  lcd.print(umid, 1);
  lcd.print("%");
  if (umid > MAX_UMID) {
    lcd.print(" ALTO!");
    umidAlert = true;
  } else if (umid < MIN_UMID) {
    lcd.print(" BAIXO!");
    umidAlert = true;
  } else {
    lcd.print(" OK!");
    umidAlert = false;
  }
 
  //Alertas
  if (tempAlert == true || umidAlert == true) {
    for (int a = 0; a < 2; a++) {
      for (int b = 0; b < 2; b++) {
        tone(buzzerPin, 1000);
        delay(150);
        noTone(buzzerPin);
        delay(150);
      }
      digitalWrite(ledG, LOW);
      digitalWrite(ledR, HIGH);
      delay(1500);
    }
  } else {
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    delay(3600);
  }
 
  checkSensor();
  delay(1500);
}
