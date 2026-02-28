#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
const unsigned long interval = 1000;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor Error");
      Serial.println("ERROR");
      return;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 2);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity, 2);
    lcd.print(" %");

    Serial.print("Temp:");
    Serial.print(temperature, 2);
    Serial.print(",Hum:");
    Serial.println(humidity, 2);
  }
}