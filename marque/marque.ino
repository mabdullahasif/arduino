#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

String message = "M Abdullah Asif";
String scrollText;

void setup()
{
  lcd.begin();
  lcd.backlight();

  // Add spaces equal to screen width before and after
  scrollText = "                " + message + "                ";
}

void loop()
{
  for (int i = 0; i < scrollText.length() - 15; i++)
  {
    lcd.setCursor(0, 0);
    lcd.print(scrollText.substring(i, i + 16));
    delay(550);
  }
}