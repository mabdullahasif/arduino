#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Joystick pins
int joyY = A3;   // <-- UPDATED
int joyBtn = 3;

// Menu state
int menuIndex = 0;
bool inMenu = true;

void setup() {
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  pinMode(joyBtn, INPUT_PULLUP);

  dht.begin();

  showMenu();
}

void loop() {
  readJoystickSerial();   // Debug

  if (inMenu) {
    handleMenu();
  } else {
    runOption(menuIndex);
  }
}

// ---------- SERIAL DEBUG ----------

void readJoystickSerial() {
  int y = analogRead(joyY);

  Serial.print("Y: ");
  Serial.print(y);

  if (y < 350) {
    Serial.println("  DOWN");
  }
  else if (y > 700) {
    Serial.println("  UP");
  }
  else {
    Serial.println("  CENTER");
  }

  delay(100);
}

// ---------- MENU ----------

void handleMenu() {
  int y = analogRead(joyY);

  static bool moved = false;

  int center = 520;
  int deadZone = 100;

  // Reset when centered
  if (y > center - deadZone && y < center + deadZone) {
    moved = false;
  }

  // Move DOWN → next option
  if (y < center - 200 && !moved) {
    menuIndex++;
    if (menuIndex > 1) menuIndex = 0;
    showMenu();
    moved = true;
  }

  // Move UP → previous option
  if (y > center + 200 && !moved) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = 1;
    showMenu();
    moved = true;
  }

  // Select
  if (digitalRead(joyBtn) == LOW) {
    delay(200);
    inMenu = false;
    lcd.clear();
  }
}

void showMenu() {
  lcd.clear();

  if (menuIndex == 0) {
    lcd.setCursor(0, 0);
    lcd.print("> Temp/Hum");

    lcd.setCursor(0, 1);
    lcd.print("  Name");
  } 
  else {
    lcd.setCursor(0, 0);
    lcd.print("  Temp/Hum");

    lcd.setCursor(0, 1);
    lcd.print("> Name");
  }
}

// ---------- OPTIONS ----------

void runOption(int option) {

  // Press again → back
  if (digitalRead(joyBtn) == LOW) {
    delay(200);
    inMenu = true;
    showMenu();
    return;
  }

  if (option == 0) {
    showTempHumidity();
  }

  if (option == 1) {
    showName();
  }
}

// ---------- FEATURES ----------

void showTempHumidity() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error   ");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring   ");
    delay(1000);
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(humidity);
  lcd.print("%   ");

  delay(500);
}

void showName() {
  lcd.setCursor(0, 0);
  lcd.print("Abdullah       ");

  lcd.setCursor(0, 1);
  lcd.print("Press to Back  ");
}